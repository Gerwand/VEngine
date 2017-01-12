#include "Input.h"

namespace vengine {

/*****************************************************/

/* Input state variables*/
Input::InputMode Input::_inputMode;
std::wstring Input::_textBuffer;

/* Key satatuses */
Input::KeyNames Input::_keyNames;
char Input::_keyStatus[GLFW_KEY_LAST + 1];
bool Input::_shouldReturnBuff;
bool Input::_cursorMode = true;

/* Cursor position variables */
Vector2 Input::_previousPosition(Vector2::zeroes);
Vector2 Input::_totalOffset(Vector2::zeroes);
Vector2 Input::_frameOffset(Vector2::zeroes);

/*****************************************************/

void
Input::UpdateInput()
{
	for (int i = 0; i < GLFW_KEY_LAST + 1; ++i)
		/* If key was pressed for one frame, it should be indicated as repeat during next frame */
		if (_keyStatus[i] == PRESS)
			_keyStatus[i] = REPEAT;

	UpdateMouseOffset();
}

void
Input::Init(const char** names, const int *codes, int bindCount)
{
	/* Put all key names into the map */
	for (int i = 0; i < bindCount; ++i) {
		_keyNames.insert(std::make_pair(*names, *codes));
		++names;
		++codes;
	}

	/* Reserve space for text buffer, because it will be constant, so we can spare some allocations */
	_textBuffer.reserve(MAX_BUF_LEN);
	ClearTextBuffer();

	/* Set default callbacks. */
	glfwSetMouseButtonCallback(Window::GetGLFWWindow(), MouseButtonHandler);
	glfwSetCursorPosCallback(Window::GetGLFWWindow(), MouseMoveHandler);
}

void
Input::SetMode(InputMode mode) {
	switch (mode) {
	case NONE:
		glfwSetKeyCallback(Window::GetGLFWWindow(), NULL);
		glfwSetCharCallback(Window::GetGLFWWindow(), NULL);
	case TEXT_LINE_MODE:
		ClearTextBuffer();
		/* There are two callbacks, one is reading unicode characters, the other one is reading key inputs */
		glfwSetKeyCallback(Window::GetGLFWWindow(), TextKeyHandler);
		glfwSetCharCallback(Window::GetGLFWWindow(), TextCharHandler);
		break;
	case KEY_MODE:
		memset(_keyStatus, 0, GLFW_KEY_LAST + 1);	
		glfwSetKeyCallback(Window::GetGLFWWindow(), KeyHandler);
		glfwSetCharCallback(Window::GetGLFWWindow(), NULL);
		break;
	}
}

void
Input::TextKeyHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/* Read special character for the buffer */
	switch (key) {
	case GLFW_KEY_ENTER:
		if (action == PRESS) {
			_textBuffer.push_back(UTF32_ENTER);
			_shouldReturnBuff = true;
		}
		break;
	case GLFW_KEY_ESCAPE:
		if (action == PRESS) {
			ClearTextBuffer();
			_shouldReturnBuff = true;
		}
		break;
	case GLFW_KEY_TAB:
		if (action != RELEASE)
			AddSymbolToTextBuffer(UTF32_TAB);
		break;
	case GLFW_KEY_BACKSPACE:
		if (action != RELEASE) {
			if (!_textBuffer.empty()) {
				/* Delete cursor and next symbol */
				_textBuffer.pop_back();
			}
		}
		break;
	}
}


}