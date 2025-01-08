#pragma once
#ifndef RENDERER_OBJECT_HPP_
#define RENDERER_OBJECT_HPP_

#include "Common.hpp"
#include <initializer_list>
#include <vector>

class Object {
private:
	u32 VAO, VBO, EBO;
	std::vector<u32> elements;
	std::vector<vi2d> attributes;
	std::vector<float> buffer;
	u64 stride;
public:
	Object(std::initializer_list<float> buffer_, std::initializer_list<u32> elements_);

	Object &add_attribute(u32 index, u32 size);
	Object &bind();
	void draw() const;
};

#endif // ! RENDERER_OBJECT_HPP_