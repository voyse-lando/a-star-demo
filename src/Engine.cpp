#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Engine.hpp"

Engine::Engine()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

bool Engine::construct(UInt32 width, UInt32 height)
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
  while(!glfwWindowShouldClose(window))
  {
    glClearColor(.2f, .2f, .4f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
}
