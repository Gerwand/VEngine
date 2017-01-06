#include "Quaternion.h"
#include "Assert.h"

namespace vengine {

/**************************************************************************
*
*								Static constants
*
***************************************************************************/
const Quaternion Quaternion::identity = { 0.0f, 0.0f, 0.0f, 1.0f };

/**************************************************************************
*
*								Constructors
*
***************************************************************************/

Quaternion::Quaternion()
{
}


Quaternion::Quaternion(const Quaternion& source)
{
	*this = source;
}


Quaternion::Quaternion(float x, float y, float z, float w)
{
	Set(x, y, z, w);
}


Quaternion::Quaternion(const Vector3& v, float r)
{
	Set(v, r);
}

Quaternion::Quaternion(const Vector4& q) : q(q)
{
}

/**************************************************************************
*
*								Operators
*
***************************************************************************/

/**************************************************************************
*								Operator =
***************************************************************************/

Quaternion&
Quaternion::operator=(const Quaternion& source)
{
	q = source.q;

	return *this;
}

Quaternion&
Quaternion::operator=(const Vector4& q)
{
	this->q = q;

	return *this;
}
/**************************************************************************
*								Operator *=
***************************************************************************/

Quaternion&
Quaternion::operator*=(const Quaternion& quaternionR)
{
	*this = *this * quaternionR;

	return *this;
}


/**************************************************************************
*								Operators []
***************************************************************************/

float&
Quaternion::operator[](int item)
{
	assert(between(item, 0, 3), "Index out of range!");
	return q[item];
}

const float&
Quaternion::operator[](int item) const
{
	assert(between(item, 0, 3), "Index out of range!");
	return q[item];
}

Quaternion::operator Matrix4() const
{
	return ToMatrix();
}

/**************************************************************************
*
*								Methods
*
***************************************************************************/

/**************************************************************************
*								Conversion
***************************************************************************/

std::string
Quaternion::ToString() const
{
	std::string out("(<");
	out += std::to_string(x);
	out += ", ";
	out += std::to_string(y);
	out += ", ";
	out += std::to_string(z);
	out += ">, ";
	out += std::to_string(w);
	out += ")";

	return out;
}

Matrix4
Quaternion::ToMatrix() const
{
	assert(around(q.MagnitudeFast(), 1.0f, 0.01f), "Quaternion - ToMatrix: Quaternion is not normalized");

	float xx = x*x;
	float yy = y*y;
	float zz = z*z;
	float xy = x*y;
	float xz = x*z;
	float yz = y*z;
	float wx = w*x;
	float wy = w*y;
	float wz = w*z;

	return Matrix4(
	{ 1.0f - 2.0f*(yy + zz), 2.0f*(xy + wz), 2.0f*(xz - wy), 0.0f },
	{ 2.0f*(xy - wz), 1.0f - 2.0f*(xx + zz), 2.0f*(yz + wx), 0.0f },
	{ 2.0f*(wy + xz), 2.0f*(yz - wx), 1.0f - 2.0f*(xx + yy), 0.0f },
	{ 0.0f, 0.0f, 0.0f, 1.0f }
	);
}

/**************************************************************************
*								Get functions
***************************************************************************/
Vector3
Quaternion::GetAxis()
{
	float sq = sqrt(1.0f - w*w);

	//If sq is close to 0, axis really doesn't matter
	return sq < 0.0001f ? Vector3(0, 0, 0) : Vector3(x / sq, y / sq, z / sq);
}

float
Quaternion::GetAngle() const
{
	/* Checking for _w will be faster than normalizing */
	assert(between(w, -1.0f, 1.0f), "Quaternion - GetAngle: w is not in <-1, 1>");
	float ac = acosf(w);
	return radToDeg(ac * 2.0f);
}

void
Quaternion::GetAngleAndAxis(float* angle, Vector3* axis)
{
	*angle = GetAngle();
	*axis = GetAxis();
}

//Pitch, Yaw, Roll (taka kolejnosc)
//ZXY
Vector3
Quaternion::GetEulerAngles() const
{
	float x1, x2;
	float y1;
	float z1, z2;

	x1 = 2.0f * (-w*x + y*z);
	x2 = 1.0f - 2.0f*(x*x + y*y);
	y1 = -2.0f * (w*y + x*z);
	z1 = 2.0f * (-w*z + x*y);
	z2 = 1.0f - 2.0f*(y*y + z*z);

	//We must cull y1 value for arcus sinus
	y1 = y1 > 1.0f ? 1.0f : (y1 < -1.0f ? -1.0f : y1);

	Vector3 euler = {
		atan2(x1, x2),
		asin(y1),
		atan2(z1, z2)
	};

	return euler;
}

/**************************************************************************
*								Set functions
***************************************************************************/

Quaternion&
Quaternion::Set(const Vector3& v, float r)
{
	this->r = r;
	this->v = v;
	return *this;
}

Quaternion&
Quaternion::Set(float x, float y, float z, float w)
{
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;

	return *this;
}

Quaternion&
Quaternion::SetRotation(float angle, const Vector3& axis)
{
	assert(around(axis.MagnitudeFast(), 1.0f, 0.01f), "Quaternion - SetRotation(angle, axis): axis not normalized");

	SetRotation(angle, axis.x, axis.y, axis.z);
	return *this;
}

Quaternion&
Quaternion::SetRotation(float angle, float x, float y, float z)
{
	assert(around(Vector3(x, y, z).MagnitudeFast(), 1.0f, 0.01f), "Quaternion - SetRotation(angle, x, y, z): axis not normalized");

	float theta = degToRad(angle / 2.0f);
	w = cos(theta);
	this->x = x*sin(theta);
	this->y = y*sin(theta);
	this->z = z*sin(theta);

	return *this;
}

Quaternion&
Quaternion::SetRotationX(float angle)
{
	float theta = degToRad(angle / 2.0f);
	w = cos(theta);
	x = sin(theta);
	y = 0.0f;
	z = 0.0f;

	return *this;
}

Quaternion&
Quaternion::SetRotationY(float angle)
{
	float theta = degToRad(angle / 2.0f);
	w = cos(theta);
	y = sin(theta);
	x = 0.0f;
	z = 0.0f;

	return *this;
}

Quaternion&
Quaternion::SetRotationZ(float angle)
{
	float theta = degToRad(angle / 2.0f);
	w = cos(theta);
	z = sin(theta);
	x = 0.0f;
	y = 0.0f;

	return *this;
}


//y, z, x
Quaternion&
Quaternion::SetRotationEuler(float x, float y, float z)
{
	x = degToRad(x);
	y = degToRad(y);
	z = degToRad(z);
	float cx = cos(x / 2.0f), cy = cos(y / 2.0f), cz = cos(z / 2.0f);
	float sx = sin(x / 2.0f), sy = sin(y / 2.0f), sz = sin(z / 2.0f);

	this->w = sx*sy*sz + cx*cy*cz;

	/*
	this->x = cy*cz*sz - cx*sy*sz;
	this->y = cx*cz*sy + cy*sx*sz;
	this->z = cx*cy*sz - cz*sx*sy;
	*/

	this->x = sx*cy*cz - cx*sy*sz;
	this->y = cx*sy*cz + sx*cy*sz;
	this->z = cx*cy*sz - sx*sy*cz;


	Normalize();
	return *this;
}

//y, z, x
Quaternion&
Quaternion::SetRotationEuler(const Vector3& angles)
{
	return SetRotationEuler(angles[0], angles[1], angles[2]);
}
/**************************************************************************
*							Quaternion operations
***************************************************************************/
Quaternion&
Quaternion::Normalize()
{
	float n = 1.0f / q.Magnitude();
	q *= n;

	return *this;
}

Quaternion&
Quaternion::Conjugate()
{
	v = -v;

	return *this;
}

Quaternion&
Quaternion::Inverse()
{
	Conjugate();
	Normalize();

	return *this;
}
/**************************************************************************
*
*							Static methods
*
***************************************************************************/

Quaternion
Quaternion::Normalized(const Quaternion& quaternion)
{
	Quaternion result = quaternion;
	result.Normalize();

	return result;
}
Quaternion
Quaternion::Conjugated(const Quaternion& quaternion)
{
	Quaternion result = quaternion;
	result.Conjugate();

	return result;
}

Quaternion
Quaternion::Log(const Quaternion& quaternion)
{
	Quaternion result;
	float angle = quaternion.GetAngle();
	//We need to get sinus for vector v of quaternion, to get only x, y and z coordinate by diving current value
	float sina = sin(angle);
	
	result.w = 0.0f;

	//angle will result in <0; pi/2>, so sina will never be less than 0
	if (sina > 0.0001f)
		result.v = quaternion.v * angle / sina;
	else
		result.v = 0.0f;

	return result;
}

Quaternion
Quaternion::Exp(const Quaternion& quaternion)
{
	//It comes from definition of e^z = e^a*e^v, and angle is equal to |v|
	//and a is equal to 0
	float angle = quaternion.v.Magnitude();
	float sina = sin(angle);
	float cosa = cos(angle);
	Quaternion result;

	result.w = cosa;
	
	if (angle > 0.0f)
		result.v = quaternion.v * sina / angle;
	else
		result.v = 0.0f;

	return result;
}

Quaternion
Quaternion::Lerp(const Quaternion& start, const Quaternion& end, float t)
{
	Quaternion result(
		fma(t, end[0] - start[0], start[0]),
		fma(t, end[1] - start[1], start[1]),
		fma(t, end[2] - start[2], start[2]),
		fma(t, end[3] - start[3], start[3])
	);

	//We must normalize result quaternion
	return result.Normalize();
}

Quaternion
Quaternion::Slerp(const Quaternion& start, const Quaternion& end, float t)
{
	Quaternion endClosest;
	float dotProduct = Vector4::Dot(start.q, end.q);

	//If angle is higher than 90 deg, we can invert rotation to reach destination faster
	if (dotProduct < 0.0f)	{
		dotProduct = -dotProduct;
		endClosest = -end;
	}
	else {
		endClosest = end;
	}


	//For higher angle we want to do slerp (for lower angle we can be too close to 0 with sinus)
	if (dotProduct < 0.95f)	{
		float angle = acos(dotProduct);
		float sina = sin(angle);
		float sinStart = sin(angle * (1.0f - t));
		float sinEnd = sin(angle * t);

		return (start*sinStart + endClosest*sinEnd) / sina;
	}
	else {
		//And linear interpolation for small angle
		return Lerp(start, endClosest, t);
	}
}

Quaternion 
Quaternion::SlerpNoInversion(const Quaternion& start, const Quaternion& end, float t)
{
	float dotProduct = Vector4::Dot(start.q, end.q);

	//For higher angle we want to do slerp (for lower angle we can be too close to 0 with sinus)
	if (between(dotProduct, -0.95f, 0.95f)) {
		float angle = acos(dotProduct);
		float sina = sin(angle);
		float sinStart = sin(angle * (1.0f - t));
		float sinEnd = sin(angle * t);

		return (start*sinStart + end*sinEnd) / sina;
	}
	else {
		//And linear interpolation for small angle
		return Lerp(start, end, t);
	}
}

Quaternion
Quaternion::Spline(const Quaternion& previous, const Quaternion& current, const Quaternion& next)
{
	Quaternion conj = Quaternion::Conjugated(current);

	return current*Quaternion::Exp(
		(Quaternion::Log(conj * previous) + Quaternion::Log(conj * next)) / (-4.0f));
}

Quaternion
Quaternion::Squad(const Quaternion& start, const Quaternion& end,
				  const Quaternion& start_control, const Quaternion& end_control, float t)
{
	Quaternion q1, q2;
	q1 = SlerpNoInversion(start, end, t);
	q2 = SlerpNoInversion(start_control, end_control, t);
	return SlerpNoInversion(q1, q2, 2.0f*t*(1.0f - t));
}

void 
Quaternion::RotatePoint(const Quaternion& rotation, Vector3* vector)
{
	Quaternion vecQuat(*vector, 0.0f);
	vecQuat = rotation * vecQuat;
		vecQuat = vecQuat * Conjugated(rotation);
	vector->Set(vecQuat.x, vecQuat.y, vecQuat.z);
}


/**************************************************************************
*
*							Global operators
*
***************************************************************************/

/**************************************************************************
*								Operator +
***************************************************************************/

Quaternion
operator+(const Quaternion& quaternion1, const Quaternion& quaternion2)
{
	return Quaternion(
		quaternion1.x + quaternion2.x,
		quaternion1.y + quaternion2.y,
		quaternion1.z + quaternion2.z,
		quaternion1.w + quaternion2.w
	);
}

/**************************************************************************
*								Operator -
***************************************************************************/

Quaternion 
operator-(const Quaternion& quaternion)
{
	return Quaternion(-quaternion.q);
}

Quaternion
operator-(const Quaternion& quaternion1, const Quaternion& quaternion2)
{
	return Quaternion(
		quaternion1.x - quaternion2.x,
		quaternion1.y - quaternion2.y,
		quaternion1.z - quaternion2.z,
		quaternion1.w - quaternion2.w
	);
}

/**************************************************************************
*								Operator *
***************************************************************************/

Quaternion 
operator*(const Quaternion& quaternionL, const Quaternion& quaternionR)
{
	//Less performant version
	//return Quaternion(quaternionL.w*quaternionR.w - Vector3::Dot(quaternionL.v, quaternionR.v),
	//				  quaternionR.w*quaternionL.v + quaternionL.w*quaternionR.v +
	//				  Vector3::Cross(quaternionL.v, quaternionR.v));

	const Quaternion& q1 = quaternionL;
	const Quaternion& q2 = quaternionR;

	//Faster version
	return Quaternion(
		q1.y*q2.z - q1.z*q2.y + q1.w*q2.x + q1.x*q2.w,
		q1.z*q2.x - q1.x*q2.z + q1.w*q2.y + q1.y*q2.w,
		q1.x*q2.y - q1.y*q2.x + q1.w*q2.z + q1.z*q2.w,
		q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z
	);
}

Vector3 
operator*(const Quaternion& quaternion, const Vector3& vector)
{
	Vector3 cr = Vector3::Cross(quaternion.v, vector);

	//Optimized version of q*v*q^-1
	return vector + 2 * quaternion.w*cr + 2 * Vector3::Cross(quaternion.v, cr);
}

Quaternion
operator*(const Quaternion& quaternion, float value)
{
	//Less performant version
	//return Quaternion(quaternionL.w*quaternionR.w - Vector3::Dot(quaternionL.v, quaternionR.v),
	//				  quaternionR.w*quaternionL.v + quaternionL.w*quaternionR.v +
	//				  Vector3::Cross(quaternionL.v, quaternionR.v));

	//Faster version
	return Quaternion(
		quaternion.x*value,
		quaternion.y*value,
		quaternion.z*value,
		quaternion.w*value
	);
}


Quaternion 
operator*(float value, const Quaternion& quaternion)
{
	return quaternion*value;
}

/**************************************************************************
*								Operator /
***************************************************************************/

Quaternion
operator/(const Quaternion& quaternion, float value)
{
	assert(value == 0.0f, "Operator / - quat/val: dividing by 0");

	return Quaternion(
		quaternion.x/value,
		quaternion.y/value,
		quaternion.z/value,
		quaternion.w / value
	);
}


/**************************************************************************
*							Comparing operators
***************************************************************************/
bool
operator==(const Quaternion& quaternion1, const Quaternion& quaternion2)
{
	return quaternion1.q == quaternion2.q;
}
bool
operator!=(const Quaternion& quaternion1, const Quaternion& quaternion2)
{
	return quaternion1.q != quaternion2.q;
}

/**************************************************************************
*								Conversion
***************************************************************************/

std::ostream&
operator<<(std::ostream &os, const Quaternion& quaternion)
{
	return os << quaternion.ToString();
}
}