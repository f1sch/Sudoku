#include "../shared/Structs.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>

#include <imgui.h>
#include <imgui-SFML.h>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <numeric>
#include <string>
#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <utility>

namespace fs = std::filesystem;

constexpr unsigned TILE_SIZE = 32u;
constexpr unsigned BOARD_TILES = 9u;
constexpr unsigned BOARD_SIZE = BOARD_TILES * TILE_SIZE;

constexpr unsigned PADDING_TILES = 5u;
constexpr unsigned TOTAL_TILES = BOARD_TILES + PADDING_TILES * 2u;
constexpr unsigned VIRTUAL_WIDTH = TOTAL_TILES * TILE_SIZE;
constexpr unsigned VIRTUAL_HEIGHT = VIRTUAL_WIDTH;

constexpr unsigned BOARD_LEFT = PADDING_TILES + TILE_SIZE;
constexpr unsigned BOARD_TOP = PADDING_TILES + TILE_SIZE;
constexpr unsigned BOARD_RIGHT = BOARD_LEFT + BOARD_SIZE;
constexpr unsigned BOARD_BOTTOM = BOARD_TOP + BOARD_SIZE;

constexpr float RULER_SIZE = 20.f;

constexpr unsigned EDITOR_PANEL_WIDTH = 700u;
constexpr unsigned EDITOR_WIDTH = VIRTUAL_WIDTH + EDITOR_PANEL_WIDTH;
constexpr unsigned EDITOR_HEIGHT = VIRTUAL_HEIGHT + 500u;

enum class EditorMode { Place, Move };

using SpriteList = std::vector<std::unique_ptr<SpriteEntry>>;

static std::vector<std::string> collectTextures(const std::string& rootDir)
{
    std::vector<std::string> result;
    if (!fs::exists(rootDir)) return result;
    for (auto& entry : fs::recursive_directory_iterator(rootDir)) 
    {
        if (!entry.is_regular_file()) continue;
        auto ext = entry.path().extension().string();
        if (ext == ".png" || ext == ".jpg" || ext == ".bmp")
            result.push_back(entry.path().string());
    }
    return result;
}

static std::vector<int> sortedByLayer(const SpriteList& sprites)
{
    std::vector<int> order(sprites.size());
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](int a, int b) {
        return sprites[a]->layer < sprites[b]->layer;
    });
    return order;
}

static sf::Vector2f snapToGrid(sf::Vector2f pos)
{
    const float ts = static_cast<float>(TILE_SIZE);
    return { std::floor(pos.x / ts) * ts, std::floor(pos.y / ts) * ts };
}

using json = nlohmann::json;

static void exportScene(const SpriteList& sprites, const std::string& file)
{
    json scene;

    for (const auto& s : sprites)
    {
        json sprite;

        sprite["texturePath"] = s->texturePath;
        sprite["x"] = s->x;
        sprite["y"] = s->y;
        sprite["tileX"] = (static_cast<int>(s->x) / TILE_SIZE) - PADDING_TILES;
        sprite["tileY"] = (static_cast<int>(s->y) / TILE_SIZE) - PADDING_TILES;
        sprite["scaleX"] = s->scaleX;
        sprite["scaleY"] = s->scaleY;
        sprite["layer"] = s->layer;
        sprite["fromTilemap"] = s->fromTilemap;

        if (s->fromTilemap)
        {
            sprite["textureRect"] = {
                { "left", s->tileRect.position.x},
                { "top", s->tileRect.position.y},
                { "width", s->tileRect.size.x},
                { "height", s->tileRect.size.y},
            };
            sprite["tilemapCol"] = s->tileCol;
            sprite["tilemapRow"] = s->tileRow;
            sprite["tilemapTileW"] = s->tileRect.size.x;
            sprite["tilemapTileH"] = s->tileRect.size.y;
        }

        scene["sprites"].push_back(sprite);
    }

    std::ofstream out(file);
    out << scene.dump(4);
}

