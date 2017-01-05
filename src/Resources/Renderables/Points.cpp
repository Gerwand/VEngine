#include "Points.h"

namespace vengine {

Points::Points() : Renderable(), _color(1.0f, 1.0f, 1.0f)
{
}

void
Points::AddPoint(const Vector3& point)
{
	AddPoint(point, _color);
}

void
Points::AddPoint(const Vector3& point, const Vector3& color)
{
	int vertNumber = _vertices.size();
	_vertices.push_back({ point, Vector3::zeroes, Vector2::zeroes, 0, 0, _color });

	_indices.push_back(vertNumber);
	_changed = true;
}

void
Points::FillInfo(RenderInfo* info)
{
	Renderable::FillInfo(info);
	info->wired = false;
	info->textured = false;
	info->drawType =  GL_POINTS;
	//info->depthEnabled = false;
}

void
Points::Draw(Renderer* renderer)
{
	RenderInfo info;

	DrawStart(&info);
	glPointSize(10.0f);
	renderer->Draw(info, Renderer::STANDARD);

	DrawEnd();
}

}