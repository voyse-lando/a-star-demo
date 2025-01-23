#pragma once
#include "Common.hpp"
#include <unordered_map>

#define PARENS ()
#define EXPAND(arg) EXPAND1(EXPAND1(EXPAND1(EXPAND1(arg))))
#define EXPAND1(arg) EXPAND2(EXPAND2(EXPAND2(EXPAND2(arg))))
#define EXPAND2(arg) EXPAND3(EXPAND3(EXPAND3(EXPAND3(arg))))
#define EXPAND3(arg) EXPAND4(EXPAND4(EXPAND4(EXPAND4(arg))))
#define EXPAND4(arg) arg
#define FOR_EACH(macro, ...)                                    \
  __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, a1, ...)                         \
  macro(a1)                                                     \
  __VA_OPT__(FOR_EACH_AGAIN PARENS (macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define DEF_MB(name) static MouseButton name;

class GLFWwindow;

namespace mouse {
	extern void button_callback(GLFWwindow *window, i32 button, i32 action, i32 mods);
	extern void position_callback(GLFWwindow* window, double xpos, double ypos);
};

struct MouseButton {
private:
	static const std::unordered_map<i32, MouseButton *const> glfwToMouseButton;
public:
	bool pressed{ false }, released{ true };

	FOR_EACH(DEF_MB,
		Left, Right, Middle, MB4, MB5, MB6, MB7, MB8
	);
	friend void mouse::button_callback(GLFWwindow *window, i32 button, i32 action, i32 mods);
};