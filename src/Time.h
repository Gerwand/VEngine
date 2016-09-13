#pragma once

#include <GLFW/glfw3.h>

namespace vengine {

class Time {
public:
	static void Update();
	static float DeltaTime();
	static float GetTime();
private:
	static float _time;
	static float _lastTime;
	static float _deltaTime;
};

inline float 
Time::GetTime()
{
	return _time;
}

}