#pragma once
#ifndef PATHFIND_ASTAR_HPP_
#define PATHFIND_ASTAR_HPP_

#include <algorithm>
#include <memory>
#include <optional>
#include <tuple>
#include <vector>
#include "PathFind/Common.hpp"

namespace pf {
	struct Node;

	struct Node {
		std::tuple<std::int32_t, std::int32_t> parent, pos;
		std::int32_t g;
		float f;

		friend bool operator<(const Node &left, const Node &Right);
	};

	float calculate_distance(
		std::tuple<std::int32_t, std::int32_t> &beginPos,
		std::tuple<std::int32_t, std::int32_t> &endPos
	);

	std::vector<std::tuple<std::int32_t, std::int32_t>> get_neighbours(
		std::tuple<std::int32_t, std::int32_t> pos,
		std::size_t maxRows, std::size_t maxCols
	);

	template<std::size_t ROWS, std::size_t COLS>
	class AStar {
	private:
		Map<ROWS, COLS> map;
		std::tuple<std::int32_t, std::int32_t>
			start, end;
		
	public:
		std::vector<Node>
				closedSet, openSet;
		AStar(
			const Map<ROWS, COLS> &map,
			const std::tuple<std::int32_t, std::int32_t> &start,
			const std::tuple<std::int32_t, std::int32_t> &end
		) : map(map), start(start), end(end)
		{ }

		std::optional<Node> operator()() {
			openSet.push_back({
				Node{
					{-1, -1}, 
					start, 
					0, 
					calculate_distance(start, end)}
			});
			// std::map<
			// 	std::tuple<std::int32_t, std::int32_t>,
			// 	std::int32_t
			// > gScore;

			// gScore[start] = 0;

			while (!openSet.empty()) {
				
				auto x = *std::min_element(
					openSet.begin(),
					openSet.end(),
					[](const Node &lhs, const Node &rhs) { return lhs.f <= rhs.f; });

				if (x.pos == end) return x;

				std::remove_if(openSet.begin(), openSet.end(), [&x](const Node &n) { return n.pos == x.pos; });
				closedSet.push_back(x);
				
				for(auto &y : get_neighbours(x.pos, ROWS, COLS)) {

					if (
						std::find_if(
							closedSet.begin(), closedSet.end(),
							[&y](const Node &n) { return n.pos == y; }
						) != closedSet.end()
					) continue;

					auto g = x.g + 1;
					float h = calculate_distance(y, end);

					std::vector<Node>::iterator found = std::find_if(
						openSet.begin(), openSet.end(),
						[&y](const Node &n) { return n.pos == y; }
					);
					if (found == openSet.end()) {
						openSet.push_back({
							x.pos, y, g, g+calculate_distance(y, end)
						});
					}
					else if (g < found->g) {
						found->parent = x.pos;
						found->g = g;
						found->f = g + calculate_distance(y, end);
					}
				}
			}
			return std::nullopt;
		}
	};

	
}

#endif // ! PATHFIND_ASTAR_HPP_