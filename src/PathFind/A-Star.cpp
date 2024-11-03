#include "PathFind/A-Star.hpp"
#include <cmath>


bool operator<(const pf::Node &left, const pf::Node &right) {
	return left.f < right.f;
}

float pf::calculate_distance(
	std::tuple<std::int32_t, std::int32_t> &beginPos,
	std::tuple<std::int32_t, std::int32_t> &endPos
) {
	auto &[begY, begX] = beginPos; 
	auto &[endY, endX] = endPos;

	auto
		distX = begX - endX,
		distY = begY - endY;

	return std::sqrt(distX*distX + distY*distY);
}

std::vector<std::tuple<std::int32_t, std::int32_t>> pf::get_neighbours(
	std::tuple<std::int32_t, std::int32_t> pos,
	std::size_t maxRows, std::size_t maxCols
) {
	std::vector<std::tuple<std::int32_t, std::int32_t>> ret;

	const auto &[y, x] = pos;

	if (y - 1 >= 0)
	{
		ret.push_back({y-1, x});
	}
	if (y + 1 < maxRows)
	{
		ret.push_back({y+1, x});
	}
	if (x + 1 < maxCols)
	{
		ret.push_back({y, x+1});
	}
	if (x - 1 >= 0)
	{
		ret.push_back({y, x-1});
	}

	return ret;
}