#pragma once

constexpr unsigned TILE_SIZE = 32u;
constexpr unsigned BOARD_TILES = 9u;
constexpr unsigned BOARD_SIZE = BOARD_TILES * TILE_SIZE;

constexpr unsigned PADDING_TILES = 5u;
constexpr unsigned TOTAL_TILES = BOARD_TILES + PADDING_TILES * 2u;
constexpr unsigned VIRTUAL_WIDTH = TOTAL_TILES * TILE_SIZE;
constexpr unsigned VIRTUAL_HEIGHT = VIRTUAL_WIDTH;

constexpr unsigned BOARD_LEFT = PADDING_TILES * TILE_SIZE;
constexpr unsigned BOARD_TOP = PADDING_TILES * TILE_SIZE;
constexpr unsigned BOARD_RIGHT = BOARD_LEFT + BOARD_SIZE;
constexpr unsigned BOARD_BOTTOM = BOARD_TOP + BOARD_SIZE;