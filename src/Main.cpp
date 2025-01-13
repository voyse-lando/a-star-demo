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
#include "Config/Config.hpp"
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

public:
	Config &config;

	AStarDemo(Config &config) : Engine(), config(config) {
		appName = "A* DEMO";
	}

	bool on_create() override {
		// find = {pf::Map(9, 9, 
		world = World::from_grid_file(
			config.mapWidth.value,
			config.mapHeight.value,
			&map,
			config.mapDefinitionsFilePath.value
		);
		world->set_player_pos({config.playerX.value, config.playerY.value});
		moving = false;
		delayChanged = false;
		return true;
	}

	bool on_update() override {
		static bool resetConfigHeld = false;

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

		if (!resetConfigHeld && !moving && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			resetConfigHeld = true;
			config = Config::from_toml("./config.toml");
			set_window_size({config.screenWidth.value, config.screenHeight.value});
			world = World::from_grid_file(
				config.mapWidth.value,
				config.mapHeight.value,
				&map,
				config.mapDefinitionsFilePath.value
			);
			world->set_player_pos({config.playerX.value, config.playerY.value});
		} else if (resetConfigHeld && glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
			resetConfigHeld = false;
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

	Config config = Config::from_toml("./config.toml");
	Common::windowSize = {config.screenWidth.value, config.screenHeight.value};
	
  	AStarDemo demo(config);

	if (demo.construct(V2D_EXPAND(Common::windowSize)))
		demo.start();

  	return 0;
}