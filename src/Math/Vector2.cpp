#include "Vector2.h"
#include "Assert.h"

namespace vengine {
/**************************************************************************
*
*								Static constants
*
***************************************************************************/
Vector2 const Vector2::ones(1, 1);
Vector2 const Vector2::zeroes(0, 0);

/**************************************************************************
*
*								Constructors
*
***************************************************************************/

Vector2::Vector2()
{
}

Vector2::Vector2(float value) : x(value), y(value)
{
}

Vector2::Vector2(float x, float y) : x(x), y(y)
{
}

Vector2::Vector2(const float *vector) :
	x(vector[0]), y(vector[1])
{
}

Vector2::Vector2(const Vector2& source) :
	x(source.x), y(source.y)
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
Vector2&
Vector2::operator=(const Vector2& source)
{
	x = source.x;
	y = source.y;

	return *this;
}

Vector2&
Vector2::operator=(float value)
{
	x = value;
	y = value;

	return *this;
}
/**************************************************************************
*						Operators with single value
***************************************************************************/

Vector2&	
Vector2::operator*=(float value)
{
	x *= value;
	y *= value;

	return *this;
}

Vector2&
Vector2::operator+=(float value)
{
	x += value;
	y += value;

	return *this;
}

Vector2&
Vector2::operator-=(float value)
{
	x -= value;
	y -= value;

	return *this;
}

Vector2&
Vector2::operator/=(float value)
{
	assert(value != 0.0f, "Divide /= Vector2 0");
	x /= value;
	y /= value;

	return *this;
}

/**************************************************************************
*							Operators with vector
***************************************************************************/

Vector2&
Vector2::operator+=(const Vector2& vector)
{
	x += vector.x;
	y += vector.y;

	return *this;
}

Vector2&
Vector2::operator-=(const Vector2& vector)
{
	x -= vector.x;
	y -= vector.y;
	
	return *this;
}

/**************************************************************************
*								Get operators
***************************************************************************/
float&
Vector2::operator[](int item)
{
	assert(item < 2 && item >= 0, "Vector 2 [] index out of bound");
	return atr[item];
}

const float&
Vector2::operator[](int item) const
{
	assert(item < 2 && item >= 0, "Vector 2 [] index out of bound");
	return atr[item];
}

/**************************************************************************
*							Conversion operators
***************************************************************************/
Vector2::operator float*()
{
	return atr;
}

Vector2::operator const float*() const
{
	return atr;
}
/**************************************************************************
*
*								  Setters
*
***************************************************************************/

Vector2&
Vector2::Set(float nx, float ny)
{
	x = nx;
	y = ny;

	return *this;
}

Vector2&
Vector2::Set(const float *vector)
{
	x = vector[0];
	y = vector[1];

	return *this;
}

/**************************************************************************
*
*							Class methods
*
***************************************************************************/

Vector2&
Vector2::Normalize()
{
	float mag = Magnitude();
	assert(mag != 0.0f, "Vector2 normalize - mag = 0");
	return *this /= mag;
}

float
Vector2::Magnitude() const
{
	return sqrt(x*x + y*y);
}

float
Vector2::MagnitudeFast() const
{
	return x*x + y*y;
}

std::string
Vector2::ToString() const
{
	std::string out("(");
	out += std::to_string(x);
	out += ", ";
	out += std::to_string(y);
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

Vector2
Vector2::Normalized(const Vector2& vector)
{
	Vector2 result = vector;
	return result.Normalize();
}

float
Vector2::Dot(const Vector2& first, const Vector2& second)
{
	return first.x * second.x + first.y * second.y;
}

float
Vector2::Distance(const Vector2& start, const Vector2& end)
{
	return Vector2(end - start).Magnitude();
}

Vector2
Vector2::Lerp(const Vector2& start, const Vector2& end, float t)
{
	//Clamp to give more precise result
	t = clamp(t, 0.0f, 1.0f);
	//Fast multiply XY and add Z
	return LerpUnclamped(start, end, t);
}

Vector2
Vector2::LerpUnclamped(const Vector2& start, const Vector2& end, float t)
{
	return Vector2(fma(t, end[0] - start[0], start[0]),
				   fma(t, end[1] - start[1], start[1]));
}

Vector2
Vector2::MoveTowards(const Vector2& current, const Vector2& end, float distanceDelta)
{
	float dist = Distance(current, end);
	//Almost 0
	if (dist <= 0.0001f)
		return end;	

	float t = distanceDelta > dist ? 1.0f : distanceDelta / dist;
	//Must use unclamped, because we want to be able to move from object when delta < 0
	return LerpUnclamped(current, end, t);
}

/**************************************************************************
*
*							Global operators
*
***************************************************************************/
Vector2
operator-(const Vector2& vector)
{
	return Vector2(-vector.x, -vector.y);
}

Vector2
operator-(const Vector2& first, const Vector2& second)
{
	return Vector2(first.x - second.x, first.y - second.y);
}

Vector2
operator-(const Vector2& vector, float value)
{
	return Vector2(vector.x - value, vector.y - value);
}

Vector2
operator+(const Vector2& first, const Vector2& second)
{
	return Vector2(first.x + second.x, first.y + second.y);
}

Vector2
operator+(const Vector2& vector, float value)
{
	return Vector2(vector.x + value, vector.y + value);
}

Vector2
operator/(const Vector2& vector, float value)
{
	assert(value != 0.0f, "Divide / Vector2 0");
	return Vector2(vector.x / value, vector.y / value);
}

Vector2
operator*(const Vector2& first, const Vector2& second)
{
	return Vector2(first.x * second.x, first.y * second.y);
}

Vector2
operator*(const Vector2& vector, float value)
{
	return Vector2(vector.x * value, vector.y * value);
}

bool
operator==(const Vector2& first, const Vector2& second)
{
	return (first.x == second.x) && (first.y == second.y);
}

bool
operator!=(const Vector2& first, const Vector2& second)
{
	return (first.x != second.x) || (first.y != second.y);
}

std::ostream& 
operator<<(std::ostream &os, const Vector2& vector)
{
	return os << vector.ToString();
}

} //vengine