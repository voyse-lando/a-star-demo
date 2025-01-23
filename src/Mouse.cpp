#include "GLFW/glfw3.h"
#include "Mouse.hpp"

void mouse::button_callback(GLFWwindow *window, i32 button, i32 action, i32 mods) {
	auto mb = MouseButton::glfwToMouseButton.at(button);
	if (action == GLFW_PRESS) {
		mb->pressed = true;
		mb->released = false;
	}
	else {
		mb->pressed = false;
		mb->released = true;
	}
}
void mouse::position_callback(GLFWwindow* window, double xpos, double ypos) {
	Common::cursorPos = {(float)xpos, (float)ypos};
}

#define DEF_MB_MAPPING(glfw, mb) {GLFW_MOUSE_BUTTON_##glfw, &mb},
const std::unordered_map<i32, MouseButton *const> MouseButton::glfwToMouseButton = {
	DEF_MB_MAPPING(LEFT, Left)
	DEF_MB_MAPPING(RIGHT, Right)
	DEF_MB_MAPPING(MIDDLE, Middle)
	DEF_MB_MAPPING(4, MB4)
	DEF_MB_MAPPING(5, MB5)
	DEF_MB_MAPPING(6, MB6)
	DEF_MB_MAPPING(7, MB7)
	DEF_MB_MAPPING(8, MB8)
};

#define INIT_MB(name) MouseButton MouseButton::name = MouseButton();
FOR_EACH(INIT_MB,
	Left, Right, Middle, MB4, MB5, MB6, MB7, MB8
);