#pragma once

#include "Window.h"
#include "Strcmp.h"
#include "Math/Vector2.h"

#include <GLFW/glfw3.h>
#include <string>
#include <map>
#include <vector>
#include <iostream>

namespace vengine {

/* Settings for text buffer and unicode codes */
#define MAX_BUF_LEN		256
#define UTF32_ENTER		'\n'
#define UTF32_TAB		'\t'
#define UTF32_CURSOR 	0x000025AE

/*
*	Class Input is static class which is managing input from both keyboard and mouse.
*/
class Input
{
public:
	/* Describes input modes for keyboard */
	enum InputMode {
		NONE,			/* There is no input handling for keyboard */
		TEXT_LINE_MODE, /* Text is going to be handled as buffer until \n character is read */
		KEY_MODE,		/* Each key is respected separately */
	};

	/* Key statuses */
	enum KeyStatus {
		PRESS = GLFW_PRESS,		/* Key just have been pressed */
		RELEASE = GLFW_RELEASE, /* Key is not holded down */
		REPEAT = GLFW_REPEAT	/* Key has been holded for more than one frame */
	};

	/*
	* Initializes input, sets default callbacks and loads names for key codes.
	*  
	*  @param names array with names of the keys
	*  @param codes GLFW key codes for named keys
	*  @param bindCound number of mapped keys
	*/
	static void Init(const char** names, const int *codes, int bindCount);

	/*
	* Checks if key have just been pressed.
	* 
	* @param keyName name of the key
	* @return true or false
	*/
	static bool IsPressed(const std::string& keyName);
	/*
	* Checks if key have been pressed or holded.
	*
	* @param keyName name of the key
	* @return true or false
	*/
	static bool IsHolded(const std::string& keyName);
	/*
	* Checks if key is not pressed
	*
	* @param keyName name of the key
	* @return true or false
	*/
	static bool IsReleased(const std::string& keyName);

	/*
	* Checks if key have just been pressed.
	*
	* @param code GLFW key code
	* @return true or false
	*/
	static bool IsPressed(int code);
	/*
	* Checks if key have been pressed or holded.
	*
	* @param code GLFW key code
	* @return true or false
	*/
	static bool IsHolded(int code);
	/*
	* Checks if key is not pressed
	*
	* @param code GLFW key code
	* @return true or false
	*/
	static bool IsReleased(int code);

	/*
	* Get exact status of the key.
	*
	* @param keyName name of the key
	* @return one of the KeyStatus
	*/
	static int GetStatus(const std::string& keyName);

	/*
	* Sets key mode for the input - buffer or direct input.
	*
	* @param mode mode for input
	*/
	static void SetMode(InputMode mode);

	/*
	* Get current input mode
	*
	* @return current input mode
	*/
	static int GetMode();

	/*
	* Get text buffer from buffered input
	*
	* @return Buffer of last received characters during buffered mode.
	*/
	static const std::wstring& GetTextBuffer();
	/* Wipes all characters from the input text buffer */
	static void ClearTextBuffer();

	/*
	* Gets current input mode.
	*
	* @return Current input mode
	*/
	static int GetInputMode();
	/*
	* Checks if current text buffer is ready or not
	*
	* @return True of false
	*/
	static bool TextReady();

	/* Disables mouse cursor */
	static void DisableCursor();
	/* Enables mouse cursor */
	static void EnableCursor();
	/* Get current mouse mode
	*
	*  @return true if cursor is enabled, false if it is not enabled
	*/
	static bool GetCursorMode();

	/*
	*  Get current mouse position
	*
	*  @return mouse position
	*/
	static const Vector2& GetCursorPosition();

	/*
	* Updates input variables and state before each frame.
	*/
	static void UpdateInput();

	/*
	* Get mouse offset calcualted from last call of the UpdateInput
	*
	* @return mouse offset from last frame
	*/
	static const Vector2& GetMouseOffset();
	
	/*
	* Updates mouse offset.
	*/
	static void UpdateMouseOffset();
private:
	typedef std::map<std::string, int, istring_less> KeyNames;

