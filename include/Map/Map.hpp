#pragma once
#ifndef MAP_MAP_HPP_
#define MAP_MAP_HPP_

#include "Common.hpp"
#include "Map/Rectangle.hpp"
#include <mutex>
#include <vector>

class Map {
public:
	u32 width, height;
private:
	static vf2d step; 
	static float borderSize;
	static vf4d fillColor, borderColor;

	vf2d scale, topLeft;
	mutable std::mutex rects_mutex;
	std::vector<Rectangle> rects;
private:
	Rectangle &rect(u32 x, u32 y);
public:
	Map(u32 width, u32 height);
	explicit Map(const Map &map);
	explicit Map(Map &&map);
	Map &operator=(Map &&map);

	void set_rect_fill(u32 x, u32 y, const vf4d &color);

	void draw() const;
};

#endif // ! MAP_MAP_HPP_