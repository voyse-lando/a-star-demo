// #include <algorithm>
// #include <future>
#include <future>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// #include "Common.hpp"
#include "Common.hpp"
#include "Engine.hpp"
#include "Keyboard.hpp"
#include "Map/Rectangle.hpp"
#include "Map/Tile.hpp"
#include "Mouse.hpp"
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

	bool moving, delayChanged, editMode;
	std::future<void> movementResult;

	std::optional<Rectangle> editModeBorder;

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

		editModeBorder = Rectangle()
			.set_border_color({1.0f, 0.0f, 0.0f, 1.0f})
			.set_fill_color({0.0f, 0.0f, 0.0f, 0.0f})
			.set_border_size(.005f)
			.set_scale({1.0f, 1.0f})
			.set_translate({0.0f, 0.0f});

		world->set_player_pos({config.playerX.value, config.playerY.value});
		moving = false;
		editMode = false;
		delayChanged = false;
		return true;
	}

	bool on_update() override {
		static bool resetConfigHeld = false;
		static bool editModeHeld = false;
		static bool mouseButtonHeld = false;

		glClearColor(.2f, .2f, .3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (Key::E.pressed && !editModeHeld) {
			editMode = !editMode;
			editModeHeld = true;
		}
		if (Key::E.released && editModeHeld) {
			editModeHeld = false;
		}

		if (!editMode && !moving && !mouseButtonHeld && MouseButton::Left.pressed)
		{
			mouseButtonHeld = true;
			vi2d tilePos = world->screen_to_world(Common::cursorPos);
			
			movementResult = std::async([this, &tilePos]() -> void {
				moving = true;
				world->move_player_to(tilePos);
				moving = false;

				world->reset_tiles();
			});
		}
		if (editMode && !moving && !mouseButtonHeld && MouseButton::Left.pressed)
		{
			mouseButtonHeld = true;
			vi2d tilePos = world->screen_to_world(Common::cursorPos);
			if (world->get_tile_state(tilePos) == TileState::DEFAULT) {
				TileType tt = world->get_tile_type(tilePos);
				world->set_tile_type(tilePos, tt == TileType::WALL ? TileType::PATH : TileType::WALL);
			}
		}
		if (MouseButton::Left.released && mouseButtonHeld) {
			mouseButtonHeld = false;
		}

		if (!delayChanged && Key::T.pressed) {
			delayChanged = true;
			if (Common::pathDelay == 0ms) Common::pathDelay = 500ms;
			else Common::pathDelay = 0ms;
		} else if (delayChanged && glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE) {
			delayChanged = false;
		}

		if (!resetConfigHeld && !moving && Key::R.pressed) {
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
		} else if (resetConfigHeld && Key::R.released) {
			resetConfigHeld = false;
		}


		map->draw();
		if (editMode) editModeBorder->draw();

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