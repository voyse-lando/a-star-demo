#pragma once
#ifndef COMMON_HPP_
#define COMMON_HPP_

#include <chrono>
#include <cstdint> // IWYU pragma: export
#include <tuple>   // IWYU pragma: export

typedef std::uint8_t  u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;
typedef std::int8_t  i8;
typedef std::int16_t i16;
typedef std::int32_t i32;
typedef std::int64_t i64;

template <class T>
struct v2d {
	T x, y;

	v2d() {}
	v2d(const T &x, const T &y) : x(x), y(y) { }

	bool operator==(v2d<T> rhs) const {
		return x == rhs.x && y == rhs.y;
	}
};
template <class T>
struct v3d {
	T x, y, z;

	v3d() {}
	v3d(const T &x, const T &y, const T &z) : x(x), y(y), z(z) { }

	bool operator==(v3d<T> rhs) const {
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}
};
template <class T>
struct v4d {
	T x, y, z, w;

	v4d() {}
	v4d(const T &x, const T &y, const T &z, const T &w) : x(x), y(y), z(z), w(w) { }

	bool operator==(v4d<T> rhs) const {
		return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
	}
};

typedef v2d<i32> vi2d;
typedef v2d<u32> vu2d;
typedef v2d<float> vf2d;
typedef v3d<i32> vi3d;
typedef v3d<u32> vu3d;
typedef v3d<float> vf3d;
typedef v4d<i32> vi4d;
typedef v4d<u32> vu4d;
typedef v4d<float> vf4d;

#define NO_EXPAND(_v1d) _v1d
#define V2D_EXPAND(_v2d) _v2d.x, _v2d.y
#define V3D_EXPAND(_v3d) _v3d.x, _v3d.y, _v3d.z
#define V4D_EXPAND(_v4d) _v4d.x, _v4d.y, _v4d.z, _v4d.w

class Config;

using namespace std::chrono_literals;
class Common {
public:
	static vu2d windowSize;
	static vf2d cursorPos;

	static std::chrono::milliseconds pathDelay;

	static char symbolWall, symbolPath, symbolRoute;

	static void from_config(const Config &config);
};

#endif // ! COMMON_HPP_