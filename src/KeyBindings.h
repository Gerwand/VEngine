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


static unsigned char swordVoxels[] = {
	0, 0, 3, 3, 3, 0, 0,
	0, 0, 0, 4, 0, 0, 0,
	0, 0, 0, 4, 0, 0, 0,
	3, 3, 3, 3, 3, 3, 3,
	3, 0, 4, 4, 4, 0, 3,
	0, 0, 4, 4, 4, 0, 0,
	0, 0, 4, 2, 4, 0, 0,
	0, 0, 4, 2, 4, 0, 0,
	0, 0, 4, 4, 4, 0, 0,
	0, 0, 2, 4, 2, 0, 0,
	0, 0, 0, 2, 0, 0, 0,

	0, 0, 3, 3, 3, 0, 0,
	0, 0, 0, 4, 0, 0, 0,
	0, 0, 0, 4, 0, 0, 0,
	3, 3, 3, 3, 3, 3, 3,
	3, 0, 4, 4, 4, 0, 3,
	0, 0, 4, 4, 4, 0, 0,
	0, 0, 4, 2, 4, 0, 0,
	0, 0, 4, 2, 4, 0, 0,
	0, 0, 4, 4, 4, 0, 0,
	0, 0, 2, 4, 2, 0, 0,
	0, 0, 0, 2, 0, 0, 0

};
static struct {
	const int x = 7;
	const int y = 11;
	const int z = 2;
} swordSize;

static unsigned char cube[] = {
	1
};
static struct {
	const int x = 1;
	const int y = 1;
	const int z = 1;
} cubeSize;

}