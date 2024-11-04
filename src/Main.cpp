#include <algorithm>
#include <future>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Common.hpp"
#include "Engine.hpp"
#include "PathFind/Map.hpp"
#include "PathFind/PathFinder.hpp"

#include <iostream>
#include <optional>
#include <thread>

#define P pf::Tile::PATH,
#define W pf::Tile::WALL,

using namespace std::chrono_literals;

class AStarDemo : public Engine {
protected:
	std::optional<pf::PathFinder> find = std::nullopt;
	std::future<void> result;

	void find_path() {
		auto x = (*find)();
		if (x == std::nullopt)
			std::cerr << "No suitable path";

		auto b = find->get_closed_list().begin();
		auto e = find->get_closed_list().end();
		std::optional<pf::Node> y;
		for (
			y = x;
			y->parent != std::nullopt;
			y = *std::find_if(b, e, [&y](const pf::Node &node) { return y->parent == node.pos; })
		) {
			find->get_map().set_tile(VI2D_EXPAND(y->pos), pf::Tile::PATH_FOUND);
			std::this_thread::sleep_for(200ms);
		}
		find->get_map().set_tile(VI2D_EXPAND(y->pos), pf::Tile::PATH_FOUND);
	}
public:
	AStarDemo() : Engine() {}

	bool on_create() override {
		find = {pf::Map(9, 9, 
		{
			P P P P P W P P P
			P W P P P P P W W
			P P W P P W P P P
			P P P W P W W W P
			P P P P P P P P P
			P W P P P W W P W
			W P P P W P P P P
			P P P W P P P W P
			P P P P W P W P P
		}), {0, 8}, {8, 0}};

		result = std::async(&AStarDemo::find_path, this);
		return true;
	}

	bool on_update() override {
    glClearColor(.2f, .2f, .3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

		find->get_map().draw();

		return true;
	}
};

int main()
{
	std::cout << "A* Demo ver 0.1" << std::endl;

	

  AStarDemo demo;

  if (demo.construct(600, 600))
    demo.start();

	

  return 0;
}


// const auto
	// 	P = pf::Tile::PATH,
	// 	W= pf::Tile::WALL;
	// pf::Map<5, 5> map {{{
	// 	{ P, P, P, P, P},
	// 	{ P, P, P, P, P},
	// 	{ P, P, P, P, P},
	// 	{ P, P, P, P, P},
	// 	{ P, P, P, P, P},
	// }}};

	// pf::AStar<5, 5> pathFinder(map, {4, 0}, {0, 4});

	// auto x = pathFinder();
	// if (x == std::nullopt) {
	// 	std::cerr << "No suitable path" << std::endl;
	// 	return 0;
	// }
	// const auto nulltuple = std::tuple{-1, -1};
	// auto node = *x;
	// while (node.parent != nulltuple) {
	// 	auto [y,x] = node.pos;
	// 	std::cout << "(" << y << ", " << x << ")\n";

	// 	node = *std::find_if(
	// 		pathFinder.closedSet.begin(), 
	// 		pathFinder.closedSet.end(),
	// 		[&node](const pf::Node &n) { return n.pos == node.parent; }
	// 	);

	// }