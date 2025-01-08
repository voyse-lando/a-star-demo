#include "Map/Map.hpp"
#include <mutex>

vf2d Map::step = {2.0f, -2.0f};
float Map::borderSize = 0.01f;
vf4d Map::fillColor = {.2f, .2f, .2f, 1.0f}, 
	 Map::borderColor = {.05f, .05f, .05f, 1.0f};

Map::Map(u32 w, u32 h) : width(w), height(h) {
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

Map::Map(const Map &map) {
	width = map.width;
	height = map.height;
	scale = map.scale;
	topLeft = map.topLeft;
	rects = map.rects;
}

Map::Map(Map &&map) {
	width = map.width;
	height = map.height;
	scale = map.scale;
	topLeft = map.topLeft;
	rects = std::move(map.rects);
}

Map &Map::operator=(Map &&map) {
	width = map.width;
	height = map.height;
	scale = map.scale;
	topLeft = map.topLeft;
	rects = std::move(map.rects);
	return *this;
}

Rectangle &Map::rect(u32 x, u32 y) {
	return rects[height * y + x];
}

void Map::set_rect_fill(u32 x, u32 y, const vf4d &color) {
	std::lock_guard lock(rects_mutex);
	rect(x, y).set_fill_color(color);
}

void Map::draw() const {
	std::lock_guard lock(rects_mutex);
	for (const auto &r : rects)
		r.draw();
}