#include "Engine.hpp"

#include <iostream>

int main()
{
	std::cout << "A* Demo ver 0.1" << std::endl;

  Engine engine;

  if (engine.construct(800, 600))
    engine.start();


  return 0;
}
