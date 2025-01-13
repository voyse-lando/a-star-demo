#include "Common.hpp"
#include <chrono>

vu2d Common::windowSize = {600, 600};
float Common::zNear = .1f, Common::zFar = 100.0f;

std::chrono::milliseconds Common::pathDelay = 0ms;

std::array<float, 16> Common::projectionScaleMatrix = {
	2.0f / (windowSize.x), .0f, .0f, .0f,
	.0f, 2.0f / (windowSize.y), .0f, .0f,
	.0f, .0f, 2.0f / (zFar-zNear), .0f,
	.0f, .0f, .0f, 1.0f
};

std::array<float, 16> Common::projectionTranslateMatrix = {
	1.0f, .0f, .0f, .0f,
	.0f, 1.0f, .0f, .0f,
	.0f, .0f, -1.0f, .0f,
	windowSize.x/2.0f, windowSize.y/2.0f, -(zFar+zNear)/2.0f, 1.0f
};

std::array<float, 16> &Common::get_projection_scale_matrix() {
	return projectionScaleMatrix;
}
std::array<float, 16> &Common::get_projection_translate_matrix() {
	return projectionTranslateMatrix;
}

void Common::update_projection_matrix() {
	projectionScaleMatrix[0] = 2.0f / (windowSize.x);
	projectionScaleMatrix[5] = 2.0f / (windowSize.y);
	projectionScaleMatrix[10] = 2.0f / (zFar-zNear);
	projectionTranslateMatrix[12] = -(windowSize.x) / 2.0f;
	projectionTranslateMatrix[13] = -(windowSize.y) / 2.0f;
	projectionTranslateMatrix[14] = -(zFar+zNear)/2.0f;
}