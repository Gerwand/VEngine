#include "Plane.h"

namespace vengine {

Plane::Plane()
{
}

Plane::Plane(const Vector3& normal, float d) : _normal(normal), _d(d)
{
	assert(around(_normal.MagnitudeFast(), 1.0f, 0.001f), "Normal vector of the plane must be normalized.");
}

Plane::Plane(float a, float b, float c, float d) : _normal(a, b, c), _d(d)
{
	assert(around(_normal.MagnitudeFast(), 1.0f, 0.001f), "Normal vector of the plane must be normalized.");
}

Plane::Plane(const Vector3& point0, const Vector3& point1, const Vector3& point2)
{
	Set(point0, point1, point2);
}

Plane::Plane(const Plane& source) : _normal(source._normal), _d(source._d)
{
}


const Plane&
Plane::Set(const Vector3& point0, const Vector3& point1, const Vector3& point2)
{
	Vector3 v1 = Vector3::Normalized(point1 - point0);
	Vector3 v2 = Vector3::Normalized(point2 - point0);

	_normal = Vector3::Cross(v1, v2).Normalize(); //Ax + By + Cz = -D
	_d = -Vector3::Dot(point0, _normal);	// -D = Ax0 + Bx0 + Cx0

	return *this;
}

const Plane&
Plane::Set(const Vector3& normal, float d)
{
	_normal = normal;
	_d = d;
	return *this;
}

const Plane&
Plane::Set(const Vector3& normal, const Vector3& point)
{
	_normal = normal;
	_d = Vector3::Dot(normal, point);
	return *this;
}

const Plane&
Plane::operator=(const Plane& other)
{
	_normal = other._normal;
	_d = other._d;

	return *this;
}

float 
Plane::Distance(const Vector3& point) const
{
	return Vector3::Dot(_normal, point) + _d;
}

Vector3 
Plane::RayIntersection(const Vector3& start, const Vector3& direction)
{
	assert(around(direction.MagnitudeFast(), 1.0f, 0.001f), "Direction vector of the plane must be normalized.");
	/* Check if ray is pararell */
	float a = Vector3::Dot(_normal, direction);
	if (between(a, -0.00001f, 0.00001f))
		return start;

	return start - direction*(Distance(start)/a);
}

bool 
operator==(const Plane& p1, const Plane& p2)
{
	return (p1._normal == p2._normal && p1._d == p2._d);
}

bool 
operator!=(const Plane& p1, const Plane& p2)
{
	return (p1._normal != p2._normal || p1._d != p2._d);
}

}