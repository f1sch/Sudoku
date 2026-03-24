#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
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

enum class EditorMode { Place, Move };

struct SpriteEntry {
    std::string texturePath;
    sf::Texture texture;
    sf::Sprite  sprite{ texture };
    float x = 0.f, y = 0.f;
    float scaleX = 1.f, scaleY = 1.f;
    int   layer = 0;

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

std::vector<std::string> collectTextures(const std::string& rootDir)
{
    std::vector<std::string> result;
    if (!fs::exists(rootDir)) return result;
    for (auto& entry : fs::recursive_directory_iterator(rootDir)) {
        if (!entry.is_regular_file()) continue;
        auto ext = entry.path().extension().string();
        if (ext == ".png" || ext == ".jpg" || ext == ".bmp")
            result.push_back(entry.path().string());
    }
    return result;
}

void drawTexturePanel(const std::vector<std::string>& textures, int& selectedTexIdx)
{
    ImGui::Begin("Textures");
    ImGui::Text("Assets/");
    ImGui::Separator();

    for (int i = 0; i < static_cast<int>(textures.size()); ++i) {
        std::string label = fs::path(textures[i]).filename().string();
        bool sel = (i == selectedTexIdx);
        if (ImGui::Selectable(label.c_str(), sel))
            selectedTexIdx = i;
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("%s", textures[i].c_str());
    }
    ImGui::End();
}

void drawScenePanel(SpriteList& sprites, int& selectedSpriteIdx)
{
    ImGui::Begin("Scene");
    ImGui::Text("Sprites (%zu)", sprites.size());
    ImGui::Separator();

    for (int i = 0; i < static_cast<int>(sprites.size()); ++i) {
        std::string label = "[" + std::to_string(i) + "] "
            + fs::path(sprites[i]->texturePath).filename().string();
        bool sel = (i == selectedSpriteIdx);
        if (ImGui::Selectable(label.c_str(), sel))
            selectedSpriteIdx = i;
    }

    if (selectedSpriteIdx >= 0 && selectedSpriteIdx < static_cast<int>(sprites.size())) {
        ImGui::Separator();
        if (ImGui::Button("Remove")) {
            sprites.erase(sprites.begin() + selectedSpriteIdx);
            selectedSpriteIdx = -1;
        }
    }
    ImGui::End();
}

void drawPropertiesPanel(SpriteList& sprites, int selectedSpriteIdx)
{
    ImGui::Begin("Properties");

    if (selectedSpriteIdx < 0 || selectedSpriteIdx >= static_cast<int>(sprites.size())) {
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

    if (changed) {
        s.sprite.setPosition({ s.x, s.y });
        s.sprite.setScale({ s.scaleX, s.scaleY });
    }
    ImGui::End();
}

void drawModeToolbar(EditorMode& mode, int selectedTexIdx,
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

    if (mode == EditorMode::Place) {
        if (selectedTexIdx >= 0)
            ImGui::Text("Texture: %s", fs::path(textures[selectedTexIdx]).filename().string().c_str());
        else
            ImGui::TextDisabled("No Texture selected");
    }
    else {
        ImGui::TextDisabled("Click to select / move");
    }

    ImGui::End();
}

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({ 1400u, 900u }),
        "Sudoku Editor",
        sf::Style::Default
    );
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    std::vector<std::string> textures = collectTextures("assets");
    SpriteList               sprites;
    EditorMode               mode = EditorMode::Move;
    int  selectedTexIdx = -1;
    int  selectedSpriteIdx = -1;
    bool isDragging = false;
    sf::Vector2f dragOffset;

    sf::Clock deltaClock;

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mb->button == sf::Mouse::Button::Left && !ImGui::GetIO().WantCaptureMouse) {
                    sf::Vector2f worldPos = window.mapPixelToCoords({ mb->position.x, mb->position.y });

                    if (mode == EditorMode::Place && selectedTexIdx >= 0) {
                        // Place new Sprite
                        sprites.push_back(std::make_unique<SpriteEntry>(
                            textures[selectedTexIdx], worldPos.x, worldPos.y));
                        selectedSpriteIdx = static_cast<int>(sprites.size()) - 1;
                        // Switch to MoveMode to avoid placing another
                        isDragging = true;
                        dragOffset = { 0.f, 0.f };

                    }
                    else if (mode == EditorMode::Move) {
                        selectedSpriteIdx = -1;
                        for (int i = static_cast<int>(sprites.size()) - 1; i >= 0; --i) {
                            if (sprites[i]->sprite.getGlobalBounds().contains(worldPos)) {
                                selectedSpriteIdx = i;
                                isDragging = true;
                                dragOffset = worldPos - sf::Vector2f(sprites[i]->x, sprites[i]->y);
                                break;
                            }
                        }
                    }
                }
            }

            if (const auto* mb = event->getIf<sf::Event::MouseButtonReleased>())
                if (mb->button == sf::Mouse::Button::Left)
                    isDragging = false;

            if (const auto* mm = event->getIf<sf::Event::MouseMoved>()) {
                if (isDragging && selectedSpriteIdx >= 0 && !ImGui::GetIO().WantCaptureMouse) {
                    sf::Vector2f worldPos = window.mapPixelToCoords({ mm->position.x, mm->position.y });
                    auto& s = *sprites[selectedSpriteIdx];
                    s.x = worldPos.x - dragOffset.x;
                    s.y = worldPos.y - dragOffset.y;
                    s.sprite.setPosition({ s.x, s.y });
                }
            }

            // Keybinds: M = Move, P = Place, Entf = delete Sprite
            if (const auto* kb = event->getIf<sf::Event::KeyPressed>()) {
                if (kb->code == sf::Keyboard::Key::M) mode = EditorMode::Move;
                if (kb->code == sf::Keyboard::Key::P) mode = EditorMode::Place;
                if (kb->code == sf::Keyboard::Key::Delete && selectedSpriteIdx >= 0) {
                    sprites.erase(sprites.begin() + selectedSpriteIdx);
                    selectedSpriteIdx = -1;
                }
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Export Scene")) { /* TODO */ }
                if (ImGui::MenuItem("Load Scene")) { /* TODO */ }
                ImGui::Separator();
                if (ImGui::MenuItem("Quit")) window.close();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Assets")) {
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

        window.clear(sf::Color(30, 30, 30));

        std::vector<int> order(sprites.size());
        std::iota(order.begin(), order.end(), 0);
        std::sort(order.begin(), order.end(), [&](int a, int b) {
            return sprites[a]->layer < sprites[b]->layer;
            });

        for (int idx : order) {
            window.draw(sprites[idx]->sprite);

            if (idx == selectedSpriteIdx) {
                auto bounds = sprites[idx]->sprite.getGlobalBounds();
                sf::RectangleShape outline({ bounds.size.x, bounds.size.y });
                outline.setPosition(bounds.position);
                outline.setFillColor(sf::Color::Transparent);
                outline.setOutlineColor(sf::Color(100, 200, 255));
                outline.setOutlineThickness(2.f);
                window.draw(outline);
            }
        }

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}