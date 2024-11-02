#include "Renderer.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void Renderer::setViewport(UInt32 x, UInt32 y, UInt32 h, UInt32 w)
{
  glViewport(x, y, w, h);
}