	static KeyNames _keyNames;					/* Maps key names with key codes */
	static char _keyStatus[GLFW_KEY_LAST + 1];	/* Array with key statuses */
	static std::wstring _textBuffer;			/* Text buffer for buffererd input mode */
	static InputMode _inputMode;				/* Current key input mode */
	static bool _shouldReturnBuff;				/* Indicates that buffer should be read */
	static bool _cursorMode;					/* Indicates that mouse cursor is enabled or disabled */

	static Vector2 _previousPosition;			/* Position of the cursor on last frame */
	static Vector2 _totalOffset;				/* Total offset summed from multiple callbacks on one frame */
	static Vector2 _frameOffset;				/* Offset during one frame */

	/* Normal mode key handler */
	static void KeyHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
	/* Buffered mode text handler for special characters and keys */
	static void TextKeyHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
	/* Buffered mode text handler for character symbols */
	static void TextCharHandler(GLFWwindow* window, unsigned int codepoint);

	/* Handler for mouse moving */
	static void MouseMoveHandler(GLFWwindow* window, double xpos, double ypos);
	/* Handler for mouse button pressing */
	static void MouseButtonHandler(GLFWwindow* window, int button, int action, int mods);

	/* Adds one symbol to text buffer */
	static void AddSymbolToTextBuffer(wchar_t character);
};

/*****************************************************/

inline bool
Input::IsPressed(const std::string& keyName)
{
	int keyIndex = _keyNames[keyName];
	return _keyStatus[keyIndex] == PRESS;
}

inline bool
Input::IsHolded(const std::string& keyName)
{
	int keyIndex = _keyNames[keyName];
	return _keyStatus[keyIndex] == REPEAT || _keyStatus[keyIndex] == PRESS;
}

inline bool
Input::IsReleased(const std::string& keyName)
{
	int keyIndex = _keyNames[keyName];
	return _keyStatus[keyIndex] == RELEASE;
}

inline bool
Input::IsPressed(int code)
{
	return _keyStatus[code] == PRESS;
}

inline bool
Input::IsHolded(int code)
{
	return _keyStatus[code] == REPEAT || _keyStatus[code] == PRESS;
}

inline bool
Input::IsReleased(int code)
{

	return _keyStatus[code] == RELEASE;
}

inline int
Input::GetStatus(const std::string& keyName)
{
	int keyIndex = _keyNames[keyName];
	return _keyStatus[keyIndex];
}

inline int
Input::GetMode()
{
	return _inputMode;
}

inline const std::wstring&
Input::GetTextBuffer()
{
	_shouldReturnBuff = false;
	return _textBuffer;
}

inline void
Input::ClearTextBuffer()
{
	_textBuffer.clear();
}

inline void
Input::KeyHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	_keyStatus[key] = action;
}

inline void
Input::TextCharHandler(GLFWwindow* window, unsigned int codepoint)
{
	AddSymbolToTextBuffer(codepoint);
}

inline void
Input::AddSymbolToTextBuffer(wchar_t character)
{
	if (_textBuffer.length() < MAX_BUF_LEN - 1) {
		_textBuffer.push_back(character);
	}
}

inline int
Input::GetInputMode()
{
	return _inputMode;
}

inline bool
Input::TextReady()
{
	return _shouldReturnBuff;
}

inline void
Input::EnableCursor() {
	glfwSetInputMode(Window::GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	_cursorMode = true;
}

inline void
Input::DisableCursor() {
	glfwSetInputMode(Window::GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	_cursorMode = false;
}

inline bool
Input::GetCursorMode() {
	return _cursorMode;
}

inline const Vector2&
Input::GetCursorPosition()
{
	return _previousPosition;
}

inline void
Input::MouseMoveHandler(GLFWwindow* window, double xpos, double ypos)
{
	Vector2 curPos = Vector2((float)xpos, (float)ypos);
	_totalOffset += curPos - _previousPosition;
	_previousPosition = curPos;
}

inline void
Input::MouseButtonHandler(GLFWwindow* window, int button, int action, int mods)
{
	_keyStatus[button] = action;
}

inline const Vector2&
Input::GetMouseOffset()
{
	return _frameOffset;
}

inline void
Input::UpdateMouseOffset()
{
	_frameOffset = _totalOffset;
	_totalOffset = Vector2::zeroes;
}

}