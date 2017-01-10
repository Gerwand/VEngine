#pragma once

#include "Math/Vector4.h"
#include "Resources/Renderables/GUIElement.h"
#include "Resources/UI/Button.h"
#include "Engine/IO/Input.h"
#include <vector>

namespace vengine {

typedef std::vector<Button *> Buttons;

/*
* Simple class containing background of the GUI and few buttons 
*/
class Canvas
{
public:
	/* Default constructor setting background color */
	Canvas(const Vector4& backgroundColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	/* Destroys all buttons in the canvas */
	~Canvas();

	/* 
	* Add new button to the canvas.
	* 
	* @param button pointer to button allocated by new operator
	* @param layer on which layer gui should be draw, from -9 to 9.
	*/
	void AddButton(Button* button, int layer = 9);
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
Canvas::AddButton(Button* button, int layer)
{
	_buttons.push_back(button);
	_guiButtons.AddRectangle(button->GetPosition(), button->GetSize(), button->GetColor(), layer);
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