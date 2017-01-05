#pragma once

#include <GLFW/glfw3.h>

namespace vengine {

static const int bindNum = 11;

static const char* keyNames[bindNum] = {
	"Forward",
	"Back",
	"Left",
	"Right",
	"Jump",
	"Walk",
	"Attack1",
	"Attack2",
	"Use",
	"Enter",
	"Exit"
};

static const int keyCodes[bindNum] = {
	GLFW_KEY_W,
	GLFW_KEY_S,
	GLFW_KEY_A,
	GLFW_KEY_D,
	GLFW_KEY_SPACE,
	GLFW_KEY_LEFT_SHIFT,
	GLFW_MOUSE_BUTTON_LEFT,
	GLFW_MOUSE_BUTTON_RIGHT,
	GLFW_KEY_E,
	GLFW_KEY_ENTER,
	GLFW_KEY_ESCAPE
};

}
