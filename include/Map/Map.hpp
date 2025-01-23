#pragma once
#ifndef MAP_MAP_HPP_
#define MAP_MAP_HPP_

#include "Common.hpp"
#include "Map/Tile.hpp"
#include <vector>

class IMap {
public:
	u32 width, height;
	IMap(u32 w, u32 h) : width(w), height(h) {}

	virtual void update_map(const std::vector<Tile> &tiles) = 0;
	virtual void draw() const = 0;
};

class ConsoleMap : public IMap {
	std::vector<Tile> tiles;
public:
	ConsoleMap(u32 width, u32 height) : IMap(width, height) { }

	void update_map(const std::vector<Tile> &tiles) override;

	void draw() const override;
};

#endif // ! MAP_MAP_HPP_