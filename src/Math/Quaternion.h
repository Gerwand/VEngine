#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include "MathFunctions.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "MathFunctions.h"

namespace vengine {

/**
*	Class representing quaternions and providing basic quaternion operations and interpolation methods.
*/
class Quaternion {
public:
	/**
	*	Default constructor doing nothing.
	*/
	Quaternion();
	/**
	*	Copy constructor.
	*
	*	@param source other quaternion to be copied to the new one.
	*/
	Quaternion(const Quaternion& source);
	/**
	*	Initializes quaternion with x, y, z and w value.
	*
	*	@param x element x of the quaternion.
	*	@param y element y of the quaternion.
	*	@param z element z of the quaternion.
	*	@param w element w of the quaternion.
	*/
	Quaternion(float x, float y, float z, float w);
	/**
	*	Initializes quaternion with r parameter and vector contatining x, y and z elements.
	*
	*	@param v vector containing x, y and z values.
	*	@param r element r of the quaternion.
	*/
	Quaternion(const Vector3& v, float r);
	/**
	*	Initializes quaternion from vector, simply coping each corresponding value to quaternion.
	*
	*	@param source vector x, y, z and w values for quaternion (in that order).
	*/
	Quaternion(const Vector4& source);

	/**
	*	Copy operator.
	*
	*	@param source quaternion to be copied.
	*	@return Reference to the object calling this operator.
	*/
	Quaternion& operator=(const Quaternion& source);
	/**
	*	Copies values from vector, simply coping each corresponding value to quaternion.
	*
	*	@param source vector x, y, z and w values for quaternion (in that order).
	*	@return Reference to the object calling this operator.
	*/
	Quaternion& operator=(const Vector4& q);

	/**
	*	Multiply two quaternions and store value in the left one (quaternion calling this operator).
	*
	*	@param quaternionR the quaternion standing on the right side in the quaternion multiplication.
	*	@return Reference to the object calling this operator.
	*/
	Quaternion& operator*=(const Quaternion& quaternionR);

	/**
	*	Treat quaternion as an array and get corresponding item.
	*	Elements goes in that order: {w, x, y, z}.
	*
	*	@param item index of the element, from 0 to 3.
	*	@return reference to the corresponding quaternion element.
	*/
	float& operator[](int item);
	/**
	*	Treat quaternion as an array and get corresponding item.
	*	Elements goes in that order: {w, x, y, z}.
	*
	*	@param item index of the element, from 0 to 3.
	*	@return const reference to the corresponding quaternion element.
	*/
	const float& operator[](int item) const;

	/**
	*	Set quaternion parameters.
	*
	*	@param v vector containing parameters x, y, z
	*	@param r value assigned to parameter w or r (depending on naming)
	*/
	Quaternion& Set(const Vector3& v, float r);
	/**
	*	Set quaternion parameters.
	*
	*	@param x parameter x of the quaternion.
	*	@param y parameter y of the quaternion.
	*	@param z parameter z of the quaternion.
	*	@param w parameter w of the quaternion.
	*	@return Reference to the object calling this method.
	*/
	Quaternion& Set(float x, float y, float z, float w);

	/**
	*	Set to the quaternion rotation around given axis. Axis must be normalized.
	*
	*	@param angle angle in degrees.
	*	@param axis normalized axis of the rotation.
	*	@return Reference to the object calling this method.
	*/
	Quaternion& SetRotation(float angle, const Vector3& axis);
	/**
	*	Set to the quaternion rotation around given axis. Axis must be normalized.
	*
	*	@param angle angle in degrees.
	*	@param x coordinate x of the normalized axis
	*	@param y coordinate y of the normalized axis
	*	@param z coordinate z of the normalized axis
	*	@return Reference to the object calling this method.
	*/
	Quaternion& SetRotation(float angle, float x, float y, float z);

	/**
	*	Set to the quaternion rotation around axis X.
	*
	*	@param angle angle in degrees.
	*	@return Reference to the object calling this method.
	*/
	Quaternion& SetRotationX(float angle);
	/**
	*	Set to the quaternion rotation around axis Y.
	*
	*	@param angle angle in degrees.
	*	@return Reference to the object calling this method.
	*/
	Quaternion& SetRotationY(float angle);
	/**
	*	Set to the quaternion rotation around axis Z.
	*
	*	@param angle angle in degrees.
	*	@return Reference to the object calling this method.
	*/
	Quaternion& SetRotationZ(float angle);

