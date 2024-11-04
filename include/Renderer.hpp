#pragma once
#ifndef RENDERER_H_
#define RENDERER_H_

#include "Common.hpp"
#include <initializer_list>
#include <vector>

namespace shdr {

	extern const char *VERTEX;
	extern const char *FRAGMENT;
};

struct ElementBufferObjects
{
	std::vector<float> vertices;
	std::vector<std::uint32_t> indices;
  std::uint32_t VAO, VBO, EBO;

	void draw() const;

	ElementBufferObjects(
		std::initializer_list<float> vertices,
		std::initializer_list<std::uint32_t> indices
	);
};

class Renderer
{
  std::uint32_t VAO, EBO, ShaderProgram;
public:
	void create_data();

  void setViewport(std::uint32_t x, std::uint32_t y, std::uint32_t h, std::uint32_t w);
	void draw_triangle();
};

#endif // !RENDERER_H_
