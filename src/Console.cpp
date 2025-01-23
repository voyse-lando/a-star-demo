#include "Config/Config.hpp"
#include "World/World.hpp"
#include <iostream>

int main() {
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
		return 0;
	}
	for (auto &tile : *route) {
		world.set_tile_state(tile, TileState::ROUTE);
	}
	map->draw();

	return 0;
}