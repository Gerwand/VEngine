#pragma once

#include "Vector3.h"
#include "Assert.h"

namespace vengine {

class Plane {
public:
	friend bool operator==(const Plane& p1, const Plane& p2);
	friend bool operator!=(const Plane& p1, const Plane& p2);

	Plane();
	Plane(const Vector3& normal, float d);
	Plane(float a, float b, float c, float d);
	Plane(const Vector3& point0, const Vector3& point1, const Vector3& point2);
	Plane(const Plane& source);

	const Plane& Set(const Vector3& normal, float d);
	const Plane& Set(const Vector3& normal, const Vector3& point);
	const Plane& Set(const Vector3& point0, const Vector3& point1, const Vector3& point2);

	const Plane& operator=(const Plane& other);
	
	bool IsOnPlane(const Vector3& point);
	float Distance(const Vector3& point) const;

	Vector3 RayIntersection(const Vector3& start, const Vector3& direction);
	const Vector3& GetNormal() const;

private:
	Vector3 _normal;
	float _d;
};

bool operator==(const Plane& p1, const Plane& p2);
bool operator!=(const Plane& p1, const Plane& p2);

inline const Vector3&
Plane::GetNormal() const
{
	return _normal;
}

}