	/**
	*	Set to the quaternion Euler rotation. Order of the rotations: X, Y, Z.
	*
	*	@param x rotation around x axis in degrees.
	*	@param y rotation around y axis in degrees.
	*	@param z rotation around z axis in degrees.
	*	@return Reference to the object calling this method.
	*/
	Quaternion& SetRotationEuler(float x, float y, float z);
	/**
	*	Set to the quaternion Euler rotation. Order of the rotations: X, Y, Z.
	*
	*	@param angles vector containing rotations in degrees around x, y and z axes
	*	@return Reference to the object calling this method.
	*/
	Quaternion& SetRotationEuler(const Vector3& angles);

	/**
	*	Calculates and returns axis of the rotation stored in quaternion.
	*
	*	@return Axis of the rotation stored in quaternion.
	*/
	Vector3 GetAxis();
	/**
	*	Calculates and returns angle of the rotation stored in quaternion.
	*
	*	@return Angle in degrees of the rotation stored in quaternion.
	*/
	float GetAngle() const;
	/**
	*	Calculates and returns angle and axis of the rotation stored in quaternion.
	*
	*	@param angle pointer to the value in which angle of the rotation in degrees will be stored.
	*	@param axis pointer to the vector in which axes of the rotation will be stored.
	*/
	void GetAngleAndAxis(float* angle, Vector3* axis);
	/**
	*	Calculates and returns angles in degrees represanting rotation as Euler rotations. Order of the rotations: X, Y, Z.
	*
	*	@return Vector containing rotations in degrees around X, Y and Z axes.
	*/
	Vector3 GetEulerAngles() const;
	
	/**
	*	Normalize quaternion. Magnitude of the normalized quaternion is equal to 1.
	*
	*	@return Reference to the object calling this method.
	*/
	Quaternion& Normalize();
	/**
	*	Conjugate quaternion. If quaternion is normalized, conjugating quaternion is equal to inversing it's rotation.
	*
	*	@return Reference to the object calling this method.
	*/
	Quaternion& Conjugate();
	/**
	*	Inverse quaternion. If quaternion is not normalized this should be used to inverse the rotation described by it.
	*
	*	@return Reference to the object calling this method.
	*/
	Quaternion& Inverse();

	/**
	*	Operator converting quaternion to matrix describing it's rotation.
	*
	*	@return Matrix describing the same rotation as quaternion.
	*/
	operator Matrix4() const;

	/**
	*	Convert quaternion to matrix describing it's rotation.
	*
	*	@return Matrix describing the same rotation as quaternion.
	*/
	Matrix4 ToMatrix() const;
	/**
	*	Format quaternion to string. Output format is (<v>, r), what is equal to: (<x, y, z>, w)
	*
	*	@return std::string object containing formated quaternion parameters.
	*/
	std::string ToString() const;

	/**
	*	Get normalized quaternion. Magnitude of the normalized quaternion is equal to 1.
	*
	*	@param quaternion reference to the quaternion that is going to be normalized.
	*	@return Normalized quaternion.
	*/
	static Quaternion Normalized(const Quaternion& quaternion);
	/**
	*	Get conjugated quaternion. If quaternion is normalized, conjugating quaternion is equal to inversing it's rotation.
	*
	*	@param quaternion reference to the quaternion that is going to be normalized.
	*	@return Conjugated quaternion.
	*/
	static Quaternion Conjugated(const Quaternion& quaternion);

	/**
	*	Get logarithm of the quaternion.
	*
	*	@param quaternion reference to the logarithmized quaternion
	*	@return Logarithm of the quaternion.
	*/
	static Quaternion Log(const Quaternion& quaternion);
	/**
	*	Get exponent of the quaternion.
	*
	*	@param quaternion reference to the quaternion.
	*	@return Exponent of the quaternion.
	*/
	static Quaternion Exp(const Quaternion& quaternion);
		
