#include "Window.h"


namespace vengine {
int Window::_width;
int Window::_height;
const char* Window::_title;
GLFWwindow* Window::_window = NULL;
bool Window::_resized = true;

void
Window::Init(int width, int height, const char* title)
{
	if (IsValid())
		Destroy();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_width = width;
	_height = height;
	_title = title;
}

void
Window::SetWindowSize(int width, int height)
{
	_width = width;
	_height = height;
	glfwSetWindowSize(_window, _width, _height);
	_resized = true;
}
	
void
Window::GetWindowSize(int *width, int *height)
{
	*width = _width;
	*height = _height;
}

void
Window::GetScreenSize(int *width, int *height)
{
	glfwGetFramebufferSize(_window, width, height);
}

void
Window::SetTitle(const char* title)
{
	_title = title;
	glfwSetWindowTitle(_window, _title);
}

const char*
Window::GetTitle()
{
	return _title;
}

bool
Window::CreateWindowed()
{
	if (_window == NULL) {
		_window = glfwCreateWindow(_width, _height, _title, NULL, NULL);
		return IsValid();

		glfwSetWindowSizeCallback(_window, ResizeHandler);
	}
	return false;
}

bool
Window::CreateFullScreen()
{
	if (_window == NULL) {
		_window = glfwCreateWindow(_width, _height, _title, glfwGetPrimaryMonitor(), NULL);
		return IsValid();
	}
	return false;
}

bool
Window::CreateWindowedFullScreen()
{
	if (_window == NULL) {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, _title, monitor, NULL);
		return IsValid();
	}
	return false;
}

void
Window::Destroy() 
{
	glfwDestroyWindow(_window);
}

bool 
Window::IsValid()
{
	return (_window != NULL);
}

void
Window::MakeActiveContext() 
{
	glfwMakeContextCurrent(_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize OpenGL context\n");
	}
	//glewExperimental = GL_TRUE;
	//if (glewInit() != GLEW_OK) {
	//	printf("Failed to initialize GLEW\n");
	//}
}

bool 
Window::IsOpened()
{
	assert(IsValid(), "Window must be activated first");
	return !glfwWindowShouldClose(_window);
}

void 
Window::HandleWindow()
{
	_resized = false;
	glfwSwapInterval(1);
	glfwSwapBuffers(_window);
	glfwPollEvents();
}

GLFWwindow* 
Window::GetGLFWWindow()
{
	return _window;
}

void
Window::Close()
{
	assert(IsValid(), "Window must be activated first");
	glfwSetWindowShouldClose(_window, 1);
}

bool 
Window::SizeChanged()
{
	return (_resized);
}

void
Window::ResizeHandler(GLFWwindow* window, int width, int height)
{
	assert(window == Window::_window, "Pointer to window is different...");

	Window::_width = width;
	Window::_height = height;

	Window::_resized = true;
}

}