#include "Time.h"


namespace vengine {
float Time::_time;
float Time::_lastTime;
float Time::_deltaTime;


void
Time::Update()
{
	_time = (float)glfwGetTime();
	_deltaTime = _time - _lastTime;
	_lastTime = _time;
}

float
Time::DeltaTime()
{
	/* We want to clamp delta time if it is aboce 1 second to prevent physic bugs */
	return _deltaTime > 1.0f ? 1.0f : _deltaTime;
}

}