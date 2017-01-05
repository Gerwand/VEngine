#pragma once


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Math/Vector3.h"
#include "Math/Vector2.h"

namespace vengine {

struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector2 texUV;
	int atlasOffset[2];
	Vector3 color;
};


}