#pragma once
#ifndef MAP_RECTANGLE_HPP_
#define MAP_RECTANGLE_HPP_
#include "Shader.hpp"
#include "Object.hpp"
#include <atomic>
#include <mutex>
#include <optional>

class Rectangle {
public:
	static std::optional<Object> object;
	static std::optional<Shader> shader;

private:
	static std::once_flag _initialized_;
	static bool _init_();

private:
	std::atomic<vf2d> translate, scale;

	std::atomic<float> borderSize = 0.0f;
	std::atomic<vf4d> fillColor, borderColor;
public:

	Rectangle();
	Rectangle(Rectangle &&rhs);
	Rectangle(const Rectangle &rhs);
	Rectangle operator=(const Rectangle &rhs);

	Rectangle &set_translate(const vf2d &translate);
	Rectangle &set_scale(const vf2d &scale);

	Rectangle &set_border_size(float size);
	Rectangle &set_fill_color(const vf4d &color);
	Rectangle &set_border_color(const vf4d &color);

	void draw() const;
};

#endif // ! MAP_RECTANGLE_HPP_