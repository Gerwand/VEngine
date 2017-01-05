#pragma once

#include "VEMath.h"

namespace vengine {

class Ray {
public:
	Ray(const Vector3& start, const Vector3& direction, float length, float accuracy = 0.1f);

	void GetDistancePoint(Vector3* source, float t) const;
	const Vector3& GetStart() const;
	const Vector3& GetDirection() const;
	float GetLength() const;

	bool HasEnded();
	void ResetShooting();
	const Vector3& Shoot();
	const Vector3& GetCurrentPosition();
	const Vector3& Advance(float t);
private:
	Vector3 _start;
	Vector3 _direction;
	Vector3 _current;

	float _length;
	float _accuracy;
	float _shootingParam;
};

}