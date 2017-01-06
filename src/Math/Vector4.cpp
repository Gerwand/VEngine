#include "Vector4.h"
#include "Assert.h"

namespace vengine {
/**************************************************************************
*
*								Static constants
*
***************************************************************************/
Vector4 const Vector4::ones(1, 1, 1, 1);
Vector4 const Vector4::zeroes(0, 0, 0, 0);


/**************************************************************************
*
*								Constructors
*
***************************************************************************/

Vector4::Vector4()
{
}


Vector4::Vector4(float value) : x(value), y(value), z(value), w(value)
{

}

Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
{
}

Vector4::Vector4(const float *vector) :
	x(vector[0]), y(vector[1]), z(vector[2]), w(vector[3])
{
}

Vector4::Vector4(const Vector4& source) :
	x(source.x), y(source.y), z(source.z), w(source.w)
{
}

Vector4::Vector4(const Vector3& source, float w) :
	x(source.x), y(source.y), z(source.z), w(w)
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
Vector4&
Vector4::operator=(const Vector4& source)
{
	for (int i = 0; i < 4; ++i)
		atr[i] = source[i];

	return *this;
}

Vector4&
Vector4::operator=(float value)
{
	for (int i = 0; i < 4; ++i)
		atr[i] = value;

	return *this;
}
/**************************************************************************
*						Operators with single value
***************************************************************************/

Vector4&
Vector4::operator*=(float value)
{
	for (int i = 0; i < 4; ++i)
		atr[i] *= value;

	return *this;
}

Vector4&
Vector4::operator+=(float value)
{
	for (int i = 0; i < 4; ++i)
		atr[i] += value;

	return *this;
}

Vector4&
Vector4::operator-=(float value)
{
	for (int i = 0; i < 4; ++i)
		atr[i] -= value;

	return *this;
}

Vector4&
Vector4::operator/=(float value)
{
	assert(value != 0.0f, "Divide /= Vector3 0");
	for (int i = 0; i < 4; ++i)
		atr[i] /= value;

	return *this;
}

/**************************************************************************
*							Operators with vector
***************************************************************************/

Vector4&
Vector4::operator+=(const Vector4& vector)
{
	for (int i = 0; i < 4; ++i)
		atr[i] += vector[i];

	return *this;
}

Vector4&
Vector4::operator-=(const Vector4& vector)
{
	for (int i = 0; i < 4; ++i)
		atr[i] -= vector[i];

	return *this;
}

/**************************************************************************
*								Get operators
***************************************************************************/
float&
Vector4::operator[](int item)
{
	assert(item < 4 && item >= 0, "Vector 4 [] index out of bound");
	return atr[item];
}

const float&
Vector4::operator[](int item) const
{
	assert(item < 4 && item >= 0, "Vector 4 [] index out of bound");
	return atr[item];
}

/**************************************************************************
*							Conversion operators
***************************************************************************/
Vector4::operator float*()
{
	return atr;
}

Vector4::operator const float*() const
{
	return atr;
}
/**************************************************************************
*
*								  Setters
*
***************************************************************************/

Vector4&
Vector4::Set(float nx, float ny, float nz, float nw)
{
	x = nx;
	y = ny;
	z = nz;
	w = nw;

	return *this;
}

Vector4&
Vector4::Set(const float *vector)
{
	x = vector[0];
	y = vector[1];
	z = vector[2];
	w = vector[3];

	return *this;
}

Vector4&
Vector4::Set(const Vector3& vector, float w)
{
	x = vector[0];
	y = vector[1];
	z = vector[2];
	w = w;

	return *this;
}
/**************************************************************************
*
*							Class methods
*
***************************************************************************/

Vector4&
Vector4::Normalize()
{
	float mag = Magnitude();
	assert(mag != 0.0f, "Vector4 normalize - mag = 0");
	return *this /= mag;
}

float
Vector4::Magnitude() const
{
	return sqrt(x*x + y*y + z*z + w*w);
}

float		
Vector4::MagnitudeFast() const
{
	return x*x + y*y + z*z + w*w;

}
std::string
Vector4::ToString() const
{
	std::string out("(");
	out += std::to_string(x);
	out += ", ";
	out += std::to_string(y);
	out += ", ";
	out += std::to_string(z);
	out += ", ";
	out += std::to_string(w);
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

Vector4
Vector4::Normalized(const Vector4& vector)
{
	Vector4 result = vector;
	return result.Normalize();
}

float
Vector4::Dot(const Vector4& first, const Vector4& second)
{
	return first.x * second.x + first.y * second.y +
		first.z * second.z + first.w * second.w;
}

float
Vector4::Distance(const Vector4& start, const Vector4& end)
{
	return Vector4(end - start).Magnitude();
}

Vector4
Vector4::Lerp(const Vector4& start, const Vector4& end, float t)
{
	//Clamp to give more precise result
	t = clamp(t, 0.0f, 1.0f);
	//Fast multiply XY and add Z
	return LerpUnclamped(start, end, t);
}

Vector4
Vector4::LerpUnclamped(const Vector4& start, const Vector4& end, float t)
{
	return Vector4(fma(t, end.x - start.x, start.x),
				   fma(t, end.y - start.y, start.y),
				   fma(t, end.z - start.z, start.z),
				   fma(t, end.w - start.w, start.w));
}	

Vector4
Vector4::MoveTowards(const Vector4& current, const Vector4& end, float distanceDelta)
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
Vector4
operator-(const Vector4& vector)
{
	return Vector4(-vector.x, -vector.y, -vector.z, -vector.w);
}

Vector4 operator-(const Vector4 & vector, float value)
{
	return Vector4(vector.x - value, vector.y - value, vector.z - value, vector.w - value);
}

Vector4
operator-(const Vector4& first, const Vector4& second)
{
	return Vector4(first.x - second.x, first.y - second.y,
				   first.z - second.z, first.w - second.w);
}

Vector4
operator+(const Vector4& first, const Vector4& second)
{
	return Vector4(first.x + second.x, first.y + second.y,
				   first.z + second.z, first.w + second.w);
}

Vector4
operator+(const Vector4& vector, float value)
{
	return Vector4(vector.x + value, vector.y + value,
				   vector.z + value, vector.w + value);
}

Vector4
operator/(const Vector4& vector, float value)
{
	assert(value != 0.0f, "Divide / Vector4 0");
	return Vector4(vector.x / value, vector.y / value,
				   vector.z / value, vector.w / value);
}

Vector4
operator*(const Vector4& first, const Vector4& second)
{
	return Vector4(first.x * second.x, first.y * second.y,
				   first.z * second.z, first.w * second.w);
}

Vector4
operator*(const Vector4& vector, float value)
{
	return Vector4(vector.x * value, vector.y * value,
				   vector.z * value, vector.w * value);
}

bool
operator==(const Vector4& first, const Vector4& second)
{
	return (first.x == second.x) && (first.y == second.y) &&
		(first.z == second.z) && (first.w == second.w);
}

bool
operator!=(const Vector4& first, const Vector4& second)
{
	return (first.x != second.x) || (first.y != second.y) ||
		(first.z != second.z) || (first.w != second.w);
}

std::ostream&
operator<<(std::ostream &os, const Vector4& vector)
{
	return os << vector.ToString();
}

} //vengine