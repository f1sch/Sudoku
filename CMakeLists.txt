cmake_minimum_required(VERSION 3.28)
project(Sudoku LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SFML COMPONENTS Graphics Window Audio System CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# shared data
file(GLOB_RECURSE SHARED_SOURCES CONFIGURE_DEPENDS shared/*.cpp)

add_library(sudoku_shared INTERFACE) #${SHARED_SOURCES}) # Make INTERFACE->PUBLIC if (source files)

# Add source files
target_sources(sudoku_shared
    PRIVATE
        ${SHARED_SOURCES}
)

target_include_directories(sudoku_shared
    INTERFACE
        ${CMAKE_SOURCE_DIR}
)
target_compile_features(sudoku_shared INTERFACE cxx_std_17)

# Sudoku Game
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(Sudoku ${GAME_SOURCES})
target_link_libraries(Sudoku 
    PRIVATE
    sudoku_shared
    SFML::Graphics
    ImGui-SFML::ImGui-SFML
    nlohmann_json::nlohmann_json
)

# ImGui Editor
file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS editor/*.cpp)

add_executable(SudokuEditor ${EDITOR_SOURCES})
target_link_libraries(SudokuEditor
    PRIVATE
    sudoku_shared
    SFML::Graphics
    ImGui-SFML::ImGui-SFML
    nlohmann_json::nlohmann_json
)

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