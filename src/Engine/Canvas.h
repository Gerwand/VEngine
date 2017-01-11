#pragma once

#include "Math/Vector4.h"
#include "Resources/Renderables/GUIElement.h"
#include "Resources/UI/Button.h"
#include "Engine/IO/Input.h"
#include <vector>

namespace vengine {

typedef std::vector<Button *> Buttons;

/*
* Simple class containing background of the GUI and few buttons. 
* Point (0, 0) is upper left corner of the canvas and it uses pixels as the unit of the measurement.
*/
class Canvas
{
public:
	/* Default constructor setting background color and initializing GUI elements */
	Canvas(const Vector4& backgroundColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	/* Destroys all buttons in the canvas */
	~Canvas();

	/* 
	* Add new button to the canvas.
	* 
	* @param button pointer to button allocated by new operator
	* @param layer on which layer gui should be drawn, from -9 to 9, where -9 is nearest.
	*/
	void AddButton(Button* button, int layer = 9);
	
	/* Draw GUI, should be called at the end of the all drawing routines */
	void Draw(Renderer* renderer);

	/*
	* Update GUI, should be called each frame to check if buttons were
	* pressed and if window resized.
	*
	* @return If any button was pressed, return true
	*/
	bool Update();

private:
	Buttons _buttons;			/* Vector containing pointers to all buttons */
	Vector4 _backgroundColor;	/* Color of the background */
	GUIElement _guiButtons;		/* Renderable element which is keeping buttons */
	GUIElement _guiBackground;	/* Renderable element which is keeping background */
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
	/* If window was resized, we must update background rectangle. */
	if (Window::SizeChanged()) {
		int w, h;
		Window::GetScreenSize(&w, &h);
		_guiBackground.ClearVertices();
		_guiBackground.AddRectangle(0.0f, Vector2(float(w), float(h)), _backgroundColor, 9);
	}

	/* Check if any button was pressed and run proper handler for it */
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