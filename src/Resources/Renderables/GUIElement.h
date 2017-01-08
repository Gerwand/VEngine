#pragma once

#include "Renderable.h"

namespace vengine {

class GUIElement : public Renderable {
public:
	void AddRectangle(const Vector2& start, const Vector2& dimension, Vector4 color, int layer = 0);
	virtual void Draw(Renderer* renderer);

protected:
	virtual void FillInfo(RenderInfo* info);
	virtual void ActivateAttributes();
};

}