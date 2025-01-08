// #include <algorithm>
// #include <future>
#include <future>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// #include "Common.hpp"
#include "Common.hpp"
#include "Engine.hpp"
#include "Map/Map.hpp"
#include "World/World.hpp"
// #include "PathFind/Map.hpp"
// #include "PathFind/PathFinder.hpp"

#include <iostream>
#include <optional>
// #include <vector>
// #include <thread>

#define P PATH,
#define W WALL,

using namespace std::chrono_literals;

class AStarDemo : public Engine {
protected:
	std::optional<Map> map;
	std::optional<World> world;

	bool moving, delayChanged;
	std::future<void> movementResult;
	// std::optional<pf::PathFinder> find = std::nullopt;
	// std::future<void> result;

// 	void find_path() {
// 		auto x = (*find)();
// 		if (x == std::nullopt)
// 			std::cerr << "No suitable path";

// 		auto b = find->get_closed_list().begin();
// 		auto e = find->get_closed_list().end();
// 		std::optional<pf::Node> y;
// 		for (
// 			y = x;
// 			y->parent != std::nullopt;
// 			y = *std::find_if(b, e, [&y](const pf::Node &node) { return y->parent == node.pos; })
// 		) {
// 			find->get_map().set_tile(VI2D_EXPAND(y->pos), pf::Tile::PATH_FOUND);
// 			std::this_thread::sleep_for(200ms);
// 		}
// 		find->get_map().set_tile(VI2D_EXPAND(y->pos), pf::Tile::PATH_FOUND);
// 	}
public:
	AStarDemo() : Engine() {
		appName = "A* DEMO";
	}

	bool on_create() override {
		// find = {pf::Map(9, 9, 
		world = World(9, 9, &map,
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
		});
		world->set_player_pos({8, 8});
		moving = false;
		delayChanged = false;
		// }), {0, 8}, {8, 0}};

		// result = std::async(&AStarDemo::find_path, this);
		// map = Map(10, 10);
		// map->set_rect_fill(3, 7, {1.0f, 0.0f, 0.0f, 1.0f});
		return true;
	}

	bool on_update() override {
		glClearColor(.2f, .2f, .3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if (!moving && state == GLFW_PRESS)
		{
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			vi2d tilePos = world->screen_to_world(vf2d{(float)xpos, (float)ypos});
			
			movementResult = std::async([this, &tilePos]() -> void {
				moving = true;
				world->move_player_to(tilePos);
				moving = false;

				world->reset_tiles();
			});
		}

		if (!delayChanged && glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
			delayChanged = true;
			if (Common::pathDelay == 0ms) Common::pathDelay = 500ms;
			else Common::pathDelay = 0ms;
		} else if (delayChanged && glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE) {
			delayChanged = false;
		}
		map->draw();

		return true;
	}

	void before_exit() override {
		glClearColor(.2f, .2f, .3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
};

int main()
{
	std::cout << "A* Demo ver 1.0" << std::endl;

	Common::windowSize = {800, 800};
	
  	AStarDemo demo;

	if (demo.construct(V2D_EXPAND(Common::windowSize)))
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