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
	return _deltaTime;
}

}