#pragma once

#include <GLFW/glfw3.h>

namespace vengine {

/*
* Class for storing time values.
*/
class Time {
public:
	/* Run this function to update delta time value */
	static void Update();

	/* Time between two Update calls - used for calculating time used for generating one frame. Will be clamped to one second to prevent physic errors */
	static float DeltaTime();

	/* Get simulation time */
	static float GetTime();
private:
	static float _time;			/* Time of the simulation */
	static float _lastTime;		/* There will be stored time when Update is called */
	static float _deltaTime;	/* Delta time between two Update calls */
};

inline float 
Time::GetTime()
{
	return _time;
}

}