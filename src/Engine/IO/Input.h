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

#define MAX_BUF_LEN		256
#define UTF32_ENTER		'\n'
#define UTF32_TAB		'\t'
#define UTF32_CURSOR 	0x000025AE

class Input
{
public:
	enum InputMode {
		NONE,
		TEXT_LINE_MODE,
		KEY_MODE,
	};

	enum KeyStatus {
		PRESS = GLFW_PRESS,
		RELEASE = GLFW_RELEASE,
		REPEAT = GLFW_REPEAT
	};

	static void Init(const char** names, const int *codes, int bindCount);

	static bool IsPressed(const std::string& keyName);
	static bool IsHolded(const std::string& keyName);
	static bool IsReleased(const std::string& keyName);
	static bool IsPressed(int code);
	static bool IsHolded(int code);
	static bool IsReleased(int code);

	static int GetStatus(const std::string& keyName);

	static void SetMode(InputMode mode);
	static int GetMode();
	static const std::wstring& GetTextBuffer();
	static void ClearTextBuffer();

	static int GetInputMode();
	static bool TextReady();

	static void DisableCursor();
	static void EnableCursor();

	static void UpdateInput();

	static const Vector2& GetMouseOffset();
	static void UpdateMouseOffset();
private:
	static void KeyHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void TextKeyHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void TextCharHandler(GLFWwindow* window, unsigned int codepoint);
	static void MouseMoveHandler(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonHandler(GLFWwindow* window, int button, int action, int mods);
	
	static void AddSymbolToTextBuffer(wchar_t character);

	typedef std::map<std::string, int, istring_less> KeyNames;

	static KeyNames _keyNames;
	static char _keyStatus[GLFW_KEY_LAST + 1];
	static std::wstring _textBuffer;
	static InputMode _inputMode;
	static bool _shouldReturnBuff;

	static Vector2 _previousPosition;
	static Vector2 _totalOffset;
	static Vector2 _frameOffset;
};



}