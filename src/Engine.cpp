#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Engine.hpp"
#include "Rectangle.hpp"

Engine::Engine()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

bool Engine::construct(std::uint32_t width, std::uint32_t height)
{
  window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
      std::cerr << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return false;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    glfwTerminate();
    window = NULL;
    return false;
  }
  
  renderer.setViewport(0, 0, width, height);
  return true;
}


void Engine::start()
{
	// renderer.create_data();
	if (!on_create()) {
		glfwTerminate();
		return;
	}
	
  while(!glfwWindowShouldClose(window))
  {
		if(!on_update()) break;

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
}
