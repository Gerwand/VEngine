#pragma once

#include "Renderable.h"

namespace vengine {

class Lines : public Renderable
{
public:	
	Lines();


	void SetColor(const Vector3& color);
	const Vector3& GetColor() const;
	void AddLine(const Vector3& start, const Vector3& end);
	void AddLine(const Vector3& start, const Vector3& end, const Vector3& color);

	virtual void Draw(Renderer* renderer);

protected:
	Vector3 _color;

	virtual void FillInfo(RenderInfo* info);
};



inline void 
Lines::SetColor(const Vector3& color)
{
	_color = color;
}

inline const Vector3& 
Lines::GetColor() const
{
	return _color;
}



}