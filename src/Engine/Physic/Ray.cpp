#include "Ray.h"

namespace vengine {
Ray::Ray(const Vector3& start, const Vector3& direction, float length, float accuracy) :
	_start(start), _direction(direction), _current(_start)
{
	_length = length;
	_accuracy = accuracy;
	_shootingParam = -accuracy;
}

void
Ray::GetDistancePoint(Vector3* source, float t) const
{
	*source = _start + (_direction * t);
}

float 
Ray::GetLength() const
{
	return _length;
}

const Vector3& 
Ray::GetStart() const
{
	return _start;
}

const Vector3& 
Ray::GetDirection() const
{
	return _direction;
}

bool
Ray::HasEnded()
{
	return _shootingParam >= _length;
}

void 
Ray::ResetShooting()
{
	_current = _start;
	_shootingParam = 0.0f;
}

const Vector3& 
Ray::Shoot()
{
	if (!HasEnded()) {
		_shootingParam += _accuracy;
		
		if (HasEnded())
			_shootingParam = _length;

		GetDistancePoint(&_current, _shootingParam);
	}
	return _current;
}

const Vector3&
Ray::Advance(float t)
{
	if (!HasEnded()) {
		_shootingParam += t;

		if (HasEnded())
			_shootingParam = _length;

		GetDistancePoint(&_current, _shootingParam);
	}
	return _current;
}

const Vector3& 
Ray::GetCurrentPosition() const
{
	return _current;
}

}