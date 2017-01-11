#include "GUIElement.h"

namespace vengine {

void
GUIElement::AddRectangle(const Vector2& start, const Vector2& dimension, Vector4 color, int layer)
{
	int vertNumber = _vertices.size();
	float z = layer / 10.0f;
	Vector3 pts[4] = {
		Vector3(start, z),
		Vector3(start.x + dimension.x, start.y, z),
		Vector3(start + dimension, z),
		Vector3(start.x, start.y + dimension.y, z)
	};

	_vertices.push_back({ pts[0], Vector3::zeroes, Vector2::zeroes, 0, 0, color });
	_vertices.push_back({ pts[1], Vector3::zeroes, Vector2::zeroes, 0, 0, color });
	_vertices.push_back({ pts[2], Vector3::zeroes, Vector2::zeroes, 0, 0, color });
	_vertices.push_back({ pts[3], Vector3::zeroes, Vector2::zeroes, 0, 0, color });

	_indices.push_back(vertNumber + 3);
	_indices.push_back(vertNumber + 1);
	_indices.push_back(vertNumber);
	_indices.push_back(vertNumber + 3);
	_indices.push_back(vertNumber + 2);
	_indices.push_back(vertNumber + 1);
	_changed = true;
}

void 
GUIElement::Draw(Renderer* renderer)
{
	RenderInfo info;

	DrawStart(&info);
	renderer->Draw(info, Renderer::GUI);
	DrawEnd();
}

void 
GUIElement::FillInfo(RenderInfo* info)
{
	Renderable::FillInfo(info);
	info->drawType = GL_TRIANGLES;
}

void 
GUIElement::ActivateAttributes()
{
	_vao.ActivateBinded(POSITION, 3, GL_FLOAT, sizeof(Vertex), (GLvoid*)0);
	_vao.ActivateBinded(COLOR, 4, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
}

}