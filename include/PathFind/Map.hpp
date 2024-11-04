#pragma once
#ifndef PATHFIND_MAP_HPP_
#define PATHFIND_MAP_HPP_

#include "Common.hpp"
#include "Rectangle.hpp"
#include <initializer_list>
#include <unordered_map>
#include <vector>

namespace pf {

	enum class Tile {
		PATH = 0, WALL = 1, PATH_OPEN_LIST, PATH_CLOSED_LIST, PATH_FOUND
	};
	
	class Map {
	public:
		static std::unordered_map<Tile, vf4d> tileColors;
		static vf4d borderColor;
	private:
		std::uint32_t width, height;
		std::vector<Tile> tiles;
		std::vector<Rectangle> rectangles;

		inline std::uint32_t to_1d_pos(std::uint32_t x, std::uint32_t y) const {
			return x * width + y;
		}

	public:
		Map(
			std::uint32_t width,
			std::uint32_t height,
			std::initializer_list<Tile> tiles
		);

		inline Tile get_tile(std::uint32_t x, std::uint32_t y) const { return tiles[to_1d_pos(x, y)]; }
		void set_tile(std::uint32_t x, std::uint32_t y, Tile tile);

		inline std::uint32_t get_width() const { return width; }
		inline std::uint32_t get_height() const { return height; }

		void draw() const;
	};

};

#endif // ! PATHFIND_MAP_HPP_