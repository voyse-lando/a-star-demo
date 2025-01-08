#include "Map/Rectangle.hpp"
#include <mutex>

std::optional<Object> Rectangle::object = std::nullopt;
std::optional<Shader> Rectangle::shader = std::nullopt;
std::once_flag Rectangle::_initialized_ = std::once_flag();

bool Rectangle::_init_() {
	object = Object(
		{
			//		VERTICES			//		TEXTURE
			-1.0, -1.0, 0.0,		0.0, 1.0,
			1.0, -1.0, 0.0,		1.0, 1.0,
			1.0, 1.0, 0.0,	1.0, 0.0,
			-1.0, 1.0, 0.0,	0.0, 0.0
		},
		{
			0, 1, 2,
			0, 2, 3
		}).add_attribute(0, 3).add_attribute(1, 2).bind();

	
	shader = Shader()
		.set_vertex_src(R"(
#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
out vec2 tex;
uniform vec2 translate;
uniform vec2 scale;

void main() {
	vec4 pos = vec4(aPos, 1.0f);
	pos.xy += translate;
	pos.xy *= scale;

	gl_Position = pos;
	tex = aTex;
}
	)").set_fragment_src(R"(
#version 330

in vec2 tex;
out vec4 FragColor;

uniform float borderSize;
uniform vec4 fillColor;
uniform vec4 borderColor;

void main() {
	if (( tex.x > (1.0 - borderSize) || tex.x < borderSize ) || ( tex.y > (1.0 - borderSize) || tex.y < borderSize )) {
		FragColor = borderColor;
	} else {
		FragColor = fillColor;
	}
}
	)").compile();

	return true;
}

Rectangle::Rectangle() {
	std::call_once(Rectangle::_initialized_, Rectangle::_init_);
}
Rectangle::Rectangle(Rectangle &&rhs) {
	scale = rhs.scale.load();
	translate = rhs.translate.load();

	borderSize = rhs.borderSize.load();
	fillColor = rhs.fillColor.load();
	borderColor = rhs.borderColor.load();
}
Rectangle::Rectangle(const Rectangle &rhs) {
	scale = rhs.scale.load();
	translate = rhs.translate.load();

	borderSize = rhs.borderSize.load();
	fillColor = rhs.fillColor.load();
	borderColor = rhs.borderColor.load();
}
Rectangle Rectangle::operator=(const Rectangle &rhs) {
	scale = rhs.scale.load();
	translate = rhs.translate.load();
	
	borderSize = rhs.borderSize.load();
	fillColor = rhs.fillColor.load();
	borderColor = rhs.borderColor.load();
	return *this;
}

Rectangle &Rectangle::set_translate(const vf2d &trs) {
	translate = trs;
	return *this;
}
Rectangle &Rectangle::set_scale(const vf2d &scl) {
	scale = scl;
	return *this;
}

Rectangle &Rectangle::set_border_size(float size) {
	borderSize = size;
	return *this;
}
Rectangle &Rectangle::set_fill_color(const vf4d &color) {
	fillColor = color;
	return *this;
}
Rectangle &Rectangle::set_border_color(const vf4d &color) {
	borderColor = color;
	return *this;
}


void Rectangle::draw() const {
	Rectangle::shader->use()
		.set_uniform("scale", scale)
		.set_uniform("translate", translate)

		.set_uniform("borderSize", borderSize)
		.set_uniform("fillColor", fillColor)
		.set_uniform("borderColor", borderColor)
	;
	Rectangle::object->draw();
}