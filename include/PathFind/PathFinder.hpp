#pragma once
#include <vector>
#ifndef PATHFIND_PATHFINDER_HPP_
#define PATHFIND_PATHFINDER_HPP_

#include "PathFind/Map.hpp"

namespace pf {

	float calculate_distance(
		const vi2d &beginPos,
		const vi2d &endPos
	);

	std::vector<vi2d> get_neighbours(
		const vi2d &pos,
		const Map &map
	);

	struct Node {
		std::optional<vi2d> parent;
		vi2d pos;

		float g, f;
	};

	class PathFinder {
	private:
		std::optional<Map> map;
		std::vector<Node> openList, closedList;

		vi2d begin, end;
	public:

		PathFinder(const Map &_map, const vi2d &begin, const vi2d &end);	

		std::optional<Node> operator()();

		inline Map &get_map() { return *map; }
		inline const std::vector<Node> &get_closed_list() const { return closedList; }
	};

};

#endif // ! PATHFIND_PATHFINDER_HPP_