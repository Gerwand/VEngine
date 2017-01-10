#pragma once

#include "VEMath.h"

namespace vengine {

/*
* Simple class representing ray
*/
class Ray {
public:
	/* Basic constructor setting up ray */
	Ray(const Vector3& start, const Vector3& direction, float length, float accuracy = 0.1f);

	/* Get any point in distance t from the start position. The result will be saved in source vector. */
	void GetDistancePoint(Vector3* source, float t) const;

	/* Interpolate ray's current position by t units. Cannot exceed ray's length, will be clamped. */
	const Vector3& Advance(float t);
	/* Shoot ray with given accuracy, changing ray's current position */
	const Vector3& Shoot();
	
	/* Get starting point of the ray */
	const Vector3& GetStart() const;
	/* Get direction, where ray is pointing */
	const Vector3& GetDirection() const;
	/* Get maximum length of the ray */
	float GetLength() const;
	/* Get current position of the ray - result from shooting */
	const Vector3& GetCurrentPosition() const;

	/* Check if ray has ended - shooting has reached length of the ray */
	bool HasEnded();
	/* Reset ray's current position to the start */
	void ResetShooting();
private:
	Vector3 _start;		/* Starting point of the ray */
	Vector3 _direction; /* Direction, where ray is pointing */
	Vector3 _current;	/* Current position in the ray, used for "shooting" */

	float _length;		/* Length of the ray, to limit shooting */
	float _accuracy;	/* Accuracy, how much position should be advanced on each shoot */
	float _shootingParam; /* Current interpolation of the ray's position (0 = start) */
};

}