#pragma once

#include "Resources/OGL/VertexArray.h"
#include "Resources/OGL/GlBuffer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace vengine {

/* Structure containing infomartion about rendering */
struct RenderInfo {
	GLuint indicesNumber; /* Number of indices */

	bool textured;		/* Should the object be textured - if false it will be colored */
	unsigned int tex;	/* What texture should it use - handle from textureManager */

	bool wired;			/* Should the object be drawn wired */

	Vector3 color;		/* Color of the object if not textured */

	GLenum drawType;	/* GL_POINTS, GL_TRAINGLES etc.*/
	float pointSize;	/* Size for GL_POINTS */
};

}