static void drawViewportGrid(sf::RenderTexture& rt)
{
    std::vector<sf::Vertex> lines;

    const int width = static_cast<int>(VIRTUAL_WIDTH);
    const int height = static_cast<int>(VIRTUAL_HEIGHT);
    const int tileSize = static_cast<int>(TILE_SIZE);

    const int midX_i = (width / 2 / tileSize) * tileSize;
    const int midY_i = (height / 2 / tileSize) * tileSize;

    // TODO: refactor drawing center lines in own function
    const sf::Color colGrid(255, 255, 255);
    const sf::Color colCenter(0, 255, 0);

    // vertical
    for (int x = 0; x <= width; x += tileSize)
    {
        //sf::Color c = (x == midX_i) ? colCenter : colGrid;
        sf::Color c = colGrid;
        float fx = static_cast<float>(x);
        lines.push_back({ sf::Vector2f(fx, 0.f), c });
        lines.push_back({ sf::Vector2f(fx, static_cast<float>(height)), c });
    }
    // horizontal
    for (int y = 0; y <= height; y += tileSize)
    {
        //sf::Color c = (y == midY_i) ? colCenter : colGrid;
        sf::Color c = colGrid;
        float fy = static_cast<float>(y);
        lines.push_back({ sf::Vector2f(0.f, fy), c });
        lines.push_back({ sf::Vector2f(static_cast<float>(width), fy), c });
    }

    rt.draw(lines.data(), lines.size(), sf::PrimitiveType::Lines);
}

static void drawBoardBoundary(sf::RenderTexture& rt)
{
    const float x = static_cast<float>((VIRTUAL_WIDTH - BOARD_SIZE) / 2);
    const float y = static_cast<float>((VIRTUAL_HEIGHT - BOARD_SIZE) / 2);
    const float s = static_cast<float>(BOARD_SIZE);

    sf::RectangleShape fill({ s, s });
    fill.setPosition({ x, y });
    fill.setFillColor(sf::Color(30, 60, 30, 40));
    fill.setOutlineColor(sf::Color(80, 180, 80, 200));
    fill.setOutlineThickness(1.f);
    rt.draw(fill);

    //std::vector<sf::Vertex> cross;
    //constexpr float m = 6.f;
    //sf::Color color(80, 180, 80, 200);
    //cross.push_back({ sf::Vector2f({l - m, t}), color });
    //cross.push_back({ sf::Vector2f({l + m, t}), color });
    //cross.push_back({ sf::Vector2f({l, t - m}), color });
    //cross.push_back({ sf::Vector2f({l, t + m}), color });

    //rt.draw(cross.data(), cross.size(), sf::PrimitiveType::Lines);
}

static void drawViewportRuler(ImDrawList* dl, ImVec2 imageOrigin)
{
    const float ts = static_cast<float>(TILE_SIZE);
    const ImU32 colBoard = IM_COL32(100, 200, 100, 255);

    for (int i = 0; i < static_cast<int>(BOARD_TILES); ++i)
    {
        float tileCenter = (static_cast<float>(i) + 0.5f) * ts;
        char  buf[8];
        snprintf(buf, sizeof(buf), "%d", i);

        ImVec2 textSize = ImGui::CalcTextSize(buf);

        float topX = (imageOrigin.x + tileCenter - textSize.x * 0.5f) + PADDING_TILES * TILE_SIZE;
        float topY = imageOrigin.y - RULER_SIZE + (RULER_SIZE - textSize.y) * 0.5f;
        dl->AddText(ImVec2(topX, topY), colBoard, buf);

        float leftX = imageOrigin.x - RULER_SIZE + (RULER_SIZE - textSize.x) * 0.5f;
        float leftY = (imageOrigin.y + tileCenter - textSize.y * 0.5f) + PADDING_TILES * TILE_SIZE;
        dl->AddText(ImVec2(leftX, leftY), colBoard, buf);
    }
}

static void drawTexturePanel(const std::vector<std::string>& textures, int& selectedTexIdx)
{
    ImGui::Begin("Textures");
    ImGui::Text("Assets/");
    ImGui::Separator();

    for (int i = 0; i < static_cast<int>(textures.size()); ++i) 
    {
        std::string label = fs::path(textures[i]).filename().string();
        bool sel = (i == selectedTexIdx);
        if (ImGui::Selectable(label.c_str(), sel))
            selectedTexIdx = i;
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("%s", textures[i].c_str());
    }
    ImGui::End();
}

static void drawScenePanel(SpriteList& sprites, int& selectedSpriteIdx)
{
    ImGui::Begin("Scene");
    ImGui::Text("Sprites (%zu)", sprites.size());
    ImGui::Separator();

    for (int i = 0; i < static_cast<int>(sprites.size()); ++i) 
    {
        std::string label = "[" + std::to_string(i) + "] "
            + fs::path(sprites[i]->texturePath).filename().string();
        bool sel = (i == selectedSpriteIdx);
        if (ImGui::Selectable(label.c_str(), sel))
            selectedSpriteIdx = i;
    }

    if (selectedSpriteIdx >= 0 && selectedSpriteIdx < static_cast<int>(sprites.size())) 
    {
        ImGui::Separator();
        if (ImGui::Button("Remove")) 
        {
            sprites.erase(sprites.begin() + selectedSpriteIdx);
            selectedSpriteIdx = -1;
        }
    }
    ImGui::End();
}

