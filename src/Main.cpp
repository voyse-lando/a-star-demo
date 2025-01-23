// #include <algorithm>
// #include <future>
#include <future>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// #include "Common.hpp"
#include "Common.hpp"
#include "Engine.hpp"
#include "Map/Map.hpp"
#include "Map/GLMap.hpp"
#include "World/World.hpp"
#include "Config/Config.hpp"
// #include "PathFind/Map.hpp"
// #include "PathFind/PathFinder.hpp"

#include <iostream>
#include <memory>
#include <optional>
// #include <vector>
// #include <thread>

#define P PATH,
#define W WALL,

using namespace std::chrono_literals;

class AStarDemo : public Engine {
protected:
	std::shared_ptr<GLMap> map;
	std::optional<World> world;

	bool moving, delayChanged;
	std::future<void> movementResult;

public:
	Config &config;

	AStarDemo(Config &config) : Engine(), config(config) {
		appName = "A* DEMO";
	}

	bool on_create() override {
		map = std::make_shared<GLMap>(GLMap(
			config.mapWidth.value,
			config.mapHeight.value
		));
		world = World::from_grid_file(
			config.mapWidth.value,
			config.mapHeight.value,
			map.get(),
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
				map.get(),
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

void console() {
	Config config = Config::from_toml("./config.toml");
	Common::from_config(config);
	std::shared_ptr<ConsoleMap> map = std::make_shared<ConsoleMap>(ConsoleMap(
		config.mapWidth.value,
		config.mapHeight.value
	));
	World world = World::from_grid_file(
		config.mapWidth.value,
		config.mapHeight.value,
		map.get(),
		config.mapDefinitionsFilePath.value
	);

	world.set_player_pos({config.playerX.value, config.playerY.value});
	auto route = world.find_path_to({19, 0});
	if (route == std::nullopt) {
		map->draw();
		std::cout << "\n" "No suitable path" << std::endl;
		return;
	}
	for (auto &tile : *route) {
		world.set_tile_state(tile, TileState::ROUTE);
	}
	map->draw();
}

int main(int argc, const char **argv) {
	std::cout << "A* Demo ver 1.1" << std::endl;
	bool runConsole = false;

	for (u32 i = 1; i < argc; i++) {
		if (std::string(argv[i]) == "--console")
			runConsole = true;
	}

	if (runConsole) {
		console();
		return 0;
	}

	Config config = Config::from_toml("./config.toml");
	Common::windowSize = {config.screenWidth.value, config.screenHeight.value};
	
  	AStarDemo demo(config);

	if (demo.construct(V2D_EXPAND(Common::windowSize)))
		demo.start();

  	return 0;
}