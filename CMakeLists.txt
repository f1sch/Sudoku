# vcpkg-Toolchain deaktivieren falls sie injiziert wird
if(DEFINED CMAKE_TOOLCHAIN_FILE AND CMAKE_TOOLCHAIN_FILE MATCHES "vcpkg")
    message(STATUS "vcpkg toolchain detected and disabled")
    unset(CMAKE_TOOLCHAIN_FILE CACHE)
endif()

cmake_minimum_required(VERSION 3.28)
project(Sudoku LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

# ── Declarations ─────────────────────────────────────────────────────────────
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.2
    GIT_SHALLOW    ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.91.5
)
FetchContent_Declare(
    imgui-sfml
    GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml.git
    GIT_TAG        master #2.6.x
)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
    GIT_SHALLOW    ON
)

# ── MakeAvailable ─────────────────────────────────────────────────────────────
option(SFML_BUILD_AUDIO   "Build audio"   OFF)
option(SFML_BUILD_NETWORK "Build network" OFF)
FetchContent_MakeAvailable(SFML)

FetchContent_MakeAvailable(imgui)
set(IMGUI_DIR ${imgui_SOURCE_DIR})

option(IMGUI_SFML_FIND_SFML "Use find_package to find SFML" OFF)
FetchContent_MakeAvailable(imgui-sfml)

FetchContent_MakeAvailable(nlohmann_json)

# ── Shared ───────────────────────────────────────────────────────────────────
file(GLOB_RECURSE SHARED_SOURCES CONFIGURE_DEPENDS shared/*.cpp)

add_library(sudoku_shared INTERFACE)
target_sources(sudoku_shared PRIVATE ${SHARED_SOURCES})
target_include_directories(sudoku_shared INTERFACE ${CMAKE_SOURCE_DIR})
target_compile_features(sudoku_shared INTERFACE cxx_std_17)

# ── Sudoku Game ───────────────────────────────────────────────────────────────
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(Sudoku ${GAME_SOURCES})
target_compile_features(Sudoku PRIVATE cxx_std_17)
target_link_libraries(Sudoku PRIVATE
    sudoku_shared
    SFML::Graphics
    ImGui-SFML::ImGui-SFML
    nlohmann_json::nlohmann_json
)

# ── Sudoku Editor ─────────────────────────────────────────────────────────────
file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS editor/*.cpp)

add_executable(SudokuEditor ${EDITOR_SOURCES})
target_compile_features(SudokuEditor PRIVATE cxx_std_17)
target_link_libraries(SudokuEditor PRIVATE
    sudoku_shared
    SFML::Graphics
    ImGui-SFML::ImGui-SFML
    nlohmann_json::nlohmann_json
)

# ── Assets ────────────────────────────────────────────────────────────────────
function(copy_assets TARGET_NAME)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:${TARGET_NAME}>/assets
    )
endfunction()

copy_assets(Sudoku)
copy_assets(SudokuEditor)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sudoku)