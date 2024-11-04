#include "PathFind/PathFinder.hpp"
#include "Common.hpp"
#include "PathFind/Map.hpp"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <optional>
#include <thread>

using namespace std::chrono_literals;

float pf::calculate_distance(
	const vi2d &beginPos,
	const vi2d &endPos
) {
	auto &[bX, bY] = beginPos;
	auto &[eX, eY] = endPos;
	return std::sqrt(
		std::powf(bX - eX, 2) + std::powf(bY - eY, 2)
	);
}
std::vector<vi2d> pf::get_neighbours(
	const vi2d &pos,
	const pf::Map &map
) {
	std::vector<vi2d> ret;

	const auto &[x, y] = pos;

	if (y + 1 < map.get_height()  && map.get_tile(x, y+1) != pf::Tile::WALL)
	{
		ret.push_back({x, y+1});
	}
	if (y - 1 >= 0 && map.get_tile(x, y-1) != pf::Tile::WALL)
	{
		ret.push_back({x, y-1});
	}
	if (x + 1 < map.get_width()  && map.get_tile(x+1, y) != pf::Tile::WALL)
	{
		ret.push_back({x+1, y});
	}
	if (x - 1 >= 0  && map.get_tile(x-1, y) != pf::Tile::WALL)
	{
		ret.push_back({x-1, y});
	}

	return ret;
}

pf::PathFinder::PathFinder(const Map &_map, const vi2d &begin, const vi2d &end)
: map(_map), begin(begin), end(end)
{ }

std::optional<pf::Node> pf::PathFinder::operator()() {
	openList.push_back(Node{
		std::nullopt,
		begin,
		0,
		calculate_distance(begin, end)
	});

	map->set_tile(VI2D_EXPAND(begin), Tile::PATH_OPEN_LIST);
	map->set_tile(VI2D_EXPAND(end), Tile::PATH_FOUND);

	while (!openList.empty()) {

		auto x = *std::min_element(
			openList.begin(),
			openList.end(),
			[](const Node &lhs, const Node &rhs) { return lhs.f <= rhs.f; }
		);

		if (x.pos == end) return x;

		auto res = std::remove_if(openList.begin(), openList.end(), [&x](const Node &n) { return n.pos == x.pos; });
		closedList.push_back(x);

		map->set_tile(VI2D_EXPAND(x.pos), Tile::PATH_CLOSED_LIST);

		// std::this_thread::sleep_for(500ms);
		for (auto &y : get_neighbours(x.pos, *map)) {

			if (
				std::find_if(
					closedList.begin(), closedList.end(),
					[&y](const Node &n) { return n.pos == y; }
				) != closedList.end()
			) continue;

			auto g = x.g + 1;
			float h = calculate_distance(y, end);

			std::vector<Node>::iterator found = std::find_if(
				openList.begin(), openList.end(),
				[&y](const Node &n) { return n.pos == y; }
			);
			if (found == openList.end()) {
				openList.push_back({
					x.pos, y, g, g+calculate_distance(y, end)
				});
				map->set_tile(VI2D_EXPAND(y), Tile::PATH_OPEN_LIST);
			}
			else if (g < found->g) {
				found->parent = x.pos;
				found->g = g;
				found->f = g + calculate_distance(y, end);
			}

			std::this_thread::sleep_for(500ms);
		}
	}
	return std::nullopt;
}