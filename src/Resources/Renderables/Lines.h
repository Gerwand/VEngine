#pragma once

#include "Renderable.h"

namespace vengine {

/* Allows to add lines and render them with proper renderer */
class Lines : public Renderable
{
public:	
	Lines();

	void SetColor(const Vector4& color);
	const Vector4& GetColor() const;

	void AddLine(const Vector3& start, const Vector3& end);
	void AddLine(const Vector3& start, const Vector3& end, const Vector4& color);

	virtual void Draw(Renderer* renderer);

protected:
	Vector4 _color;

	virtual void FillInfo(RenderInfo* info);
};



inline void 
Lines::SetColor(const Vector4& color)
{
	_color = color;
}

inline const Vector4& 
Lines::GetColor() const
{
	return _color;
}

}