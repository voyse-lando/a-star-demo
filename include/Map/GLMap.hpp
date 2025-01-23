#pragma once
#ifndef MAP_GLMAP_HPP_
#define MAP_GLMAP_HPP_

#include "Map/Map.hpp"
#include "Map/Rectangle.hpp"
#include <mutex>

class GLMap : public IMap {
private:
	static vf2d step; 
	static float borderSize;
	static vf4d fillColor, borderColor;

	vf2d scale, topLeft;
	mutable std::mutex rects_mutex;
	std::vector<Rectangle> rects;
private:
	Rectangle &rect(u32 x, u32 y);
	void set_rect_fill(u32 x, u32 y, const vf4d &color);
public:
	GLMap(u32 width, u32 height);
	explicit GLMap(const GLMap &map);
	explicit GLMap(GLMap &&map);
	GLMap &operator=(GLMap &&map);

	void update_map(const std::vector<Tile> &tiles) override;

	void draw() const override;
};

#endif // ! MAP_GLMAP_HPP_