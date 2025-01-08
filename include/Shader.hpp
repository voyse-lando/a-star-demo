#pragma once
#ifndef RENDERER_SHADER_HPP_
#define RENDERER_SHADER_HPP_

#include "Common.hpp"
#include <array>

class Shader {
private:
	const char *vertexShaderSource = nullptr, *fragmentShaderSource = nullptr;
	u32 shaderProgram;
	bool compiled = false;
public:
	Shader();

	Shader &set_vertex_src(const char *src);
	Shader &set_fragment_src(const char *src);
	Shader &compile();
	Shader &use();

	Shader &set_uniform(const char *name, const float& value);
	Shader &set_uniform(const char *name, const vf2d& value);
	Shader &set_uniform(const char *name, const vf3d& value);
	Shader &set_uniform(const char *name, const vf4d& value);
	Shader &set_uniform(const char *name, const i32& value);
	Shader &set_uniform(const char *name, const vi2d& value);
	Shader &set_uniform(const char *name, const vi3d& value);
	Shader &set_uniform(const char *name, const vi4d& value);
	Shader &set_uniform(const char *name, const bool& value);

	Shader &set_uniform_mat4(const char *name, const std::array<float, 16> &values);
};

#endif // ! RENDERER_SHADER_HPP_