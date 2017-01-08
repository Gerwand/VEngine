#include "Vector3.h"
#include "Assert.h"

namespace vengine {
/**************************************************************************
*
*								Static constants
*
***************************************************************************/
Vector3 const Vector3::ones(1, 1, 1);
Vector3 const Vector3::zeroes(0, 0, 0);

Vector3 const Vector3::forward(0, 0, -1);
Vector3 const Vector3::backward(0, 0, 1);

Vector3 const Vector3::up(0, 1, 0);
Vector3 const Vector3::down(0, -1, 0);

Vector3 const Vector3::right(1, 0, 0);
Vector3 const Vector3::left(-1, 0, 0);


/**************************************************************************
*
*								Constructors
*
***************************************************************************/
Vector3::Vector3()
{
}

Vector3::Vector3(float value) : x(value), y(value), z(value)
{
}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
{
}

Vector3::Vector3(const float *vector) :
	x(vector[0]), y(vector[1]), z(vector[2])
{
}

Vector3::Vector3(const Vector3& source) :
	x(source.x), y(source.y), z(source.z)
{
}

Vector3::Vector3(const Vector2& source, float z) :
	x(source.x), y(source.y), z(z)
{
}
/**************************************************************************
*
*								  Operators
*
***************************************************************************/
/**************************************************************************
*								Operators =
***************************************************************************/
Vector3&	
Vector3::operator=(const Vector3& source)
{
	for (int i = 0; i < 3; ++i)
		atr[i] = source[i];

	return *this;
}

Vector3&
Vector3::operator=(float value)
{
	for (int i = 0; i < 3; ++i)
		atr[i] = value;

	return *this;
}
/**************************************************************************
*						Operators with single value
***************************************************************************/

Vector3&	
Vector3::operator*=(float value)
{
	for (int i = 0; i < 3; ++i)
		atr[i] *= value;

	return *this;
}

Vector3&
Vector3::operator+=(float value)
{
	for (int i = 0; i < 3; ++i)
		atr[i] += value;

	return *this;
}

Vector3&
Vector3::operator-=(float value)
{
	for (int i = 0; i < 3; ++i)
		atr[i] -= value;

	return *this;
}

Vector3&
Vector3::operator/=(float value)
{
	assert(value != 0.0f, "Divide /= Vector3 0");
	for (int i = 0; i < 3; ++i)
		atr[i] /= value;

	return *this;
}

/**************************************************************************
*							Operators with vector
***************************************************************************/

Vector3&	
Vector3::operator+=(const Vector3& vector)
{
	for (int i = 0; i < 3; ++i)
		atr[i] += vector[i];

	return *this;
}

Vector3&	
Vector3::operator-=(const Vector3& vector)
{
	for (int i = 0; i < 3; ++i)
		atr[i] -= vector[i];

	return *this;
}

/**************************************************************************
*								Get operators
***************************************************************************/
float&
Vector3::operator[](int item)
{
	assert(item < 3 && item >= 0, "Vector 3 [] index out of bound");
	return atr[item];
}

const float& 
Vector3::operator[](int item) const
{
	assert(item < 3 && item >= 0, "Vector 3 [] index out of bound");
	return atr[item];
}



/**************************************************************************
*							Conversion operators
***************************************************************************/
Vector3::operator float*()
{
	return atr;
}

Vector3::operator const float*() const
{
	return atr;
}

/**************************************************************************
*
*								  Setters
*
***************************************************************************/

Vector3& 
Vector3::Set(float nx, float ny, float nz)
{
	x = nx;
	y = ny;
	z = nz;

	return *this;
}

Vector3&
Vector3::Set(const float *vectorArray)
{
	for (int i = 0; i < 3; ++i)
		atr[i] = vectorArray[i];

	return *this;
}

/**************************************************************************
*
*							Class methods
*
***************************************************************************/

Vector3&
Vector3::Normalize()
{
	float mag = Magnitude();
	assert(mag != 0.0f, "Vector3 normalize - mag = 0");
	return *this /= mag;
}

float
Vector3::Magnitude() const
{
	return sqrt(x*x + y*y + z*z);
}

float
Vector3::MagnitudeFast() const
{
	return x*x + y*y + z*z;
}

std::string
Vector3::ToString() const
{
	std::string out("(");
	out += std::to_string(x);
	out += ", ";
	out += std::to_string(y);
	out += ", ";
	out += std::to_string(z);
	out += ")";

	return out;
}


/**************************************************************************
*
*							Static methods
*
***************************************************************************/

/**************************************************************************
*							Vector operations
***************************************************************************/

Vector3	
Vector3::Normalized(const Vector3& vector)
{
	Vector3 result = vector;
	return result.Normalize();
}

Vector3
Vector3::Cross(const Vector3& first, const Vector3& second) 
{
	return Vector3( first[1] * second[2] - second[1] * first[2],
					first[2] * second[0] - second[2] * first[0],
					first[0] * second[1] - second[0] * first[1]);
}


float	
Vector3::Dot(const Vector3& first, const Vector3& second)
{
	return first.x * second.x + first.y * second.y + first.z * second.z;
}


float
Vector3::Angle(const Vector3& start, const Vector3& end)
{
	Vector3 startNorm = Normalized(start);
	Vector3 endNorm = Normalized(end);
	float rads = acos(Dot(startNorm, endNorm));
	return radToDeg(rads);
}

float
Vector3::Distance(const Vector3& start, const Vector3& end)
{
	return Vector3(end - start).Magnitude();
}

Vector3 
Vector3::Lerp(const Vector3& start, const Vector3& end, float t)
{
	//Clamp to give more precise result
	t = clamp(t, 0.0f, 1.0f);
	//Fast multiply XY and add Z
	return LerpUnclamped(start, end, t);
}

Vector3
Vector3::LerpUnclamped(const Vector3& start, const Vector3& end, float t)
{
	return Vector3(fma(t, end[0] - start[0], start[0]),
				   fma(t, end[1] - start[1], start[1]),
				   fma(t, end[2] - start[2], start[2]));
}

Vector3
Vector3::Slerp(const Vector3& start, const Vector3& end, float t)
{
	//Clamp to give more precise result
	clamp(t, 0.0f, 1.0f);

	return SlerpUnclamped(start, end, t);
}

Vector3
Vector3::Nlerp(const Vector3& start, const Vector3& end, float t)
{
	return Lerp(start, end, t).Normalize();
}

Vector3
Vector3::NlerpUnclamped(const Vector3& start, const Vector3& end, float t)
{
	return LerpUnclamped(start, end, t).Normalize();
}

Vector3
Vector3::SlerpUnclamped(const Vector3& start, const Vector3& end, float t)
{
	Vector3 normStart = Normalized(start);
	Vector3 normEnd = Normalized(end);
	float dotProduct = Dot(normStart, normEnd);
	float rads = acos(dotProduct)*t;
	Vector3 relative = normEnd - normStart*dotProduct;
	relative.Normalize();
	return (normStart * cos(rads)) + (relative * sin(rads));
}

Vector3 
Vector3::MoveTowards(const Vector3& current, const Vector3& end, float distanceDelta)
{
	float dist = Distance(current, end);
	//Almost 0
	if (dist <= 0.0001f)
		return end;

	float t = distanceDelta > dist ? 1.0f : distanceDelta / dist;
	//Must use unclamped, because we want to be able to move from object when delta < 0
	return LerpUnclamped(current, end, t);
}

Vector3 
Vector3::RotateTowards(const Vector3& current, const Vector3& end, float radiansDelta,
					   float magnitudeDelta)
{
	Vector3 normStart = Normalized(current);
	Vector3 normEnd = Normalized(end);

	float rads = acos(Dot(current, end));
	if (rads <= 0.0001f)
		return end;

	float t = radiansDelta / rads;
	Vector3 result = SlerpUnclamped(current, end, t);
	float magS = current.Magnitude();
	float magE = end.Magnitude();
	result *= magE - magS < magnitudeDelta ? magE - magS : magnitudeDelta;
	return result;
}


/**************************************************************************
*
*							Global operators
*
***************************************************************************/
Vector3	
operator-(const Vector3& vector)
{
	return Vector3(-vector.x, -vector.y, -vector.z);
}

Vector3
operator-(const Vector3& vector, float value)
{
	return Vector3(vector.x - value, vector.y - value, vector.z - value);
}

Vector3
operator-(const Vector3& first, const Vector3& second)
{
	return Vector3(first.x - second.x, first.y - second.y, first.z - second.z);
}

Vector3
operator+(const Vector3& first, const Vector3& second)
{
	return Vector3(first.x + second.x, first.y + second.y, first.z + second.z);
}

Vector3
operator+(const Vector3& vector, float value)
{
	return Vector3(vector.x + value, vector.y + value, vector.z + value);
}

Vector3
operator/(const Vector3& vector, float value)
{
	assert(value != 0.0f, "Divide / Vector3 0");
	return Vector3(vector.x / value, vector.y / value, vector.z / value);
}

Vector3
operator*(const Vector3& first, const Vector3& second)
{
	return Vector3(first.x * second.x, first.y * second.y, first.z * second.z);
}

Vector3
operator*(const Vector3& vector, float value)
{
	return Vector3(vector.x * value, vector.y * value, vector.z * value);
}

Vector3
operator*(float value, const Vector3& vector)
{
	return Vector3(vector.x * value, vector.y * value, vector.z * value);
}

bool
operator==(const Vector3& first, const Vector3& second)
{
	return (first.x == second.x) && (first.y == second.y) && (first.z == second.z);
}

bool
operator!=(const Vector3& first, const Vector3& second)
{
	return (first.x != second.x) || (first.y != second.y) || (first.z != second.z);
}

std::ostream&
operator<<(std::ostream &os, const Vector3& vector)
{
	return os << vector.ToString();
}

} //vengine