static void drawPropertiesPanel(SpriteList& sprites, int selectedSpriteIdx)
{
    ImGui::Begin("Properties");

    if (selectedSpriteIdx < 0 || selectedSpriteIdx >= static_cast<int>(sprites.size())) 
    {
        ImGui::TextDisabled("No Sprite selected.");
        ImGui::End();
        return;
    }

    SpriteEntry& s = *sprites[selectedSpriteIdx];
    ImGui::Text("Texture: %s", fs::path(s.texturePath).filename().string().c_str());
    ImGui::Separator();

    int tileX = static_cast<int>(s.x) / static_cast<int>(TILE_SIZE) - PADDING_TILES;
    int tileY = static_cast<int>(s.y) / static_cast<int>(TILE_SIZE) - PADDING_TILES;
    ImGui::TextDisabled("Grid index: [%d, %d]", tileX, tileY);
    ImGui::Separator();

    bool changed = false;
    changed |= ImGui::DragFloat("X", &s.x, 1.f);
    changed |= ImGui::DragFloat("Y", &s.y, 1.f);
    changed |= ImGui::DragFloat("Scale X", &s.scaleX, 0.01f, 0.01f, 20.f);
    changed |= ImGui::DragFloat("Scale Y", &s.scaleY, 0.01f, 0.01f, 20.f);
    changed |= ImGui::InputInt("Layer", &s.layer);

    if (changed) 
    {
        s.sprite.setPosition({ s.x, s.y });
        s.sprite.setScale({ s.scaleX, s.scaleY });
    }
    ImGui::End();
}

static void drawModeToolbar(EditorMode& mode, bool& snapEnabled, int selectedTexIdx,
    const std::vector<std::string>& textures)
{
    ImGui::Begin("Tools");

    bool placeActive = (mode == EditorMode::Place);
    if (placeActive) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.f));
    if (ImGui::Button("[ Place ] (P)", ImVec2(140, 0))) mode = EditorMode::Place;
    if (placeActive) ImGui::PopStyleColor();

    ImGui::SameLine();

    bool moveActive = (mode == EditorMode::Move);
    if (moveActive) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.8f, 1.f));
    if (ImGui::Button("[ Move ] (M)", ImVec2(140, 0))) mode = EditorMode::Move;
    if (moveActive) ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::Checkbox("Snap to Grid", &snapEnabled);

    ImGui::Separator();

    if (mode == EditorMode::Place) 
    {
        if (selectedTexIdx >= 0)
            ImGui::Text("Texture: %s", fs::path(textures[selectedTexIdx]).filename().string().c_str());
        else
            ImGui::TextDisabled("No Texture selected");
    }
    else 
    {
        ImGui::TextDisabled("Click to select | Drag to move | Del to remove");
    }

    ImGui::Separator();
    ImGui::TextDisabled("Board: %upx | offset: %u,%u | %u%u tiles @ %upx",
        BOARD_SIZE, BOARD_LEFT, BOARD_TOP, BOARD_TILES, BOARD_TILES, TILE_SIZE);

    ImGui::End();
}

struct ViewportResult
{
    bool clicked = false;
    bool dragging = false;
    sf::Vector2f worldPos;
    sf::Vector2f dragDelta;
};

