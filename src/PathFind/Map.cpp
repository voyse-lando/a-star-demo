#include "PathFind/Map.hpp"
#include <stdexcept>


std::unordered_map<pf::Tile, vf4d> pf::Map::tileColors = {
	{pf::Tile::PATH, {.4f, .4f, .4f, 1.0f}},
	{pf::Tile::WALL, {.1f, .1f, .1f, 1.0f}},
	{pf::Tile::PATH_OPEN_LIST, {.4f, .6f, .4f, 1.0f}},
	{pf::Tile::PATH_CLOSED_LIST, {.6f, .4f, .4f, 1.0f}},
	{pf::Tile::PATH_FOUND, {.6f, .6f, .4f, 1.0f}}
};

vf4d pf::Map::borderColor = {.0f, .0f, .0f, 1.0f};

pf::Map::Map(
	std::uint32_t width,
	std::uint32_t height,
	std::initializer_list<Tile> txs
) : width(width), height(height) {

	float scaleX = 1.0f/width;
	float scaleY = 1.0f/height;

	float translationX = 2.0f / (width - 1);
	float translationY = 2.0f / (height - 1);

	for (auto &t : txs) {
		this->tiles.push_back(t);
	}
	tiles.resize(width*height, Tile::WALL);

	for (auto i = 0; i < width; i++) {
		for (auto j = 0; j < height; j++) {
			this->rectangles.push_back(
				Rectangle()
					.set_scale(scaleX)
					.set_border_width(.01f)
					.set_color(tileColors[this->tiles[to_1d_pos(i, j)]])
					.set_border_color(borderColor)
					.set_translation({-1.0f + translationX * i, 1.0f - translationY * j})
			);
		}
	}
}

void pf::Map::set_tile(std::uint32_t x, std::uint32_t y, pf::Tile tile) {
	if (x > width || y > height) throw std::out_of_range("pf::PathMap::set_tile: index out of range");

	tiles[to_1d_pos(x, y)] = tile;
	rectangles[to_1d_pos(x, y)].set_color(tileColors[tile]);
}

void pf::Map::draw() const {
	for (const auto &rect : rectangles) rect.draw();
}
