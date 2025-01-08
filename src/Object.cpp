#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Object.hpp"

Object::Object(std::initializer_list<float> buffer_, std::initializer_list<u32> elements_)
: buffer(buffer_), elements(elements_), stride(0)
{ }

Object &Object::add_attribute(u32 index, u32 size) {
	attributes.push_back({(i32)index, (i32)size});
	stride += size;

	return *this;
}

Object &Object::bind() {
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(
		GL_ARRAY_BUFFER, 
		sizeof(float) * this->buffer.size(),
		&this->buffer[0], 
		GL_STATIC_DRAW
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, 
		sizeof(std::uint32_t) * this->elements.size(),
		&this->elements[0],
		GL_STATIC_DRAW
	);

	u64 ptr = 0;
	for (auto &[i, s] : attributes) {
		glVertexAttribPointer(
			i, 
			s, 
			GL_FLOAT, 
			GL_FALSE, 
			stride * sizeof(float), 
			(void*)(ptr * sizeof(float))
		);
		glEnableVertexAttribArray(i);
		ptr += s;
	}

	return *this;
}

void Object::draw() const {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}