static ViewportResult drawViewportPanel(const sf::RenderTexture& rt)
{
    ViewportResult result;

    const float windowWidth = static_cast<float>(VIRTUAL_WIDTH) + RULER_SIZE + 16.f;
    const float windowHeight = static_cast<float>(VIRTUAL_HEIGHT) + RULER_SIZE + 36.f;

    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight),ImGuiCond_Always);

    ImGui::Begin("Viewport", nullptr,
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoMove);

    ImVec2 cursorStart = ImGui::GetCursorPos();
    ImGui::SetCursorPos(ImVec2(cursorStart.x + RULER_SIZE, cursorStart.y + RULER_SIZE));

    ImVec2 imageOrigin = ImGui::GetCursorScreenPos();
    ImGui::Image(rt, sf::Vector2f(VIRTUAL_WIDTH, VIRTUAL_HEIGHT));

    drawViewportRuler(ImGui::GetWindowDrawList(), imageOrigin);

    auto toWorld = [&](ImVec2 mouse) -> sf::Vector2f 
        { 
            return { mouse.x - imageOrigin.x, mouse.y - imageOrigin.y }; 
        };

    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            ImVec2 delta = ImGui::GetIO().MouseDelta;
            if (delta.x != 0.f || delta.y != 0.f)
            {
                result.dragging = true;
                result.worldPos = toWorld(ImGui::GetMousePos());
                result.dragDelta = { delta.x, delta.y };
            }
        }
        else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            result.clicked = true;
            result.worldPos = toWorld(ImGui::GetMousePos());
        }
    }

    ImGui::End();
    return result;
}

