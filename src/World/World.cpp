#include "World/World.hpp"
#include "Common.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <mutex>
#include <optional>
#include <sstream>
#include <string>
#include <thread>
#include <vector>


World::World(
	u32 w,
	u32 h,
	IMap *m,
	std::initializer_list<Tile> ts
) : width(w), height(h), p_map(m)
{
	if (p_map == nullptr) {
		std::cerr << "Error: World::World(): Map is nullptr\n";
		throw "";
	}

	tiles.reserve(w * h);
	for (i32 i = 0; i < w*h; i++) {
		if (ts.begin() + i >= ts.end())
			tiles.push_back(PATH);
		else
			tiles.push_back(*(ts.begin() + i));
	}

	update_map();
}

World World::from_grid_file(
	u32 w,
	u32 h,
	IMap *m,
	const std::string &path	
) {
	World world = World(w, h, m, {});
	std::ifstream ifs(path);
	if (!ifs.good()) {
		std::cout << "Warning: file `" << path << "` could not be read.\n";
		return World(world);
	}
	std::string line;
	for (u32 j = 0; j < h && std::getline(ifs, line); j++) {
		std::istringstream iss(line);
		std::string digit;
		for (u32 k = 0; k < w && std::getline(iss, digit, ' '); k++) {
			if (digit == "0") world.tile(k, j).type = TileType::PATH;
			else if (digit == "5") world.tile(k, j).type = TileType::WALL;
		}
	}

	return World(world);
}

World::World(const World &world) {
	width = world.width;
	height = world.height;
	p_map = world.p_map;
	tiles = world.tiles;
	playerPos = world.playerPos.load();
}
World::World(World &&world)  {
	width = world.width;
	height = world.height;
	p_map = world.p_map;
	tiles = std::move(world.tiles);
	playerPos = world.playerPos.load();
}
World &World::operator=(World &&world) {
	width = world.width;
	height = world.height;
	p_map = world.p_map;
	tiles = std::move(world.tiles);
	playerPos = world.playerPos.load();
	return *this;
}


Tile &World::tile(u32 x, u32 y) {
	return tiles[height * y + x];
}

void World::update_map() {
	p_map->update_map(tiles);
}

vi2d World::get_player_pos() { return playerPos.load(); }
void World::set_player_pos(const vi2d &pos) {
	if (tile(V2D_EXPAND(pos)).type == TileType::WALL) {
		return;
	}
	{
		std::lock_guard lock(tiles_mutex);
		tile(V2D_EXPAND(playerPos.load())).state = TileState::DEFAULT;
		tile(V2D_EXPAND(pos)).state = TileState::PLAYER;
		playerPos = pos;
	}
	update_map();
}

void World::set_tile_state(const vi2d &pos, TileState state) {
	{
		std::lock_guard lock(tiles_mutex);
		tile(V2D_EXPAND(pos)).state = state;
	}
	update_map();
}

vi2d World::screen_to_world(vf2d screen) const {
	vi2d pos;
	screen.x /= Common::windowSize.x;
	screen.y /= Common::windowSize.y;
	screen.x *= width;
	screen.y *= height;
	pos.x = int(std::ceil(screen.x) - 1.0f);
	pos.y = int(std::ceil(screen.y) - 1.0f);
	pos.x = pos.x < 0 ? 0 : pos.x;
	pos.y = pos.y < 0 ? 0 : pos.y;
	return pos;
}

float calculate_distance(
	const vi2d &b,
	const vi2d &e
) {
	return std::sqrt(
		std::powf(b.x - e.x, 2.0f) + std::powf(b.y - e.y, 2.0f)
	);
}
std::vector<vi2d> World::get_neighbours(const vi2d &target) {
	std::vector<vi2d> ret;

	const auto x = target.x, y = target.y;
	if (y - 1 >= 0 && tile(x, y-1).type != TileType::WALL) {
		ret.push_back({x, y-1});
	}
	if (y + 1 < height && tile(x, y+1).type != TileType::WALL) {
		ret.push_back({x, y+1});
	}
	if (x + 1 < width && tile(x+1, y).type != TileType::WALL) {
		ret.push_back({x+1, y});
	}
	if (x - 1 >= 0 && tile(x-1, y).type != TileType::WALL) {
		ret.push_back({x-1, y});
	}

	return ret;
}

std::vector<vi2d> get_route(
	Node node,
	const std::vector<Node> closedList
) {
	std::vector<vi2d> ret;
	ret.push_back(node.pos);
	while (node.parent != std::nullopt) {
		ret.push_back(*node.parent);
		auto parent = std::find_if(closedList.begin(), closedList.end(), [&node](const Node &n) {
			return n.pos == node.parent;
		});
		node = *parent;
	}
	return ret;
}

std::optional<std::vector<vi2d>> World::find_path_to(vi2d target) {
	std::vector<Node> openList, closedList;
	set_tile_state(target, TileState::ROUTE);
	
	openList.push_back(Node{
		playerPos,
		std::nullopt,
		0,
		calculate_distance(playerPos, target)
	});

	while (!openList.empty()) {
		auto min = *std::min_element(
			openList.begin(),
			openList.end(),
			[](const Node &lhs, const Node &rhs) { return lhs.f <= rhs.f; }
		);
		if (min.pos == target) return get_route(min, closedList);
		
		for (auto it = openList.begin(); it != openList.end();) {
			if (it->pos == min.pos)
				it = openList.erase(it);
			else
				++it;
		}
		
		closedList.push_back(min);
		set_tile_state(min.pos, TileState::CLOSED_LIST);

		auto neighbours = get_neighbours(min.pos);
		for (auto &neighbour : neighbours) {
			if (
				std::find_if(
					closedList.begin(), closedList.end(),
					[&neighbour](const Node &n) { return n.pos == neighbour; }
				) != closedList.end()
			) continue;

			float g = min.g + 1;
			float h = calculate_distance(neighbour, target);

			std::vector<Node>::iterator found = std::find_if(
				openList.begin(), openList.end(),
				[&neighbour](const Node &n) { return n.pos == neighbour; }
			);

			if (found == openList.end()) {
				openList.push_back({
					neighbour, min.pos, g, g+h
				});
				set_tile_state(neighbour, TileState::OPEN_LIST);
			}
			else if (g < found->g) {
				found->parent = min.pos;
				found->g = g;
				found->f = g + h;
			}
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(Common::pathDelay);
		}
	}
	return std::nullopt;
}

void World::reset_tiles() {
	for (auto &tile : tiles) {
		tile.state = TileState::DEFAULT;
	}
	set_tile_state(playerPos, TileState::PLAYER);
}

void World::move_player_to(vi2d target) {
	if (tile(V2D_EXPAND(target)).type == TileType::WALL) return;
	auto path = find_path_to(target);

	if (path == std::nullopt) {
		std::cout << "No suitable path\n";
		return;
	}
	using namespace std::chrono_literals;
	for (const auto &t : *path) {
		set_tile_state(t, TileState::ROUTE);
	}
	while (path->size() > 0) {
		set_player_pos(path->back());
		path->pop_back();
		update_map();
		std::this_thread::sleep_for(300ms);
	}
}