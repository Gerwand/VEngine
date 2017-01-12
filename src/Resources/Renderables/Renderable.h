#pragma once
#include "Engine/Vertex.h"
#include "Resources/OGL/GlBuffer.h"
#include "Resources/OGL/VertexArray.h"
#include "Engine/Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

namespace vengine {

typedef std::vector<Vertex> Vertices;
typedef std::vector<GLuint> Indices;

/* Abstract class representing all renderables objects */
class Renderable {
public:
	/* Default constructor */
	Renderable();

	/* Initializes all obejcts used for rendering */
	void Init();
	/* Deleteas all resources */
	void Delete();

	/* Get vertices stored in renderable object */
	const Vertices& GetVertices();
	/* Get indices containing information about order of the vertices */
	const Indices& GetIndices();

	/* Add new vertices to the renderable. It will be markes as changed. */
	void AddVertices(const Vertices& vertices, const Indices& indices);
	/* Delete all vertices from the renderable. */
	void ClearVertices();

	/* Drawing routine - it should call DrawStart first, then render everything and DrawEnd at the end */
	virtual void Draw(Renderer* renderer) = 0;

protected:
	/* Layouts of variables used in shaders */
	enum VaoPos {
		POSITION,
		NORMALS,
		TEXTURE,
		COLOR,
		ATLAS_OFFSET
	};

	bool _changed; /* Check if object changed, if yes, resources must be assigned again */

	GlBuffer _vbo;	/* Vertex Buffer Object - all vertices */
	GlBuffer _ebo;	/* Element Buffer Object - indices */
	VertexArray _vao; /* Vertex Array Object - binds variables from the buffers to the shader */

	Vertices _vertices;	/* Vertices of the renderable */
	Indices _indices;	/* Indices of the renderable */

	void UpdateBuffers();	/* Updates buffers and set that object is not changed */

	/* If object changed, update buffers. Call FillInfo and fill RenderInfo structure. Bind VAO and EBO. */
	void DrawStart(RenderInfo* info); 
	/* End drawing routine - unbind all buffers  */
	void DrawEnd();	

	/* Set VAO to pass to shader proper values from given structure. Can be overloaded by inheritated objects. */
	virtual void ActivateAttributes();
	/* FIll RenderInfo with proper data for rendering, which will be passed to renderable object */
	virtual void FillInfo(RenderInfo* info);
};

}
