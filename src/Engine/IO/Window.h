#pragma once

#include "Assert.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>


namespace vengine {
class Window {
public:
	/* Initializes GLFW Window */
	static void Init(int width, int height, const char* title);


	static void SetWindowSize(int width, int height);
	static void GetWindowSize(int *width, int *height);

	static void GetScreenSize(int *width, int *height);

	static void SetTitle(const char* title);
	static const char* GetTitle();

	static bool CreateWindowed();
	static bool CreateFullScreen();
	static bool CreateWindowedFullScreen();

	static void Destroy();

	static bool IsValid();

	static void MakeActiveContext();

	static bool IsOpened();

	static bool SizeChanged();

	static void HandleWindow();

	static GLFWwindow* GetGLFWWindow();

	static void Close();
private:
	static int _width, _height;
	static const char* _title;

	static bool _resized;

	static GLFWwindow* _window;

	static void ResizeHandler(GLFWwindow* window, int width, int height);
};
}