	/**
	*	Lineary interpolate rotation. This interpolation is fast, however speed of the rotation is not constant.
	*
	*	@param start quaternion describing beginning of the rotation.
	*	@param end quaternion describing end of the rotation.
	*	@param t value describing interpolation of the rotation, where 0 is start rotation, 1 is end rotation.
	*	@return Interpolated rotation.
	*/
	static Quaternion Lerp(const Quaternion& start, const Quaternion& end, float t);
	/**
	*	Spherical linear interpolation by smallest angle. Gives constant and best results to rotate from a to b.
	*	It is not the best choice to use it to interpolate consecutive rotations, because result will be very sharp.
	*
	*	@param start quaternion describing beginning of the rotation.
	*	@param end quaternion describing end of the rotation.
	*	@param t value describing interpolation of the rotation, where 0 is start rotation, 1 is end rotation.
	*	@return Interpolated rotation.
	*/
	static Quaternion Slerp(const Quaternion& start, const Quaternion& end, float t);
	/**
	*	Spherical linear interpolation, does not rotate by smallest angle. Gives constant and best results to rotate from a to b.
	*	It is not the best choice to use it to interpolate consecutive rotations, because result will be very sharp.
	*
	*	@param start quaternion describing beginning of the rotation.
	*	@param end quaternion describing end of the rotation.
	*	@param t value describing interpolation of the rotation, where 0 is start rotation, 1 is end rotation.
	*	@return Interpolated rotation.
	*/
	static Quaternion SlerpNoInversion(const Quaternion& start, const Quaternion& end, float t);
	/**
	*	Generates spline for consecutive rotations. It can be later used in squad interpolation.
	*	Current is i rotation, previous is i-1 rotation and next is i+1 rotation.
	*	
	*	@param previous quaternion representing i-1 rotation.
	*	@param previous quaternion representing i rotation.
	*	@param previous quaternion representing i+1 rotation.
	*	@return spline for those rotations.
	*/
	static Quaternion Spline(const Quaternion& previous, const Quaternion& current, const Quaternion& next);
	/**
	*	Spherical quadrangle interpolation. It can be used to smoothly itnerpolate multiple consecutive rotations.
	*
	*	@param start quaternion describing beginning of the rotation.
	*	@param end quaternion describing end of the rotation.
	*	@param start_control control point for start rotation, generated from Spline function.
	*	@param start_control control point for end rotation, generated from Spline function.
	*	@return Interpolated rotation.
	*/
	static Quaternion Squad(const Quaternion& start, const Quaternion& end,
							const Quaternion& start_control, const Quaternion& end_control,
							float t);

	static const Quaternion identity; /* Identity quaternion, describes no rotation */
	
	static void RotatePoint(const Quaternion& rotation, Vector3* vector);

	/**
	*	Ways of representating quaternion. It can be treated like one vector of 4 elements.
	*	It can also be treated as four elements: x, y, z and w (this order will be used in whole quaternion code, to 
	*	make it compatible with structure of the class Vector4).
	*	Finally it can be treated as Vector3(x, y, z) and value r.
	*	It can help do some quaternion operations and makes this class more universal.
	*/
	union {
		Vector4 q;
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		struct {
			Vector3 v;
			float r;
		};
	};
};


/**
*	Multiply two quaternions.
*
*	@param quaternionL quaternion standing on the left side in the quaternion multiplication.
*	@param quaternionR quaternion standing on the right side in the quaternion multiplication.
*	@return Result of multiplication.
*/
Quaternion operator*(const Quaternion& quaternionL, const Quaternion& quaternionR);
/**
*	Multiply each element of quaternion by given value.
*
*	@param value value for multiplication
*	@param quaternion quaternion to be multiplied.
*	@return Result of the multiplication.
*/
Quaternion operator*(float value, const Quaternion& quaternion);
/**
*	Multiply each element of quaternion by given value.
*
*	@param quaternion quaternion to be multiplied.
*	@param value value for multiplication
*	@return Result of the multiplication.
*/
Quaternion operator*(const Quaternion& quaternion, float value);

/**
*	Divide each element of quaternion by given value.
*
*	@param quaternion quaternion to be divided.
*	@param value value for division.
*	@return Result of the division.
*/
Quaternion operator/(const Quaternion& quaternion, float value);

/**
*	Change each attribute of the quaternion to the oposite one.
*
*	@param quaternion quaternion used for operation.
*	@return Quaternion witch each value being opposite.
*/
Quaternion operator-(const Quaternion& quaternion);
/**
*	Subtract two quaternions.
*
*	@param quaternion1 quaternion from which the other one will be subtracted.
*	@param quaternion2 subtracted quaternion.
*	@return Result of the subtract.
*/
Quaternion operator-(const Quaternion& quaternion1, const Quaternion& quaternion2);
/**
*	Add two quaternions.
*
*	@param quaternion1 first quaternion for sum.
*	@param quaternion2 second quaternion for sum.
*	@return Sum of quaternions.
*/
Quaternion operator+(const Quaternion& quaternion1, const Quaternion& quaternion2);

/**
*	Check if two quaternions are equal.
*
*	@param quaternion1 first quaternion.
*	@param quaternion2 second quaternion.
*	@return True if quaternions are equal, false if not.
*/
bool operator==(const Quaternion& quaternion1, const Quaternion& quaternion2);
/**
*	Check if two quaternions are not equal.
*
*	@param quaternion1 first quaternion.
*	@param quaternion2 second quaternion.
*	@return True if quaternions are not equal, false if they are.
*/
bool operator!=(const Quaternion& quaternion1, const Quaternion& quaternion2);

/**
*	Overload operator << for use in std::cout to conver quaternion to string.
*
*	@param os reference to ostream stream.
*	@param quaternion quaternion for conversion
*	@return stream with added string.
*/
std::ostream& operator<<(std::ostream &os, const Quaternion& quaternion);
}
