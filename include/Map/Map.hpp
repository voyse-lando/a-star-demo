#pragma once
#ifndef MAP_MAP_HPP_
#define MAP_MAP_HPP_

#include "Common.hpp"
#include "Map/Rectangle.hpp"
#include "Map/Tile.hpp"
#include <mutex>
#include <vector>

class IMap {
public:
	u32 width, height;
	IMap(u32 w, u32 h) : width(w), height(h) {}

	virtual void update_map(const std::vector<Tile> &tiles) = 0;
	virtual void draw() const = 0;
};

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

class ConsoleMap : public IMap {
	std::vector<Tile> tiles;
public:
	ConsoleMap(u32 width, u32 height) : IMap(width, height) { }

	void update_map(const std::vector<Tile> &tiles) override;

	void draw() const override;
};

#endif // ! MAP_MAP_HPP_