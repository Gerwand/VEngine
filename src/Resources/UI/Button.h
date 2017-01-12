#pragma once

#include "Math/Vector4.h"

namespace vengine {

/* Simple class representing button. Can perform some actions when clicked. */
class Button
{
public:
	Button();
	Button(const Vector2& position, const Vector2& size, const Vector4& color = Vector4(0.0f));

	void SetColor(const Vector4& color);

	void SetPosition(const Vector2& position);
	void SetSize(const Vector2& size);

	const Vector2& GetPosition();
	const Vector2& GetSize();
	const Vector4& GetColor();

	/* Check if point is inside the button */
	bool IsInside(const Vector2& point);

	/* Do something when clicked. Will be called by canvas class */
	virtual void OnClick() { std::cout << "Click!" << std::endl; };
private:
	std::string _description;
	Vector2 _position;
	Vector2 _size;
	Vector4 _color;
};

inline
Button::Button()
{
}

inline
Button::Button(const Vector2& position, const Vector2& size, const Vector4& color) :
	_position(position),
	_size(size),
	_color(color)
{

}

inline void
Button::SetColor(const Vector4& color)
{
	_color = color;
}

inline void
Button::SetPosition(const Vector2& position)
{
	_position = position;
}

inline void
Button::SetSize(const Vector2& size)
{
	_size = size;
}

inline bool
Button::IsInside(const Vector2& point)
{
	return between(point.x, _position.x, _position.x + _size.x) &&
		between(point.y, _position.y, _position.y + _size.y);
}


inline const Vector2&
Button::GetPosition()
{
	return _position;
}

inline const Vector2&
Button::GetSize()
{
	return _size;
}

inline const Vector4& 
Button::GetColor()
{
	return _color;
}


}