#include "Map/Tile.hpp"
#include "Common.hpp"

extern const Tile WALL = {TileType::WALL, TileState::DEFAULT};
extern const Tile PATH = {TileType::PATH, TileState::DEFAULT};

vf4d Tile::tileColor() const {
	static vf4d colors[] = {
		vf4d{.2f, .2f, .2f, 1.0f},
		vf4d{.2f, .8f, .2f, 1.0f},
		vf4d{.8f, .2f, .2f, 1.0f},
		vf4d{.8f, .8f, .2f, 1.0f},
		vf4d{.2f, .8f, .8f, 1.0f}
	};
	switch (this->type) {
	case TileType::WALL:
		return vf4d{0.05f, 0.05f, 0.05f, 1.0f};
	case TileType::PATH:
		return colors[(u32)this->state];
	}
	return vf4d{.0f, .0f, .0f, 1.0f};
}
char Tile::tileSymbol() const {
	switch (this->type) {
	case TileType::WALL:
		return Common::symbolWall;
	case TileType::PATH:
		if (this->state == TileState::ROUTE)
			return Common::symbolRoute;
		else
			return Common::symbolPath;
	}
	return Common::symbolWall;
}