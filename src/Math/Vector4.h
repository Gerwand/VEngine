#pragma once

#include <math.h>
#include <string>

#include "MathFunctions.h"
#include "Vector3.h"

namespace vengine {

/**
*	Class representating vector containing two elements. It can describe i.e. coordinates in 3D space for multiplicating with matrices
*	or rgba colour.
*/
class Vector4 {
public:
	/**
	*	Default constructor doing nothing.
	*/
	Vector4();
	/**
	*	Fills vector with given value.
	*
	*	@param value value to be put into the vector.
	*/
	Vector4(float value);
	/**
	*	Initializes vector with four float values.
	*
	*	@param x attribute x of the vector.
	*	@param y attribute y of the vector.
	*	@param z attribute z of the vector.
	*	@param w attribute w of the vector.
	*/
	Vector4(float x, float y, float z, float w);
	/**
	*	Initializes vector from array containing 4 elements. First one will be assigned to x, second one to the y, third one to the z
	*	and the last one to the w.
	*
	*	@param vector array containing four elements.
	*/
	Vector4(const float *vector);
	/**
	*	Copy constructor.
	*
	*	@param source vector to be copied.
	*/
	Vector4(const Vector4& source);
	/**
	*	Creates Vector4 from Vector3 plus value w.
	*
	*	@param source vector with x, y and z values.
	*	@param w attribute w of the vector.
	*/
	Vector4(const Vector3& source, float w);

	/**
	*	Copy operator.
	*
	*	@param source vector to be copied.
	*	@return Reference to the object calling this operator.
	*/
	Vector4&	operator=(const Vector4& source);
	/**
	*	Copy given value to the vector.
	*
	*	@param value value copied to the vector.
	*	@return Reference to the object calling this operator.
	*/
	Vector4&	operator=(float value);

	/**
	*	Multiply each element of the vector by given value.
	*
	*	@param value value for multiplication.
	*	@return Reference to the object calling this operator.
	*/
	Vector4&	operator*=(float value);

	/**
	*	Add to each parameter of the vector given value.
	*
	*	@param value value to be added
	*	@return Reference to the object calling this operator.
	*/
	Vector4&	operator+=(float value);
	/**
	*	Add two vectors, adding coresponding values to each other.
	*
	*	@param vector vector to be added
	*	@return Reference to the object calling this operator.
	*/
	Vector4&	operator+=(const Vector4& vector);

	/**
	*	Subtract from each parameter of the vector given value.
	*
	*	@param value value to be subtracted
	*	@return Reference to the object calling this operator.
	*/
	Vector4&	operator-=(float value);
	/**
	*	Subtract two vectors, subtracting coresponding values to each other.
	*
	*	@param vector vector to be subtracted
	*	@return Reference to the object calling this operator.
	*/
	Vector4&	operator-=(const Vector4& vector);

	/**
	*	Divide each element of the vector by given value.
	*
	*	@param value value for division.
	*	@return Reference to the object calling this operator.
	*/
	Vector4&	operator/=(float value);

	/**
	*	Operator allowing to treat vector as an array containing 4 elements.
	*	Index 0 is x or r, index 1 is y or g, index 2 is z or b and index 3 is w or a.
	*
	*	@param item index of the item from 0-3.
	*	@return Reference to the specified parameter.
	*/
	float&		operator[](int item);
	/**
	*	Operator allowing to treat vector as an array containing 4 elements.
	*	Index 0 is x or r, index 1 is y or g, index 2 is z or b and index 3 is w or a.
	*
	*	@param item index of the item from 0-3.
	*	@return Const reference to the specified parameter.
	*/
	const float& operator[](int item) const;

	/**
	*	Converts vector to an array of 4 elements.
	*	@return Pointer to vector represented as an array.
	*/
	operator float*();
	operator const float*() const;