static bool drawTilemapPanel(TilemapAsset& tm, int& selCol, int& selRow)
{
    bool changed = false;
    ImGui::Begin("Tilemap");

    static char pathBuf[256] = "assets/textures/sudokuTilemap.png";
    static int tileW = 32, tileH = 32;
    ImGui::InputText("Path", pathBuf, sizeof(pathBuf));
    ImGui::InputInt("Tile W", &tileW);
    ImGui::InputInt("Tile H", &tileH);
    if (ImGui::Button("Load Tilemap"))
        tm.load(pathBuf, tileW, tileH);

    if (tm.cols == 0) 
    { 
        ImGui::TextDisabled("No tilemap loaded."); 
        ImGui::End(); 
        return false; 
    };

    ImGui::Separator();
    ImGui::Text("%s (%dx%d tiles", fs::path(tm.path).filename().string().c_str(), tm.cols, tm.rows);
    ImGui::Separator();

    ImVec2 origin = ImGui::GetCursorScreenPos();
    const float dispW = static_cast<float>(tm.texture.getSize().x);
    const float dispH = static_cast<float>(tm.texture.getSize().y);
    ImGui::Image(tm.texture, sf::Vector2f(dispW, dispH));

    ImDrawList* dl = ImGui::GetWindowDrawList();
    const float tw = static_cast<float>(tm.tileW);
    const float th = static_cast<float>(tm.tileH);
    const ImU32 colGrid = IM_COL32(80, 80, 80, 180);
    const ImU32 colSel = IM_COL32(100, 200, 255, 180);

    for (int c = 0; c <= tm.cols; ++c)
    {
        dl->AddLine(ImVec2(origin.x + c * tw, origin.y),
            ImVec2(origin.x + c * tw, origin.y + dispH), colGrid);
    }
    for (int r = 0; r <= tm.rows; ++r)
    {
        dl->AddLine(ImVec2(origin.x, origin.y + r * th), 
            ImVec2(origin.x + dispW, origin.y + r * th), colGrid);
    }

    if (selCol >= 0 && selRow >= 0)
    {
        ImVec2 tl(origin.x + selCol * tw, origin.y + selRow * th);
        ImVec2 br(tl.x + tw, tl.y + th);
        dl->AddRect(tl, br, colSel, 0.f, 0, 2.f);
    }

    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        ImVec2 mouse = ImGui::GetMousePos();
        int c = static_cast<int>((mouse.x - origin.x) / tw);
        int r = static_cast<int>((mouse.y - origin.y) / th);
        if (c >= 0 && c < tm.cols && r >= 0 && r < tm.rows)
        {
            selCol = c;
            selRow = r;
            changed = true;
        }
    }

    ImGui::End();
    return changed;
}

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({ EDITOR_WIDTH, EDITOR_HEIGHT }),
        "Sudoku Editor",
        sf::Style::Default
    );
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window)) 
    {
        // TODO: error
    }

    sf::RenderTexture sceneRT;
    if (!sceneRT.resize({ VIRTUAL_WIDTH, VIRTUAL_HEIGHT }))
    {
        // TODO: error
    }
    sceneRT.clear(sf::Color(20, 20, 20));
    sceneRT.display();

    std::vector<std::string> textures = collectTextures("assets");
    SpriteList sprites;
    EditorMode mode = EditorMode::Move;
    bool snapEnabled = true;
    int  selectedTexIdx = -1;
    int  selectedSpriteIdx = -1;

    TilemapAsset tm;
    int tmSelCol = -1, tmSelRow = -1;

    sf::Clock deltaClock;

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
                window.close();

            // Keybinds: M = Move, P = Place, Entf = delete Sprite
            if (const auto* kb = event->getIf<sf::Event::KeyPressed>()) 
            {
                if (kb->code == sf::Keyboard::Key::M) mode = EditorMode::Move;
                if (kb->code == sf::Keyboard::Key::P) mode = EditorMode::Place;
                if (kb->code == sf::Keyboard::Key::Delete && selectedSpriteIdx >= 0) {
                    sprites.erase(sprites.begin() + selectedSpriteIdx);
                    selectedSpriteIdx = -1;
                }
            }
        }
        // ImGui Update
        ImGui::SFML::Update(window, deltaClock.restart());

        sceneRT.clear(sf::Color(20, 20, 20));
        drawViewportGrid(sceneRT);
        drawBoardBoundary(sceneRT);

        for (int idx : sortedByLayer(sprites))
        {
            sceneRT.draw(sprites[idx]->sprite);

            if (idx == selectedSpriteIdx) 
            {
                auto bounds = sprites[idx]->sprite.getGlobalBounds();
                sf::RectangleShape outline({ bounds.size.x, bounds.size.y });
                outline.setPosition(bounds.position);
                outline.setFillColor(sf::Color::Transparent);
                outline.setOutlineColor(sf::Color(100, 200, 255));
                outline.setOutlineThickness(2.f);
                sceneRT.draw(outline);
            }
        }

        sceneRT.display();

        if (ImGui::BeginMainMenuBar()) 
        {
            if (ImGui::BeginMenu("File")) 
            {
                if (ImGui::MenuItem("Export Scene")) { exportScene(sprites, "scene.json"); }
                if (ImGui::MenuItem("Load Scene")) { /* TODO */ }
                ImGui::Separator();
                if (ImGui::MenuItem("Quit")) window.close();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Assets")) 
            {
                if (ImGui::MenuItem("Load Assets"))
                    textures = collectTextures("assets");
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        drawModeToolbar(mode, snapEnabled, selectedTexIdx, textures);
        drawTexturePanel(textures, selectedTexIdx);
        drawTilemapPanel(tm, tmSelCol, tmSelRow);
        drawScenePanel(sprites, selectedSpriteIdx);
        drawPropertiesPanel(sprites, selectedSpriteIdx);

        // Viewport
        ViewportResult vp = drawViewportPanel(sceneRT);

        if (vp.clicked && mode == EditorMode::Place)
        {
            sf::Vector2f pos = snapEnabled ? snapToGrid(vp.worldPos) : vp.worldPos;

            if (tmSelCol >= 0 && tmSelRow >= 0 && tm.cols > 0) 
            {
                sf::IntRect rect = tm.rectAt(tmSelCol, tmSelRow);
                auto entry = std::make_unique<SpriteEntry>(tm.path, pos.x, pos.y, rect);
                entry->tileCol = tmSelCol;
                entry->tileRow = tmSelRow;
                sprites.push_back(std::move(entry));
                selectedSpriteIdx = static_cast<int>(sprites.size()) - 1;
            }
            else if (selectedTexIdx >= 0)
            {
                // Place new Sprite
                sprites.push_back(std::make_unique<SpriteEntry>(
                    textures[selectedTexIdx], pos.x, pos.y));
                selectedSpriteIdx = static_cast<int>(sprites.size()) - 1;
            }
            else if (mode == EditorMode::Move)
            {
                selectedSpriteIdx = -1;
                for (int i = static_cast<int>(sprites.size()) - 1; i >= 0; --i) 
                {
                    if (sprites[i]->sprite.getGlobalBounds().contains(vp.worldPos)) 
                    {
                        selectedSpriteIdx = i;
                        break;
                    }
                }
            }
        }

        if (vp.dragging && mode == EditorMode::Move && selectedSpriteIdx >= 0)
        {
            auto& s = *sprites[selectedSpriteIdx];
            s.x += vp.dragDelta.x;
            s.y += vp.dragDelta.y;

            s.sprite.setPosition({ s.x, s.y });
        }
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && snapEnabled && selectedSpriteIdx >= 0)
        {
            auto& s = *sprites[selectedSpriteIdx];
            sf::Vector2f snapped = snapToGrid({ s.x, s.y });
            s.x = snapped.x;
            s.y = snapped.y;

            s.sprite.setPosition({ s.x, s.y });
        }

        // main window
        window.clear(sf::Color(30, 30, 30));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}