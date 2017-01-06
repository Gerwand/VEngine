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

class Renderable {
public:
	Renderable();

	virtual void Init();
	virtual void Delete();

	const Vertices& GetVertices();
	const Indices& GetIndices();
	void AddVertices(const Vertices& vertices, const Indices& indices);
	void ClearVertices();

	virtual void Draw(Renderer* renderer) = 0;

protected:
	enum VaoPos {
		POSITION,
		NORMALS,
		TEXTURE,
		COLOR,
		ATLAS_OFFSET
	};

	bool _changed;

	GlBuffer _vbo;
	GlBuffer _ebo;
	VertexArray _vao;

	Vertices _vertices;
	Indices _indices;

	void UpdateBuffers();

	void DrawStart(RenderInfo* info);
	void DrawEnd();	
	virtual void ActivateAttributes();
	virtual void FillInfo(RenderInfo* info);
};

}
