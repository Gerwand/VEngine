#pragma once

#include "Assert.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>


namespace vengine {
/*
* Windwo is static class with all 
*/
class Window {
public:
	/* Initializes GLFW Window */
	static void Init(int width, int height, const char* title);

	/* Resize window */
	static void SetWindowSize(int width, int height);
	/* Get window size */
	static void GetWindowSize(int *width, int *height);

	/* Get screen size - not only renderable area, but full window with borders*/
	static void GetScreenSize(int *width, int *height);

	/* Set title of the window */
	static void SetTitle(const char* title);
	/* Get tile of the window */
	static const char* GetTitle();

	/* Create window in window mode */
	static bool CreateWindowed();
	/* Create window in fullscreen mode */
	static bool CreateFullScreen();
	/* Create window in borderless mode - windowed fullscreen */
	static bool CreateWindowedFullScreen();

	/* Destroy window */
	static void Destroy();

	/* Check if window was created properly */
	static bool IsValid();

	/* Make this window active OpenGL context */
	static void MakeActiveContext();

	/* Check if window is opened */
	static bool IsOpened();

	/* Check if size of the window changed in the last frame */
	static bool SizeChanged();

	/* Swap buffers and poll all events, should be called at the end of the cycle */
	static void HandleWindow();

	/* Get GLFW window pointer */
	static GLFWwindow* GetGLFWWindow();

	/*
	 * Set that window should be closed - however it is not closing window. 
	 * It is giving information that window should be closed
	 */
	static void Close();

private:
	static int _width, _height; /* Width and height of the window */
	static const char* _title; /* Title of the window */

	static bool _resized; /* Flag indicating that window has been resized */

	static GLFWwindow* _window; /* GLFW window pointer */

	/*
	* Handler routine called when window has been resized, it is setting
	* window size and indicating that window has been resized.
	*/
	static void ResizeHandler(GLFWwindow* window, int width, int height);
};
}
