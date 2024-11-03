#pragma once
#ifndef PATHFIND_COMMON_HPP_
#define PATHFIND_COMMON_HPP_

#include <array>
#include <stdexcept>

namespace pf {
	
	enum class Tile {
		PATH = 0, WALL = 1
	};

	template<std::size_t _ROWS, std::size_t _COLS>
	struct Map {
		static const std::size_t ROWS = _ROWS, COLS = _COLS;

		std::array< std::array<Tile, COLS>, ROWS > data;

		inline std::array<Tile, COLS> &operator[](std::size_t index)
		{
			if (index >= ROWS)
				throw std::out_of_range("Map::operator[]: index out of range");
			
			return data[index];
		}
		inline const std::array<Tile, COLS> &at(std::size_t index)
		{
			if (index >= ROWS)
				throw std::out_of_range("Map::operator[]: index out of range");
			
			return data[index];
		}
	};


}

#endif
