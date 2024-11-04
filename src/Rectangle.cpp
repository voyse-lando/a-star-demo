#include "Rectangle.hpp"
#include "Renderer.hpp"
#include <cstdint>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace shdr {

	const char *RECTANGLE_VERTEX =
R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexture;
out vec2 texture;

uniform float scale;
uniform vec3 translation;

void main() {

	gl_Position = vec4((aPos - translation) * scale + translation, 1.0f);
	texture = aTexture;
}
)";

	const char *RECTANGLE_FRAGMENT =
R"(
#version 330 core
in vec2 texture;
out vec4 FragColor;

uniform float aspect;
uniform float borderWidth;
uniform vec4 rectColor;
uniform vec4 borderColor;

void main() {
	float maxX = 1.0f - borderWidth;
	float minX = borderWidth;
	float maxY = maxX/aspect;
	float minY = minX/aspect;

	if  (texture.x < maxX && texture.x > minX
		&& texture.y < maxY && texture.y > minY) {

		FragColor = rectColor;
	} else {
		FragColor = borderColor;
	}
}
)";
};

std::optional<ElementBufferObjects> Rectangle::EBO = std::nullopt;
void Rectangle::setupEBO() {
	Rectangle::EBO = ElementBufferObjects(
		{
			// positions										 // textures			
			-1.f,  -1.f,  .0f,   0.0f,  0.0f,
			1.f,   -1.f,  .0f, 	 1.0f,  0.0f,
			1.f,  1.f, .0f,   1.0f, 1.0f,
			-1.f, 1.f, .0f,	 0.0f, 1.0f
		}, {
			0, 1, 2,
			0, 2, 3
	});
}

Rectangle::Rectangle() {
	if (Rectangle::EBO == std::nullopt) Rectangle::setupEBO();
	// #region compile_shader

	std::uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &shdr::RECTANGLE_VERTEX, NULL);

	glCompileShader(vertexShader);
	std::int32_t  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return;
	}

	std::uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &shdr::RECTANGLE_FRAGMENT, NULL);

	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return;
	}

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	// #endregion
}

void Rectangle::draw() const {

	glUseProgram(shaderProgram);

	std::int32_t floatScaleLocation = glGetUniformLocation(shaderProgram, "scale");
	std::int32_t vf3dTranslationLocation = glGetUniformLocation(shaderProgram, "translation");
	std::int32_t floatAspectLocation = glGetUniformLocation(shaderProgram, "aspect");
	std::int32_t floatBorderWidthLocation = glGetUniformLocation(shaderProgram, "borderWidth");
	std::int32_t vf4dColorLocation = glGetUniformLocation(shaderProgram, "rectColor");
	std::int32_t vf4dBorderColorLocation = glGetUniformLocation(shaderProgram, "borderColor");

	glUniform1f(floatScaleLocation, scale);
	glUniform1f(floatAspectLocation, 1.0f);
	glUniform1f(floatBorderWidthLocation, borderWidth);
	glUniform3f(
		vf3dTranslationLocation,
		VF2D_EXPAND(translation), 0.f
	);
	glUniform4f(
		vf4dColorLocation,
		VF4D_EXPAND(color)
	);
	glUniform4f(
		vf4dBorderColorLocation,
		VF4D_EXPAND(borderColor)
	);
	EBO->draw();
}

Rectangle &Rectangle::set_border_width(float value) {
	this->borderWidth = value;
	return *this;
}
Rectangle &Rectangle::set_scale(float value) {
	this->scale = value;
	return *this;
}
Rectangle &Rectangle::set_translation(const vf2d &value) {
	this->translation = value;
	return *this;
}
Rectangle &Rectangle::set_color(const vf4d &value) {
	this->color = value;
	return *this;
}
Rectangle &Rectangle::set_border_color(const vf4d &value) {
	this->borderColor = value;
	return *this;
}