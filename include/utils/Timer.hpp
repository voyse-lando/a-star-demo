#pragma once

#include <chrono>

class Timer {
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
	float *result;
public:
	Timer(float *result);
	~Timer();
};

#define TIMER_START float __timer_time; { Timer t(&__timer_time)
#define TIMER_END } std::cout << __timer_time << '\n'