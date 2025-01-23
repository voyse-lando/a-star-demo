#include "Common.hpp"
#include "Config/Config.hpp"
#include <chrono>

vu2d Common::windowSize = {600, 600};
vf2d Common::cursorPos = {0.0f, 0.0f };

std::chrono::milliseconds Common::pathDelay = 0ms;

char Common::symbolWall = '5', Common::symbolPath = '0', Common::symbolRoute = '.';

void Common::from_config(const Config &config) {
	Common::windowSize = {config.screenWidth.value, config.screenHeight.value};
	Common::symbolWall = config.symWall.value;
	Common::symbolPath = config.symPath.value;
	Common::symbolRoute = config.symRoute.value;
}