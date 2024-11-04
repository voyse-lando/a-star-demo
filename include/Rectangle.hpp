#pragma once
#ifndef RECTANGLE_HPP_
#define RECTANGLE_HPP_

#include "Renderer.hpp"
#include <optional>

namespace shdr {

	extern const char *RECTANGLE_VERTEX;
	extern const char *RECTANGLE_FRAGMENT;
};

struct Rectangle {
	static std::optional<ElementBufferObjects> EBO;
	static void setupEBO();
	std::uint32_t shaderProgram;
	float borderWidth = 0.0f;
	float scale;
	vf2d translation;
	vf4d color, borderColor;

	Rectangle();
	void draw() const;

	Rectangle &set_border_width(float value);
	Rectangle &set_scale(float value);
	Rectangle &set_translation(const vf2d &value);
	Rectangle &set_color(const vf4d &value);
	Rectangle &set_border_color(const vf4d &value);
};

#endif // ! RECTANGLE_HPP_