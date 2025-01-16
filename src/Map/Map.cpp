#include "Map/Map.hpp"
#include <iostream>
#include <mutex>
#include <sstream>

vf2d GLMap::step = {2.0f, -2.0f};
float GLMap::borderSize = 0.01f;
vf4d GLMap::fillColor = {.2f, .2f, .2f, 1.0f}, 
	 GLMap::borderColor = {.05f, .05f, .05f, 1.0f};

GLMap::GLMap(u32 w, u32 h) : IMap(w, h) {
	scale = {1.0f / (float)w, 1.0f / (float)h};
	topLeft = {1.0f - (float)w, -1.0f + (float)h};

	rects.reserve(w*h);

	for (u32 y = 0; y < h; y++) {
		for (u32 x = 0; x < w; x++) {
			vf2d translate = topLeft;
			translate.x += step.x * x;
			translate.y += step.y * y;

			rects.push_back(Rectangle()
				.set_translate(translate)
				.set_scale(scale)
				.set_fill_color(fillColor)
				.set_border_color(borderColor)
				.set_border_size(borderSize)
			);
		}
	}
}

GLMap::GLMap(const GLMap &map) : IMap(map.width, map.height) {
	scale = map.scale;
	topLeft = map.topLeft;
	rects = map.rects;
}

GLMap::GLMap(GLMap &&map) : IMap(map.width, map.height)  {
	scale = map.scale;
	topLeft = map.topLeft;
	rects = std::move(map.rects);
}

GLMap &GLMap::operator=(GLMap &&map) {
	width = map.width;
	height = map.height;
	scale = map.scale;
	topLeft = map.topLeft;
	rects = std::move(map.rects);
	return *this;
}

Rectangle &GLMap::rect(u32 x, u32 y) {
	return rects[height * y + x];
}

void GLMap::set_rect_fill(u32 x, u32 y, const vf4d &color) {
	std::lock_guard lock(rects_mutex);
	rect(x, y).set_fill_color(color);
}

void GLMap::update_map(const std::vector<Tile> &tiles) {
	auto it = tiles.begin();
	for (u32 y = 0; y < height; y++) {
		for (u32 x = 0; x < width; x++) {
			if (it == tiles.end()) return;
			set_rect_fill(x, y, (it++)->tileColor());
		}
	}
}

void GLMap::draw() const {
	std::lock_guard lock(rects_mutex);
	for (const auto &r : rects)
		r.draw();
}


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