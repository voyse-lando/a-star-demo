#include "World/World.hpp"
#include "Common.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>

extern const Tile WALL = {TileType::WALL, TileState::DEFAULT};
extern const Tile PATH = {TileType::PATH, TileState::DEFAULT};

vf4d World::tileColor(const Tile &tile) {
	static vf4d colors[] = {
		vf4d{.2f, .2f, .2f, 1.0f},
		vf4d{.2f, .8f, .2f, 1.0f},
		vf4d{.8f, .2f, .2f, 1.0f},
		vf4d{.8f, .8f, .2f, 1.0f},
		vf4d{.2f, .8f, .8f, 1.0f}
	};
	switch (tile.type) {
	case TileType::WALL:
		return vf4d{0.05f, 0.05f, 0.05f, 1.0f};
	case TileType::PATH:
		return colors[(u32)tile.state];
	}
	return vf4d{.0f, .0f, .0f, 1.0f};
}

World::World(
	u32 w,
	u32 h,
	std::optional<Map> *m,
	std::initializer_list<Tile> ts
) : width(w), height(h), p_map(m)
{
	map() = Map(w, h);

	tiles.reserve(w * h);
	for (i32 i = 0; i < w*h; i++) {
		tiles.push_back(*(ts.begin() + i));
	}

	update_map();
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

std::optional<Map> &World::map() {
	return *p_map;
}

Tile &World::tile(u32 x, u32 y) {
	return tiles[height * y + x];
}

void World::update_map() {
	for (u32 y = 0; y < height; y++) {
		for (u32 x = 0; x < width; x++) {
			map()->set_rect_fill(x, y, World::tileColor(tile(x, y)));
		}
	}
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
		std::powf(b.x - e.x, 2) + std::powf(b.y - e.y, 2)
	);
}
std::vector<vi2d> World::get_neighbours(const vi2d &target) {
	std::vector<vi2d> ret;

	const auto x = target.x, y = target.y;
	if (y + 1 < height && tile(x, y+1).type != TileType::WALL) {
		std::cout << "UP " << x << ' ' << y+1 << '\n';
		ret.push_back({x, y+1});
	}
	if (y - 1 >= 0 && tile(x, y-1).type != TileType::WALL) {
		std::cout << "DOWN " << x << ' ' << y-1 << '\n';
		ret.push_back({x, y-1});
	}
	if (x + 1 < width && tile(x+1, y).type != TileType::WALL) {
		std::cout << "RIGHT " << x+1 << ' ' << y << '\n';
		ret.push_back({x+1, y});
	}
	if (x - 1 >= 0 && tile(x-1, y).type != TileType::WALL) {
		std::cout << "LEFT " << x-1 << ' ' << y << '\n';
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
		auto x = *std::min_element(
			openList.begin(),
			openList.end(),
			[](const Node &lhs, const Node &rhs) { return lhs.f <= rhs.f; }
		);
		if (x.pos == target) return get_route(x, closedList);
		
		std::cout << "Before:\n";
		for (auto &z : openList) {
			std::cout << z.pos.x << ' ' << z.pos.y << '\n';
		}

		std::cout << "After:\n";
		for (auto it = openList.begin(); it != openList.end();) {
			if (it->pos == x.pos)
				it = openList.erase(it);
			else
				++it;
		}
		
		closedList.push_back(x);
		set_tile_state(x.pos, TileState::CLOSED_LIST);

		auto neighbours = get_neighbours(x.pos);
		for (auto &neighbour : neighbours) {
			if (
				std::find_if(
					closedList.begin(), closedList.end(),
					[&neighbour](const Node &n) { return n.pos == neighbour; }
				) != closedList.end()
			) continue;

			float g = x.g + 1;
			float h = calculate_distance(neighbour, target);

			std::vector<Node>::iterator found = std::find_if(
				openList.begin(), openList.end(),
				[&neighbour](const Node &n) { return n.pos == neighbour; }
			);

			if (found == openList.end()) {
				openList.push_back({
					neighbour, x.pos, g, g+h
				});
				set_tile_state(neighbour, TileState::OPEN_LIST);
			}
			else if (g < found->g) {
				found->parent = x.pos;
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