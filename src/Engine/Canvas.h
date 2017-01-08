#pragma once

#include "Math/Vector4.h"
#include "Resources/Renderables/GUIElement.h"
#include "Resources/UI/Button.h"
#include "Engine/IO/Input.h"
#include <vector>

namespace vengine {

typedef std::vector<Button *> Buttons;

class Canvas
{
public:
	Canvas(const Vector4& backgroundColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	~Canvas();

	void AddButton(Button* button);
	void Draw(Renderer* renderer);

	bool Update();

private:
	Buttons _buttons;
	Vector4 _backgroundColor;
	GUIElement _guiButtons;
	GUIElement _guiBackground;
};

inline
Canvas::Canvas(const Vector4& backgroundColor)
{
	_guiButtons.Init();
	_guiBackground.Init();
	_backgroundColor = backgroundColor;
	int w, h;
	Window::GetScreenSize(&w, &h);
	_guiBackground.AddRectangle(0.0f, Vector2(float(w), float(h)), _backgroundColor, 9);
}

inline 
Canvas::~Canvas()
{
	for (Buttons::iterator it = _buttons.begin(); it != _buttons.end(); ++it)
		delete (*it);
}

inline void
Canvas::AddButton(Button* button)
{
	_buttons.push_back(button);
	_guiButtons.AddRectangle(button->GetPosition(), button->GetSize(), button->GetColor());
}

inline void
Canvas::Draw(Renderer* renderer)
{
	_guiButtons.Draw(renderer);
	_guiBackground.Draw(renderer);
}

inline bool 
Canvas::Update()
{
	if (Window::SizeChanged()) {
		int w, h;
		Window::GetScreenSize(&w, &h);
		//_guiButtons.ClearVertices();
		//for (Buttons::iterator it = _buttons.begin(); it != _buttons.end(); ++it)
		//	_guiButtons.AddRectangle((*it)->GetPosition(), (*it)->GetSize(), (*it)->GetColor());

		_guiBackground.ClearVertices();
		_guiBackground.AddRectangle(0.0f, Vector2(float(w), float(h)), _backgroundColor, 9);
	}

	bool changed = false;
	if (Input::GetCursorMode() && Input::IsPressed(GLFW_MOUSE_BUTTON_1))
		for (Buttons::iterator it = _buttons.begin(); it != _buttons.end(); ++it)
			if ((*it)->IsInside(Input::GetCursorPosition())) {
				(*it)->OnClick();
				changed = true;
			}

	return changed;
}


}