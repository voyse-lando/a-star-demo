#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"
#include "Common.hpp"
#include <iostream>

Shader::Shader() {
	this->shaderProgram = glCreateProgram();
}

Shader &Shader::set_vertex_src(const char *src) {
	this->vertexShaderSource = src;
	return *this;
}
Shader &Shader::set_fragment_src(const char *src) {
	this->fragmentShaderSource = src;
	return *this;
}
Shader &Shader::compile() {	
	int  success;
	char infoLog[512];

	auto vert = glCreateShader(GL_VERTEX_SHADER);
	auto frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vert, 1, &vertexShaderSource, NULL);
	glShaderSource(frag, 1, &fragmentShaderSource, NULL);

	glCompileShader(vert);
	glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
	if(!success) {
    glGetShaderInfoLog(vert, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if(!success) {
    glGetShaderInfoLog(frag, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glAttachShader(shaderProgram, vert);
	glAttachShader(shaderProgram, frag);
	glLinkProgram(shaderProgram);

	glDeleteShader(vert);
	glDeleteShader(frag);

	compiled = true;
	return *this;
}

Shader &Shader::use() {
	glUseProgram(shaderProgram);
	return *this;
}

#define PROTO_SET_UNIFORM(Type, GLFunctionSetUniform, VALUE_EXPAND) \
Shader &Shader::set_uniform(const char *name, const Type& value) { \
	auto loc = glGetUniformLocation(shaderProgram, name); \
	GLFunctionSetUniform(loc, VALUE_EXPAND(value)); \
	return *this; \
}

PROTO_SET_UNIFORM(float, glUniform1f, NO_EXPAND);
PROTO_SET_UNIFORM(vf2d, glUniform2f, V2D_EXPAND);
PROTO_SET_UNIFORM(vf3d, glUniform3f, V3D_EXPAND);
PROTO_SET_UNIFORM(vf4d, glUniform4f, V4D_EXPAND);
PROTO_SET_UNIFORM(i32, glUniform1f, NO_EXPAND);
PROTO_SET_UNIFORM(vi2d, glUniform2f, V2D_EXPAND);
PROTO_SET_UNIFORM(vi3d, glUniform3f, V3D_EXPAND);
PROTO_SET_UNIFORM(vi4d, glUniform4f, V4D_EXPAND);
PROTO_SET_UNIFORM(bool, glUniform1i, NO_EXPAND);

Shader &Shader::set_uniform_mat4(const char *name, const std::array<float, 16> &values) {
	auto loc = glGetUniformLocation(shaderProgram, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, values.data());
	return *this;
}