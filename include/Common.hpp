#pragma once
#ifndef COMMON_H_
#define COMMON_H_

#include <cstdint> // IWYU pragma: export
#include <tuple>

using vi2d = std::tuple<std::int32_t, std::int32_t>;
using vf2d = std::tuple<float, float>;
using vf4d = std::tuple<float, float, float, float>;

#define VI2D_EXPAND(vect) std::get<0>(vect), std::get<1>(vect)
#define VF2D_EXPAND(vect) std::get<0>(vect), std::get<1>(vect)
#define VF4D_EXPAND(vect) std::get<0>(vect), std::get<1>(vect), std::get<2>(vect), std::get<3>(vect)

#endif // !COMMON_H_
