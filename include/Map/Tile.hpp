#pragma once
#ifndef MAP_TILE_HPP_
#define MAP_TILE_HPP_

#include "Common.hpp"

enum class TileType {
	PATH, WALL
};
enum class TileState {
	DEFAULT = 0, OPEN_LIST = 1, CLOSED_LIST = 2,
	ROUTE = 3, PLAYER = 4 
};
struct Tile {
	TileType type;
	TileState state;
	float cost = 1.0f;

	vf4d tileColor() const;
	char tileSymbol() const;
};

extern const Tile WALL;
extern const Tile PATH;


#endif // ! MAP_TILE_HPP_