	/**
	*	Set new values to the vector.
	*
	*	@param nx new value for the x.
	*	@param ny new value for the y.
	*	@param nz new value for the z.
	*	@param nw new value for the w.
	*	@return Reference to the object calling this method.
	*/
	Vector4&	Set(float nx, float ny, float nz, float nw);
	/**
	*	Set new values to the vector.
	*
	*	@param vectorArray array of two elements containing new values for x, y, z and w.
	*	@return Reference to the object calling this method.
	*/
	Vector4&	Set(const float *vectorArray);
	/**
	*	Set new values from Vector3 plus value w.
	*
	*	@param vector vector with x, y and z values.
	*	@param w attribute w of the vector.
	*/
	Vector4&	Set(const Vector3& vector, float w);

	/**
	*	Normalize vector.
	*
	*	@return Reference to the object calling this method.
	*/
	Vector4&	Normalize();

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
	*	Format vector to the string. Format output: (x, y, z, w) or in another convention (r, g, b, a).
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
	static Vector4	Normalized(const Vector4& vector);

	/**
	*	Get dot product of two vectors.
	*
	*	@param first first vector.
	*	@param second second vector.
	*	@return Dot product of two vectors.
	*/
	static float	Dot(const Vector4& first, const Vector4& second);
	/**
	*	Get distance between two vectors.
	*
	*	@param start starting point.
	*	@param end ending point.
	*	@return Distance between start and end.
	*/
	static float	Distance(const Vector4& start, const Vector4& end);

	/**
	*	Lineary interpolate between two vectors. Result will be clamped to be between start and end vectors.
	*
	*	@param start starting vector.
	*	@param end ending vector
	*	@param t interpolation parameter, where 0 means start vector and 1 means end vector. Will be clamped to be in range [0; 1].
	*	@return Interpolated vector.
	*/
	static Vector4	Lerp(const Vector4& start, const Vector4& end, float t);
	/**
	*	Spherical linear interpolation between two vectors. In linear interpolation, vector interpolates through sphere.
	*	When using Slerp, vector interpolates through sphere, so in 2D plane resulting move will be an arc, not line.
	*	In this version of function, result will be clamped to be between start and end.
	*
	*	@param start starting vector.
	*	@param end ending vector
	*	@param t interpolation parameter, where 0 means start vector and 1 means end vector. Will be clamped to be in range [0; 1].
	*	@return Interpolated vector.
	*/
	static Vector4	LerpUnclamped(const Vector4& start, const Vector4& end, float t);
	/**
	*	Rotates vector by maximum specified radians towards given destination. It length of current vector is different than length of the ending vector,
	*	length will be interpolated too by maximum given magnitude.
	*
	*	@param current vector repressnting current position.
	*	@param end ending vector.
	*	@param distanceDelta maximum distance that can be moved.
	*	@return Vector rotated towards end by maximum of radiansDelta and which magnitude is closer by maximum of manitudeDelta.
	*/
	static Vector4	MoveTowards(const Vector4& current, const Vector4& end, float distanceDelta);

	static const Vector4 ones;		/* Vector filled with ones.		*/
	static const Vector4 zeroes;	/* Vector filled with zeroes.	*/

	/*
	*	Vector4 can be described in many ways: as x, y, z, w 3D coordinates for matrix multiplication,
	*	as r, g, b, a color with alpha canal and as array of four elements.
	*/
	union {
		float atr[4];
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		struct {
			float r;
			float g;
			float b;
			float a;
		};
	};
};

Vector4	operator-(const Vector4& vector);
Vector4	operator-(const Vector4& vector, float value);
Vector4	operator-(const Vector4& first, const Vector4& second);
Vector4	operator+(const Vector4& first, const Vector4& second);
Vector4	operator/(const Vector4& first, float value);
Vector4	operator*(const Vector4& first, const Vector4& second);
bool	operator==(const Vector4& first, const Vector4& second);
bool	operator!=(const Vector4& first, const Vector4& second);
std::ostream& operator<<(std::ostream &os, const Vector4& vector);

} //vengine
