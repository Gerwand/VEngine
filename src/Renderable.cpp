#include "Renderable.h"


namespace vengine {

Renderable::Renderable() : _changed(false)
{
}

void
Renderable::Init()
{
	_vao.Init();
	_vbo.Init();
	_ebo.Init();
}

void 
Renderable::Delete()
{
	_vao.Delete();
	_vbo.Delete();
	_ebo.Delete();
	_vertices.clear();
	_indices.clear();
}

void
Renderable::AddVertices(const Vertices& vertices, const Indices& indices)
{
	_vertices.insert(std::end(_vertices), std::begin(vertices), std::end(vertices));
	_indices.insert(std::end(_indices), std::begin(indices), std::end(indices));
	_changed = true;
}

void
Renderable::ClearVertices()
{
	_vertices.clear();
	_indices.clear();
	_changed = true;
}


void
Renderable::UpdateBuffers()
{
	_vao.Bind();

	_vbo.Bind(GlBuffer::VERTEX);
	_vbo.ReserveMutableSizeData(_vertices.data(), _vertices.size() * sizeof(Vertex), GlBuffer::DYNAMIC_DRAW);
	ActivateAttributes();
	_vbo.Unbind(GlBuffer::VERTEX);

	_ebo.Bind(GlBuffer::INDICES);
	_ebo.ReserveMutableSizeData(_indices.data(), _indices.size() * sizeof(GLuint), GlBuffer::DYNAMIC_DRAW);
	_ebo.Unbind(GlBuffer::INDICES);

	_vao.Unbind();

	_changed = false;
}

void 
Renderable::DrawStart(RenderInfo* info)
{
	if (_changed) {
		UpdateBuffers();
	}

	FillInfo(info);

	_vao.Bind();
	_ebo.Bind(GlBuffer::INDICES);
}

void 
Renderable::DrawEnd()
{
	_vao.Unbind();
	_ebo.Unbind(GlBuffer::INDICES);
}


const Vertices& 
Renderable::GetVertices()
{
	return _vertices;
}
const Indices&
Renderable::GetIndices()
{
	return _indices;
}

void
Renderable::ActivateAttributes()
{
	_vao.ActivateBinded(POSITION, 3, GL_FLOAT, sizeof(Vertex), (GLvoid*)0);
	_vao.ActivateBinded(NORMALS, 3, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	_vao.ActivateBinded(TEXTURE, 3, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texUV));
	_vao.ActivateBinded(COLOR, 3, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
}

void
Renderable::FillInfo(RenderInfo* info)
{
	info->indicesNumber = _indices.size();
}



}