#include "Lines.h"

namespace vengine {

Lines::Lines() : Renderable(), _color(1.0f, 1.0f, 1.0f)
{
}

void
Lines::AddLine(const Vector3& start, const Vector3& end)
{
	AddLine(start, end, _color);
}

void 
Lines::AddLine(const Vector3& start, const Vector3& end, const Vector3& color)
{
	int vertNumber = _vertices.size();
	_vertices.push_back({ start, Vector3::zeroes, Vector2::zeroes, 0, 0, _color });
	_vertices.push_back({ end, Vector3::zeroes, Vector2::zeroes, 0, 0, _color });

	_indices.push_back(vertNumber);
	_indices.push_back(vertNumber + 1);
	_changed = true;
}

void
Lines::FillInfo(RenderInfo* info)
{
	Renderable::FillInfo(info);
	info->wired = false;
	info->textured = false;
	info->drawType = GL_LINES;
}

void 
Lines::Draw(Renderer* renderer)
{
	RenderInfo info;

	DrawStart(&info);

	renderer->Draw(info, Renderer::STANDARD);

	DrawEnd();
}

}