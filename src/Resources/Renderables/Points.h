#pragma once

#include "Renderable.h"

namespace vengine {

/* Simple renderable class representing points */
class Points : public Renderable
{
public:
	Points();

	/* Set color of the all points that will be added after this call */
	void SetColor(const Vector3& color);
	const Vector3& GetColor() const;
	
	void AddPoint(const Vector3& point);
	void AddPoint(const Vector3& point, const Vector3& color);

	virtual void Draw(Renderer* renderer);

protected:
	Vector3 _color;

	virtual void FillInfo(RenderInfo* info);
};

inline void
Points::SetColor(const Vector3& color)
{
	_color = color;
}

inline const Vector3&
Points::GetColor() const
{
	return _color;
}

}