#pragma once
#include <deque>
#ifndef WORLD_WORLD_HPP_
#define WORLD_WORLD_HPP_
#include "Common.hpp"
#include "Map/Map.hpp"
#include <initializer_list>
#include <optional>
#include <queue>
#include <vector>

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
};

extern const Tile WALL;
extern const Tile PATH;

struct Node {
	vi2d pos;
	std::optional<vi2d> parent;
	float g, f;
};

class World {
public:
	u32 width, height;

	static vf4d tileColor(const Tile &tile);
private:
	std::optional<Map> *p_map;

	std::mutex tiles_mutex;
	std::vector<Tile> tiles;

	std::atomic<vi2d> playerPos = vi2d{0, 0};
private:
	std::optional<Map> &map();
	Tile &tile(u32 x, u32 y);

	void update_map();

	std::vector<vi2d> get_neighbours(const vi2d &target);
public:
	World(
		u32 width,
		u32 height,
		std::optional<Map> *map,
		std::initializer_list<Tile> tiles
	);

	explicit World(const World &world);
	explicit World(World &&world);
	World &operator=(World &&world);
	std::optional<std::vector<vi2d>> find_path_to(vi2d target);

	vi2d get_player_pos();
	void set_player_pos(const vi2d &pos);
	void move_player_to(vi2d target);
	
	// TileState get_tile_state();
	void set_tile_state(const vi2d &pos, TileState state);
	void reset_tiles();

	vi2d screen_to_world(vf2d screen) const;

};

#endif // ! WORLD_WORLD_HPP_