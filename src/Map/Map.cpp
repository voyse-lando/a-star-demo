#include "Map/Map.hpp"
#include <iostream>
#include <sstream>

void ConsoleMap::update_map(const std::vector<Tile> &tiles) {
	this->tiles = tiles;
}

void ConsoleMap::draw() const {
	std::ostringstream buf;

	auto it = tiles.begin();
	for (u32 y = 0; y < height; y++) {
		for (u32 x = 0; x < width; x++) {
			if (it == tiles.end()) break;
			buf << (it++)->tileSymbol() << ' ';
		}
		buf << '\n';
		if (it == tiles.end()) break;
	}

	std::cout << buf.str();
}