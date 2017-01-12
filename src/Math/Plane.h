#pragma once

#include "Vector3.h"
#include "Assert.h"

namespace vengine {

/* Class representing plane */
class Plane {
public:
	friend bool operator==(const Plane& p1, const Plane& p2);
	friend bool operator!=(const Plane& p1, const Plane& p2);

	Plane();
	Plane(const Vector3& normal, float d);
	Plane(float a, float b, float c, float d);

	Plane(const Vector3& point0, const Vector3& point1, const Vector3& point2);
	Plane(const Plane& source);

	/* Set plane using normal and d parameter */
	const Plane& Set(const Vector3& normal, float d);
	/* Set plane using normal and point */
	const Plane& Set(const Vector3& normal, const Vector3& point);
	/* Set plane using 3 points */
	const Plane& Set(const Vector3& point0, const Vector3& point1, const Vector3& point2);

	const Plane& operator=(const Plane& other);
	
	/* Get point's distance from the plane */
	float Distance(const Vector3& point) const;

	/* Get intersection of the infinite ray with the plane - should be improved to use Ray class */
	Vector3 RayIntersection(const Vector3& start, const Vector3& direction);

	/* Get normal of the plane */
	const Vector3& GetNormal() const;

private:
	Vector3 _normal; /* Normal of the plane */
	float _d;		/* Parameter d of the plane */
};

bool operator==(const Plane& p1, const Plane& p2);
bool operator!=(const Plane& p1, const Plane& p2);

inline const Vector3&
Plane::GetNormal() const
{
	return _normal;
}

}

