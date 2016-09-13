#pragma once


#include <math.h>
#include <string>

#include "MathFunctions.h"

namespace vengine {
/**
*	Class representating vector containing two elements. It can describe i.e. coordinates in 3D space
*	or rgb colour.
*/
class Vector3 {
public:
	/**
	*	Default constructor doing nothing.
	*/
	Vector3();
	/**
	*	Fills vector with given value.
	*
	*	@param value value to be put into the vector.
	*/
	Vector3(float value);
	/**
	*	Initializes vector with three float values.
	*
	*	@param x attribute x of the vector.
	*	@param y attribute y of the vector.
	*	@param z attribute z of the vector.
	*/
	Vector3(float x, float y, float z);
	/**
	*	Initializes vector from array containing 3 elements. First one will be assigned to x, second one to the y, third one to the z.
	*
	*	@param vector array containing three elements.
	*/
	Vector3(const float *vector);
	/**
	*	Copy constructor.
	*
	*	@param source vector to be copied.
	*/
	Vector3(const Vector3& source);

	/**
	*	Copy operator.
	*
	*	@param source vector to be copied.
	*	@return Reference to the object calling this operator.
	*/
	Vector3&	operator=(const Vector3& source);
	/**
	*	Copy given value to the vector.
	*
	*	@param value value copied to the vector.
	*	@return Reference to the object calling this operator.
	*/
	Vector3&	operator=(float value);

	/**
	*	Multiply each element of the vector by given value.
	*
	*	@param value value for multiplication.
	*	@return Reference to the object calling this operator.
	*/
	Vector3&	operator*=(float value);

	/**
	*	Add to each parameter of the vector given value.
	*
	*	@param value value to be added
	*	@return Reference to the object calling this operator.
	*/
	Vector3&	operator+=(float value);
	/**
	*	Add two vectors, adding coresponding values to each other.
	*
	*	@param vector vector to be added
	*	@return Reference to the object calling this operator.
	*/
	Vector3&	operator+=(const Vector3& vector);

	/**
	*	Subtract from each parameter of the vector given value.
	*
	*	@param value value to be subtracted
	*	@return Reference to the object calling this operator.
	*/
	Vector3&	operator-=(float value);
	/**
	*	Subtract two vectors, subtracting coresponding values to each other.
	*
	*	@param vector vector to be subtracted
	*	@return Reference to the object calling this operator.
	*/
	Vector3&	operator-=(const Vector3& vector);

	/**
	*	Divide each element of the vector by given value.
	*
	*	@param value value for division.
	*	@return Reference to the object calling this operator.
	*/
	Vector3&	operator/=(float value);

	/**
	*	Operator allowing to treat vector as an array containing 3 elements.
	*	Index 0 is x or r, index 1 is y or g, index 2 is z or b.
	*
	*	@param item index of the item from 0-2.
	*	@return Reference to the specified parameter.
	*/
	float&		operator[](int item);
	/**
	*	Operator allowing to treat vector as an array containing 3 elements.
	*	Index 0 is x or r, index 1 is y or g, index 2 is z or b.
	*
	*	@param item index of the item from 0-2.
	*	@return Const reference to the specified parameter.
	*/
	const float& operator[](int item) const;
	
	/**
	*	Converts vector to an array of 3 elements.
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
	*	@return Reference to the object calling this method.
	*/
	Vector3&	Set(float nx, float ny, float nz);
	/**
	*	Set new values to the vector.
	*
	*	@param vectorArray array of two elements containing new values for x, y and z.
	*	@return Reference to the object calling this method.
	*/
	Vector3&	Set(const float *vectorArray);

	/**
	*	Normalize vector.
	*
	*	@return Reference to the object calling this method.
	*/
	Vector3&	Normalize();

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
	*	Format vector to the string. Format output: (x, y, z) or in another convention (r, g, b).
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
	static Vector3	Normalized(const Vector3& vector);

	/**
	*	Get result of cross operator on two vectors. Result vector is perpendicular to the plane presented
	*	by two input vectors.
	*
	*	@param first vector standing on the left side during cross operator of the vectors.
	*	@param second vector standing on the right side during cross operator of the vectors.
	*	@return Cross operator result.
	*/
	static Vector3	Cross(const Vector3& first, const Vector3& second);
	/**
	*	Get dot product of two vectors.
	*
	*	@param first first vector.
	*	@param second second vector.
	*	@return Dot product of two vectors.
	*/
	static float	Dot(const Vector3& first, const Vector3& second);

	/**
	*	Get angle between two vectors. Angle is returned in degrees.
	*
	*	@param start first vector for measurement.
	*	@param end second vector for measurement.
	*	@return Angle between two vectors in degrees.
	*/
	static float	Angle(const Vector3& start, const Vector3& end);
	/**
	*	Get distance between two vectors.
	*
	*	@param start starting point.
	*	@param end ending point.
	*	@return Distance between start and end.
	*/
	static float	Distance(const Vector3& start, const Vector3& end);

