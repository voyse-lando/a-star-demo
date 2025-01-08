#pragma once
#ifndef ENGINE_H_
#define ENGINE_H_

#include "Common.hpp"

class GLFWwindow;

class Engine
{
protected:
  GLFWwindow *window;
  std::string appName;
public:
  Engine();

	virtual bool on_create() = 0;
	virtual bool on_update() = 0;
	virtual void before_exit() {};

  bool construct(std::uint32_t width, std::uint32_t height);
  void start();
};

#endif // !ENGINE_H_
