// #include "Engine.hpp"
#include "PathFind/A-Star.hpp"
#include "PathFind/Common.hpp"

#include <algorithm>
#include <iostream>
#include <optional>

int main()
{
	std::cout << "A* Demo ver 0.1" << std::endl;

	const auto
		P = pf::Tile::PATH,
		W= pf::Tile::WALL;
	pf::Map<5, 5> map {{{
		{ P, P, P, P, P},
		{ P, P, P, P, P},
		{ P, P, P, P, P},
		{ P, P, P, P, P},
		{ P, P, P, P, P},
	}}};

	pf::AStar<5, 5> pathFinder(map, {4, 0}, {0, 4});

	auto x = pathFinder();
	if (x == std::nullopt) {
		std::cerr << "No suitable path" << std::endl;
		return 0;
	}
	const auto nulltuple = std::tuple{-1, -1};
	auto node = *x;
	while (node.parent != nulltuple) {
		auto [y,x] = node.pos;
		std::cout << "(" << y << ", " << x << ")\n";

		node = *std::find_if(
			pathFinder.closedSet.begin(), 
			pathFinder.closedSet.end(),
			[&node](const pf::Node &n) { return n.pos == node.parent; }
		);

	}

  // Engine engine;

  // if (engine.construct(800, 600))
  //   engine.start();

	

  return 0;
}
