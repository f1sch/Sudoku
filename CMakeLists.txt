cmake_minimum_required(VERSION 3.28)
project(Sudoku LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

# SFML
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)

# Dear ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.1
)
FetchContent_MakeAvailable(imgui)

set(IMGUI_DIR ${imgui_SOURCE_DIR})

# ImGui-SFML
FetchContent_Declare(
    ImGui-SFML
    GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml.git
    GIT_TAG master
)
FetchContent_MakeAvailable(ImGui-SFML)

# nlohmann/json (editor data export)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(nlohmann_json)

# Sudoku Game
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(Sudoku ${GAME_SOURCES})
target_compile_features(Sudoku PRIVATE cxx_std_17)
target_link_libraries(Sudoku 
    PRIVATE 
    SFML::Graphics
    ImGui-SFML::ImGui-SFML
    nlohmann_json::nlohmann_json
)

add_custom_command(TARGET Sudoku POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:Sudoku>/assets
)

# ImGui Editor
file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS editor/*.cpp)

add_executable(SudokuEditor ${EDITOR_SOURCES})
target_compile_features(SudokuEditor PRIVATE cxx_std_17)
target_link_libraries(SudokuEditor
    PRIVATE
    SFML::Graphics
    ImGui-SFML::ImGui-SFML
    nlohmann_json::nlohmann_json
)

# make assets available for editor
add_custom_command(TARGET SudokuEditor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:SudokuEditor>/assets
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sudoku)