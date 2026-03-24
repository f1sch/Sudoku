#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include <algorithm>
#include <filesystem>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

namespace fs = std::filesystem;

constexpr unsigned GAME_WINDOW_WIDTH = 1280u;
constexpr unsigned GAME_WINDOW_HEIGHT = 720u;

enum class EditorMode { Place, Move };

struct SpriteEntry 
{
    std::string texturePath;
    sf::Texture texture;
    sf::Sprite  sprite{ texture };
    float x = 0.f, y = 0.f;
    float scaleX = 1.f, scaleY = 1.f;
    int layer = 0;

    SpriteEntry(const std::string& path, float px, float py)
        : texturePath(path), x(px), y(py)
    {
        texture.loadFromFile(path);
        sprite.setTexture(texture, true);
        sprite.setPosition({ px, py });
    }

    SpriteEntry(const SpriteEntry&) = delete;
    SpriteEntry& operator=(const SpriteEntry&) = delete;
};

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

static void drawModeToolbar(EditorMode& mode, int selectedTexIdx,
    const std::vector<std::string>& textures)
{
    ImGui::Begin("Tools");

    bool placeActive = (mode == EditorMode::Place);
    if (placeActive) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.f));
    if (ImGui::Button("[ Place ]", ImVec2(130, 0))) mode = EditorMode::Place;
    if (placeActive) ImGui::PopStyleColor();

    ImGui::SameLine();

    bool moveActive = (mode == EditorMode::Move);
    if (moveActive) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.8f, 1.f));
    if (ImGui::Button("[ Move ]", ImVec2(130, 0))) mode = EditorMode::Move;
    if (moveActive) ImGui::PopStyleColor();

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
        ImGui::TextDisabled("Click to select / move");
    }

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

    ImGui::SetNextWindowSize(
        ImVec2(static_cast<float>(GAME_WINDOW_WIDTH) + 16.f,
            static_cast<float>(GAME_WINDOW_HEIGHT) + 36.f),
        ImGuiCond_Always);

    ImGui::Begin("Viewport", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

    ImVec2 imageOrigin = ImGui::GetCursorScreenPos();

    ImGui::Image(rt, sf::Vector2f(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT));

    auto toWorld = [&](ImVec2 mouse) -> sf::Vector2f 
        { 
            return { mouse.x - imageOrigin.x, mouse.y - imageOrigin.y }; 
        };

    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            result.clicked = true;
            result.worldPos = toWorld(ImGui::GetMousePos());
        }
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            ImVec2 delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
            ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
            result.dragging = true;
            result.worldPos = toWorld(ImGui::GetMousePos());
            result.dragDelta = { delta.x, delta.y };
        }
    }

    ImGui::End();
    return result;
}

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({ GAME_WINDOW_WIDTH + 700u, GAME_WINDOW_HEIGHT + 60u }),
        "Sudoku Editor",
        sf::Style::Default
    );
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window)) 
    {
        // TODO: error
    }

    sf::RenderTexture sceneWindow;
    if (!sceneWindow.resize({ GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT }))
    {
        // TODO: error
    }
    sceneWindow.clear(sf::Color(20, 20, 20));
    sceneWindow.display();

    std::vector<std::string> textures = collectTextures("assets");
    SpriteList sprites;
    EditorMode mode = EditorMode::Move;
    int  selectedTexIdx = -1;
    int  selectedSpriteIdx = -1;

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

        sceneWindow.clear(sf::Color(20, 20, 20));

        for (int idx : sortedByLayer(sprites))
        {
            sceneWindow.draw(sprites[idx]->sprite);

            if (idx == selectedSpriteIdx) 
            {
                auto bounds = sprites[idx]->sprite.getGlobalBounds();
                sf::RectangleShape outline({ bounds.size.x, bounds.size.y });
                outline.setPosition(bounds.position);
                outline.setFillColor(sf::Color::Transparent);
                outline.setOutlineColor(sf::Color(100, 200, 255));
                outline.setOutlineThickness(2.f);
                sceneWindow.draw(outline);
            }
        }

        sceneWindow.display();

        if (ImGui::BeginMainMenuBar()) 
        {
            if (ImGui::BeginMenu("File")) 
            {
                if (ImGui::MenuItem("Export Scene")) { /* TODO */ }
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

        drawModeToolbar(mode, selectedTexIdx, textures);
        drawTexturePanel(textures, selectedTexIdx);
        drawScenePanel(sprites, selectedSpriteIdx);
        drawPropertiesPanel(sprites, selectedSpriteIdx);

        // Viewport
        ViewportResult vp = drawViewportPanel(sceneWindow);

        if (vp.clicked)
        {
            if (mode == EditorMode::Place && selectedTexIdx >= 0) 
            {
                // Place new Sprite
                sprites.push_back(std::make_unique<SpriteEntry>(
                    textures[selectedTexIdx], vp.worldPos.x, vp.worldPos.y));
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
            s.x = vp.dragDelta.x;
            s.y = vp.dragDelta.y;
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