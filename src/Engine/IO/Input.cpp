#include "Input.h"

namespace vengine {
Input::KeyNames Input::_keyNames;
char Input::_keyStatus[GLFW_KEY_LAST + 1];
std::wstring Input::_textBuffer;
Input::InputMode Input::_inputMode;
bool Input::_shouldReturnBuff;

Vector2 Input::_previousPosition(Vector2::zeroes);
Vector2 Input::_totalOffset(Vector2::zeroes);
Vector2 Input::_frameOffset(Vector2::zeroes);

void
Input::UpdateInput()
{
	for (int i = 0; i < GLFW_KEY_LAST + 1; ++i)
		if (_keyStatus[i] == PRESS)
			_keyStatus[i] = REPEAT;
}

void
Input::Init(const char** names, const int *codes, int bindCount)
{
	for (int i = 0; i < bindCount; ++i) {
		_keyNames.insert(std::make_pair(*names, *codes));
		++names;
		++codes;
	}
	_textBuffer.reserve(MAX_BUF_LEN);
	ClearTextBuffer();

	glfwSetMouseButtonCallback(Window::GetGLFWWindow(), MouseButtonHandler);
	glfwSetCursorPosCallback(Window::GetGLFWWindow(), MouseMoveHandler);
}

bool
Input::IsPressed(const std::string& keyName)
{
	int keyIndex = _keyNames[keyName];
	return _keyStatus[keyIndex] == PRESS;
}
bool
Input::IsHolded(const std::string& keyName)
{
	int keyIndex = _keyNames[keyName];
	return _keyStatus[keyIndex] == REPEAT || _keyStatus[keyIndex] == PRESS;
}
bool 
Input::IsReleased(const std::string& keyName)
{
	int keyIndex = _keyNames[keyName];
	return _keyStatus[keyIndex] == RELEASE;
}
bool 
Input::IsPressed(int code)
{
	return _keyStatus[code] == PRESS;
}

bool 
Input::IsHolded(int code)
{
	return _keyStatus[code] == REPEAT || _keyStatus[code] == PRESS;
}

bool 
Input::IsReleased(int code)
{

	return _keyStatus[code] == RELEASE;
}

int 
Input::GetStatus(const std::string& keyName)
{
	int keyIndex = _keyNames[keyName];
	return _keyStatus[keyIndex];
}

void
Input::SetMode(InputMode mode) {
	switch (mode) {
	case NONE:
		glfwSetKeyCallback(Window::GetGLFWWindow(), NULL);
		glfwSetCharCallback(Window::GetGLFWWindow(), NULL);
	case TEXT_LINE_MODE:
		ClearTextBuffer();
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

int 
Input::GetMode()
{
	return _inputMode;
}

const std::wstring&
Input::GetTextBuffer()
{
	_shouldReturnBuff = false;
	return _textBuffer;
}

void 
Input::ClearTextBuffer()
{
	_textBuffer.clear();
}

void
Input::KeyHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	_keyStatus[key] = action;
}

void 
Input::TextCharHandler(GLFWwindow* window, unsigned int codepoint)
{
	AddSymbolToTextBuffer(codepoint);
}

void
Input::TextKeyHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
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
				//Delete cursor and next symbol
				_textBuffer.pop_back();
			}
		}
		break;
	}

}

void 
Input::AddSymbolToTextBuffer(wchar_t character)
{
	if (_textBuffer.length() < MAX_BUF_LEN - 1) {
		//Add character
		_textBuffer.push_back(character);
	}
}

int
Input::GetInputMode()
{
	return _inputMode;
}

bool 
Input::TextReady()
{
	return _shouldReturnBuff;
}

void
Input::DisableCursor() {
	glfwSetInputMode(Window::GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	double x, y;
	glfwGetCursorPos(Window::GetGLFWWindow(), &x, &y);
	_previousPosition.x = (float)x;
	_previousPosition.y = (float)y;
}

void
Input::EnableCursor() {
	glfwSetInputMode(Window::GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void
Input::MouseMoveHandler(GLFWwindow* window, double xpos, double ypos)
{
	Vector2 curPos = Vector2((float)xpos, (float)ypos);
	_totalOffset += curPos - _previousPosition;
	_previousPosition = curPos;
}

void 
Input::MouseButtonHandler(GLFWwindow* window, int button, int action, int mods)
{
	_keyStatus[button] = action;
}

const Vector2&
Input::GetMouseOffset()
{
	return _frameOffset;
}

void
Input::UpdateMouseOffset()
{
	_frameOffset = _totalOffset;
	_totalOffset = Vector2::zeroes;
}

}