#pragma once
#ifndef ENGINE_H_
#define ENGINE_H_

#include "Common.hpp"
#include "Renderer.hpp"

class GLFWwindow;

class Engine
{
private:
  GLFWwindow *window;
  Renderer renderer;
public:
  Engine();

  bool construct(UInt32 width, UInt32 height);
  void start();
};

#endif // !ENGINE_H_
