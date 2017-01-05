#pragma once

#include "Managers/VertexArray.h"
#include "Managers/GlBuffer.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace vengine {

struct RenderInfo {
	GLuint indicesNumber;

	bool textured;
	unsigned int tex;

	bool wired;
	Vector3 color;

	GLenum drawType;
	float pointSize;
	bool depthEnabled = true;
};

}