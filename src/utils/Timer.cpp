#include "utils/Timer.hpp"
#include <chrono>

Timer::Timer(float *result)
: start(std::chrono::high_resolution_clock::now()), result(result) {}
Timer::~Timer() {
	*result = std::chrono::duration<float, std::ratio<1,1>>(std::chrono::high_resolution_clock::now() - start).count();
}