	/**
	*	Lineary interpolate between two vectors. Result will be clamped to be between start and end vectors.
	*
	*	@param start starting vector.
	*	@param end ending vector
	*	@param t interpolation parameter, where 0 means start vector and 1 means end vector. Will be clamped to be in range [0; 1].
	*	@return Interpolated vector.
	*/
	static Vector3	Lerp(const Vector3& start, const Vector3& end, float t);
	/**
	*	Lineary interpolate between two vectors.
	*
	*	@param start starting vector.
	*	@param end ending vector
	*	@param t interpolation parameter, where 0 means start vector and 1 means end vector.
	*	@return Interpolated vector.
	*/
	static Vector3	LerpUnclamped(const Vector3& start, const Vector3& end, float t);

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
	static Vector3	Slerp(const Vector3& start, const Vector3& end, float t);
	/**
	*	Spherical linear interpolation between two vectors. In linear interpolation, vector interpolates through sphere.
	*	When using Slerp, vector interpolates through sphere, so in 2D plane resulting move will be an arc, not line.
	*
	*	@param start starting vector.
	*	@param end ending vector
	*	@param t interpolation parameter, where 0 means start vector and 1 means end vector.
	*	@return Interpolated vector.
	*/
	static Vector3	SlerpUnclamped(const Vector3& start, const Vector3& end, float t);

	/**
	*	Results in vector which is closer to the end vector by distanceDelta units.
	*	If distance is below 0, resulting vector will further from end vector by distanceDelta units.
	*
	*	@param current vector repressnting current position.
	*	@param end ending vector.
	*	@param distanceDelta maximum distance that can be moved.
	*	@return Vector which is closer or further from end vector.
	*/
	static Vector3	MoveTowards(const Vector3& current, const Vector3& end, float distanceDelta);
	/**
	*	Rotates vector by maximum specified radians towards given destination. It length of current vector is different than length of the ending vector,
	*	length will be interpolated too by maximum given magnitude.
	*
	*	@param current vector repressnting current position.
	*	@param end ending vector.
	*	@param distanceDelta maximum distance that can be moved.
	*	@return Vector rotated towards end by maximum of radiansDelta and which magnitude is closer by maximum of manitudeDelta.
	*/
	static Vector3	RotateTowards(const Vector3& current, const Vector3& end, float radiansDelta, float magnitudeDelta);

	/**
	*	Interpolates vector through sphere but without constant speed. Will result in normalized vector.
	*	Resulting vector will be clamped to be between start and end.
	*
	*	@param start starting vector.
	*	@param end ending vector
	*	@param t interpolation parameter, where 0 means start vector and 1 means end vector. Will be clamped to be in range [0; 1].
	*	@return Interpolated and normalized vector.
	*/
	static Vector3	Nlerp(const Vector3& start, const Vector3& end, float t);
	/**
	*	Interpolates vector through sphere but without constant speed. Will result in normalized vector.
	*
	*	@param start starting vector.
	*	@param end ending vector
	*	@param t interpolation parameter, where 0 means start vector and 1 means end vector.
	*	@return Interpolated and normalized vector.
	*/
	static Vector3	NlerpUnclamped(const Vector3& start, const Vector3& end, float t);

	static const Vector3 ones;		/* Vector filled with ones.								*/
	static const Vector3 zeroes;	/* Vector filled with zeroes.							*/

	static const Vector3 forward;	/* Vector representing forward direction (-z axis).		*/
	static const Vector3 backward;	/* Vector representing backward direction (+z axis).	*/
	static const Vector3 up;		/* Vector representing up direction (+y axis).			*/
	static const Vector3 down;		/* Vector representing down direction (-y axis).		*/
	static const Vector3 right;		/* Vector representing right direction (+x axis).		*/
	static const Vector3 left;		/* Vector representing left direction (-x axis).		*/

	/*
	*	Vector3 can be described in many ways: as x, y, z 3D coordinates,
	*	as r, g, b color parameters and as three elements array.
	*/
	union {
		float atr[3];
		struct {
			float x;
			float y;
			float z;
		};
		struct {
			float r;
			float g;
			float b;
		};
	};
};

Vector3	operator-(const Vector3& vector);
Vector3	operator-(const Vector3& first, const Vector3& second);
Vector3	operator-(const Vector3& vector, float value);
Vector3	operator+(const Vector3& first, const Vector3& second);
Vector3	operator/(const Vector3& first, float value);
Vector3	operator*(const Vector3& first, const Vector3& second);
Vector3	operator*(float value, const Vector3& vector);
Vector3	operator*(const Vector3& vector, float value);
bool	operator==(const Vector3& first, const Vector3& second);
bool	operator!=(const Vector3& first, const Vector3& second);
std::ostream& operator<<(std::ostream &os, const Vector3& vector);

} //vengine
