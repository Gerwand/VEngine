#include "BoundingBox.h"

namespace vengine {

BoundingBox::BoundingBox() {};
BoundingBox::BoundingBox(const Vector3& center, float length, float height, float width) :
	_center(center),
	_length(length), _height(height), _width(width)
{
	_halfDimension = _dimension / 2.0f;
	_radius = _halfDimension.Magnitude();

	_min = _center - _halfDimension;
	_max = _center + _halfDimension;
}
BoundingBox::BoundingBox(const Vector3& center, const Vector3& dimension) :
	_center(center),
	_dimension(dimension)
{
	_halfDimension = _dimension / 2.0f;
	_radius = _halfDimension.Magnitude();
	_min = _center - _halfDimension;
	_max = _center + _halfDimension;
}

BoundingBox::BoundingBox(const BoundingBox& source) : 
	_center(source._center),
	_dimension(source._dimension)
{
	_halfDimension = _dimension / 2.0f;
	_radius = _halfDimension.Magnitude();
	_min = _center - _halfDimension;
	_max = _center + _halfDimension;
}

const BoundingBox&
BoundingBox::operator=(const BoundingBox& source)
{
	_center = source._center;
	_dimension = source._dimension;
	_halfDimension = _dimension / 2.0f;
	_radius = _halfDimension.Magnitude();
	_min = _center - _halfDimension;
	_max = _center + _halfDimension;

	return *this;
}

Vector3& 
BoundingBox::GetPosition()
{
	return _center;
}

void 
BoundingBox::GetPVertex(Vector3* pVertex, const Vector3& normal) const
{
	pVertex->x = normal.x >= 0 ? _max.x : _min.x;
	pVertex->y = normal.y >= 0 ? _max.y : _min.y;
	pVertex->z = normal.z >= 0 ? _max.z : _min.z;
}

void 
BoundingBox::GetNVertex(Vector3* nVertex, const Vector3& normal) const
{
	nVertex->x = normal.x > 0 ? _min.x : _max.x;
	nVertex->y = normal.y > 0 ? _min.y : _max.y;
	nVertex->z = normal.z > 0 ? _min.z : _max.z;
}

const Vector3& 
BoundingBox::GetPosition() const
{
	return _center;
}

void 
BoundingBox::GetDimensions(float* length, float* height, float* width) const
{
	*length = _length;
	*height = _height;
	*width = _width;
}

const Vector3&
BoundingBox::GetDimension() const
{
	return _dimension;
}

BoundingBox& 
BoundingBox::Set(const Vector3& center, float length, float height, float width)
{
	_center = center;
	_length = length;
	_height = height;
	_width = width;

	_halfDimension = _dimension / 2.0f;
	_radius = _halfDimension.Magnitude();
	_min = _center - _halfDimension;
	_max = _center + _halfDimension;
	return *this;
}

BoundingBox& 
BoundingBox::Set(const Vector3& center, const Vector3& dimension)
{
	_center = center;
	_dimension = dimension;

	_halfDimension = _dimension / 2.0f;
	_radius = _halfDimension.Magnitude();
	_min = _center - _halfDimension;
	_max = _center + _halfDimension;
	return *this;
}

BoundingBox& 
BoundingBox::SetPosition(const Vector3& center)
{
	_center = center;
	_min = _center - _halfDimension;
	_max = _center + _halfDimension;

	return *this;
}

BoundingBox& 
BoundingBox::SetDimension(const Vector3& dimension)
{
	_dimension = dimension;
	_halfDimension = _dimension / 2.0f;
	_radius = _halfDimension.Magnitude();
	_min = _center - _halfDimension;
	_max = _center + _halfDimension;

	return *this;
}

BoundingBox& 
BoundingBox::SetDimension(float length, float height, float width)
{
	_length = length;
	_height = height;
	_width = width;

	_halfDimension = _dimension / 2.0f;
	_radius = _halfDimension.Magnitude();
	_min = _center - _halfDimension;
	_max = _center + _halfDimension;

	return *this;
}

bool 
BoundingBox::IsContaining(const Vector3& point) const
{
	return (between(point.x, _min.x, _max.x) &&
			between(point.y, _min.y, _max.y) &&
			between(point.z, _min.z, _max.z));
}

bool 
BoundingBox::IsContaining(const BoundingBox& boundingBox) const
{
	const Vector3 &otherMin = boundingBox._min;
	const Vector3 &otherMax = boundingBox._max;
	Vector3 pts[8] = {
		Vector3(otherMin.x, otherMin.y, otherMin.z),
		Vector3(otherMin.x, otherMin.y, otherMax.z),
		Vector3(otherMin.x, otherMax.y, otherMin.z),
		Vector3(otherMin.x, otherMax.y, otherMax.z),
		Vector3(otherMax.x, otherMin.y, otherMin.z),
		Vector3(otherMax.x, otherMin.y, otherMax.z),
		Vector3(otherMax.x, otherMax.y, otherMin.z),
		Vector3(otherMax.x, otherMax.y, otherMax.z)
	};

	for (int i = 0; i < 8; ++i) {
		if (!IsContaining(pts[i]))
			return false;
	}

	return true;
}

bool 
BoundingBox::IsColliding(const BoundingBox& other) const
{
	for (int i = 0; i < 3; ++i) {
		if (!(abs(_center[i] - other._center[i]) * 2.0f <= (_dimension[i] + other._dimension[i])))
			return false;
	}
	return true;
}

void 
BoundingBox::InsertLines(std::vector<Vector3>* lines) const
{
	Vector3 pts[8] = {
		Vector3(_min.x, _min.y, _min.z),
		Vector3(_min.x, _min.y, _max.z),
		Vector3(_min.x, _max.y, _min.z),
		Vector3(_min.x, _max.y, _max.z),
		Vector3(_max.x, _min.y, _min.z),
		Vector3(_max.x, _min.y, _max.z),
		Vector3(_max.x, _max.y, _min.z),
		Vector3(_max.x, _max.y, _max.z)
	};

	lines->push_back(pts[0]); lines->push_back(pts[1]);
	lines->push_back(pts[0]); lines->push_back(pts[2]);
	lines->push_back(pts[0]); lines->push_back(pts[4]);

	lines->push_back(pts[1]); lines->push_back(pts[3]);
	lines->push_back(pts[1]); lines->push_back(pts[5]);

	lines->push_back(pts[2]); lines->push_back(pts[3]);
	lines->push_back(pts[2]); lines->push_back(pts[6]);

	lines->push_back(pts[3]); lines->push_back(pts[7]);

	lines->push_back(pts[4]); lines->push_back(pts[5]);
	lines->push_back(pts[4]); lines->push_back(pts[6]);

	lines->push_back(pts[5]); lines->push_back(pts[7]);
	lines->push_back(pts[6]); lines->push_back(pts[7]);
}

}