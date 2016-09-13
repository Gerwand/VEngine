#pragma once

#define _USE_MATH_DEFINES
#include <math.h>


#define between(val, floor, ceil) ((val) >= (floor) && (val) <= (ceil))
#define around(in, val, tresh) (((in) >= (val)*(1.0f - (tresh))) && ((in) <= (val)*(1.0f + (tresh))))

/**
*	Converts angle value from radian to degrees.
*	@param angleRad angle in radians.
*	@return Angle in degrees.
*/	
inline float radToDeg(float angleRad)
{
	return angleRad * 180.0f / (float)M_PI;
}

/** 
*	Converts angle value from degrees to angles.
*	@param angleRad angle in degrees.
*	@return Angle in radians.
*/
inline float degToRad(float angleDeg)
{
	return angleDeg * (float)M_PI / 180.0f;
}

/** 
*	Clamps value to range between floor and ceiling.
*	If value is outside [a, b] range, it will be clamped to this range.
*	@param value value to be clamped.
*	@param floor lower limit for value - a parameter in description.
*	@param ceiling higher limit for value - b parameter in description.
*	@return Clamped value.
*/
inline float clamp(float value, float floor, float ceiling)
{
	return value < floor ? floor : value > ceiling ? ceiling : value;
}
