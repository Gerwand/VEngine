#pragma once

#include "Button.h"

namespace vengine {

/* Button that will be toggling one value on and off every click */
class ToggleButton : public Button
{
public:
	ToggleButton();
	ToggleButton(const Vector2& position, const Vector2& size, const Vector4& color = Vector4(0.0f));

	bool GetValue();
	virtual void OnClick();
private:
	bool _toggle; 
};

inline
ToggleButton::ToggleButton()
{
	_toggle = false;
}

inline
ToggleButton::ToggleButton(const Vector2& position, const Vector2& size, const Vector4& color) :
	Button(position, size, color)
{
	_toggle = false;
}

inline bool
ToggleButton::GetValue()
{
	return _toggle;
}

inline void
ToggleButton::OnClick()
{
	_toggle = !_toggle;
}

}