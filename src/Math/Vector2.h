#pragma once


#include <math.h>
#include <string>

#include "MathFunctions.h"

namespace vengine {

/**
*	Class representating vector containing two elements. It can describe i.e. coordinates in 2D space
*	or texture u, v coordinates.
*/
class Vector2 {
public:
	/**
	*	Default constructor doing nothing.
	*/
	Vector2();
	/**
	*	Fills vector with given value.
	*
	*	@param value value to be put into the vector.
	*/
	Vector2(float value);
	/**
	*	Initializes vector with two float values.
	*
	*	@param x attribute x of the vector.
	*	@param y attribute y of the vector.
	*/
	Vector2(float x, float y);
	/**
	*	Initializes vector from array containing 2 elements. First one will be assigned to x, second one to the y.
	*
	*	@param vector array containing two elements.
	*/
	Vector2(const float* vector);
	/**
	*	Copy constructor.
	*
	*	@param source vector to be copied.
	*/
	Vector2(const Vector2& source);

	/**
	*	Copy operator.
	*
	*	@param source vector to be copied.
	*	@return Reference to the object calling this operator.
	*/
	Vector2&	operator=(const Vector2& source);
	/**
	*	Copy given value to the vector.
	*
	*	@param value value copied to the vector.
	*	@return Reference to the object calling this operator.
	*/
	Vector2&	operator=(float value);

	/**
	*	Multiply each element of the vector by given value.
	*
	*	@param value value for multiplication.
	*	@return Reference to the object calling this operator.
	*/
	Vector2&	operator*=(float value);

	/**
	*	Add to each parameter of the vector given value.
	*
	*	@param value value to be added
	*	@return Reference to the object calling this operator.
	*/
	Vector2&	operator+=(float value);
	/**
	*	Add two vectors, adding coresponding values to each other.
	*
	*	@param vector vector to be added
	*	@return Reference to the object calling this operator.
	*/
	Vector2&	operator+=(const Vector2& vector);
	
	/**
	*	Subtract from each parameter of the vector given value.
	*
	*	@param value value to be subtracted
	*	@return Reference to the object calling this operator.
	*/
	Vector2&	operator-=(float value);
	/**
	*	Subtract two vectors, subtracting coresponding values to each other.
	*
	*	@param vector vector to be subtracted
	*	@return Reference to the object calling this operator.
	*/
	Vector2&	operator-=(const Vector2& vector);

	/**
	*	Divide each element of the vector by given value.
	*
	*	@param value value for division.
	*	@return Reference to the object calling this operator.
	*/
	Vector2&	operator/=(float value);

	/**
	*	Operator allowing to treat vector as an array containing 2 elements.
	*	Index 0 is x or u, index 1 is y or v.	
	*
	*	@param item index of the item from 0-1.
	*	@return Reference to the specified parameter.
	*/
	float&		operator[](int item);
	/**
	*	Operator allowing to treat vector as an array containing 2 elements.
	*	Index 0 is x or u, index 1 is y or v.
	*
	*	@param item index of the item from 0-1.
	*	@return Const reference to the specified parameter.	
	*/
	const float& operator[](int item) const;

	/**
	*	Converts vector to an array of 2 elements.
	*	@return Pointer to vector represented as an array.
	*/
	operator float*();
	operator const float*() const;

	/**
	*	Set new values to the vector.
	*	
	*	@param nx new value for the x.
	*	@param ny new value for the y.
	*	@return Reference to the object calling this method.
	*/
	Vector2&	Set(float nx, float ny);
	/**
	*	Set new values to the vector.
	*
	*	@param vectorArray array of two elements containing new values for x and y.
	*	@return Reference to the object calling this method.
	*/
	Vector2&	Set(const float* vectorArray);

	/**
	*	Normalize vector.
	*
	*	@return Reference to the object calling this method.
	*/
	Vector2&	Normalize();

	/**
	*	Calculates magnitude of the vector.
	*
	*	@return Magnitude of the vector.
	*/
	float		Magnitude() const;
	/**
	*	Calculates fast magnitude of the vector without square root.
	*
	*	@return Magnitude of the vector without square root.
	*/
	float		MagnitudeFast() const;
	/**
	*	Format vector to the string. Format output: (x, y) or in another convention (u, v).
	*
	*	@return std::string object containing formated vector parameters.
	*/
	std::string	ToString() const;

	/**
	*	Get normalized vector.
	*
	*	@param vector reference to the vector that is going to be normalized.
	*	@return Normalized vector.
	*/
	static Vector2	Normalized(const Vector2& vector);

	/**
	*	Get dot product of two vectors.
	*
	*	@param first first vector.
	*	@param second second vector.
	*	@return Dot product of two vectors.
	*/
	static float	Dot(const Vector2& first, const Vector2& second);
	/**
	*	Get distance between two vectors.
	*
	*	@param start starting point.
	*	@param end ending point.
	*	@return Distance between start and end.
	*/
	static float	Distance(const Vector2& start, const Vector2& end);

	/**
	*	Lineary interpolate between two vectors. Result will be clamped to be between start and end vectors.
	*
	*	@param start starting vector.
	*	@param end ending vector
	*	@param t interpolation parameter, where 0 means start vector and 1 means end vector.
	*	@return Interpolated vector.
	*/
	static Vector2	Lerp(const Vector2& start, const Vector2& end, float t);
	/**
	*	Lineary interpolate between two vectors.
	*
	*	@param start starting vector.
	*	@param end ending vector
	*	@param t interpolation parameter, where 0 means start vector and 1 means end vector.
	*	@return Interpolated vector.
	*/
	static Vector2	LerpUnclamped(const Vector2& start, const Vector2& end, float t);
	/**
	*	Results in vector which is closer to the end vector by distanceDelta units.
	*	If distance is below 0, resulting vector will further from end vector by distanceDelta units.
	*
	*	@param current vector repressnting current position.
	*	@param end ending vector.
	*	@param distanceDelta maximum distance that can be moved.
	*	@return Vector which is closer or further from end vector.
	*/
	static Vector2	MoveTowards(const Vector2& current, const Vector2& end, float distanceDelta);

	static const Vector2 ones;		/* Vector filled with ones.		*/
	static const Vector2 zeroes;	/* Vector filled with zeroes.	*/

	/*
	*	Vector2 can be described in many ways: as x, y 2D coordinates,
	*	as u, v texture coordinates and as two elements array.
	*/
	union {
		float atr[2];
		struct {
			float x;
			float y;
		};
		struct {
			float u;
			float v;
		};
	};
};


Vector2	operator-(const Vector2& vector);
Vector2	operator-(const Vector2& first, const Vector2& second);
Vector2 operator-(const Vector2& vector, float value);
Vector2	operator+(const Vector2& first, const Vector2& second);
Vector2	operator/(const Vector2& first, float value);
Vector2	operator*(const Vector2& first, const Vector2& second);
bool	operator==(const Vector2& first, const Vector2& second);
bool	operator!=(const Vector2& first, const Vector2& second);
std::ostream& operator<<(std::ostream &os, const Vector2& vector);

} //vengine