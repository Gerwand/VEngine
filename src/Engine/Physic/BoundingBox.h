#pragma once

#include "Math/Vector3.h"

#include <vector>

namespace vengine {

class BoundingBox {
public:
	BoundingBox();
	BoundingBox(const BoundingBox& source);
	BoundingBox(const Vector3& start, float length, float height, float width);
	BoundingBox(const Vector3& start, const Vector3& dimension);

	const BoundingBox& operator=(const BoundingBox& source);

	Vector3& GetPosition();
	float GetRadius() const;
	void GetPVertex(Vector3* pVertex, const Vector3& normal) const;
	void GetNVertex(Vector3* nVertex, const Vector3& normal) const;
	const Vector3& GetPosition() const;
	void GetDimensions(float* length, float* height, float* width) const;
	const Vector3& GetDimension() const;

	BoundingBox& Set(const Vector3& start, float length, float height, float width);
	BoundingBox& Set(const Vector3& start, const Vector3& dimension);
	BoundingBox& SetPosition(const Vector3& start);
	BoundingBox& SetDimension(const Vector3& dimension);
	BoundingBox& SetDimension(float length, float height, float width);

	bool IsContaining(const Vector3& point) const;
	bool IsContaining(const BoundingBox& boundingBox) const;

	bool IsIntersected(const BoundingBox& boundingBox) const;

	bool IsColliding(const BoundingBox& other) const;

	bool IsValid() const;

	void InsertLines(std::vector<Vector3>* points) const;

	const Vector3& GetMinimas() const;
	const Vector3& GetMaximas() const;

private:
	Vector3 _center;
	union {
		struct {
			float _length;
			float _height;
			float _width;
		};
		Vector3 _dimension;
	};
	Vector3 _halfDimension;

	Vector3 _min;
	Vector3 _max;
	float _radius;
};

inline bool 
BoundingBox::IsValid() const
{
	return (_length != 0.0f && _height > 0.0f && _width > 0.0f);
}


inline float
BoundingBox::GetRadius() const
{
	return _radius;
}

inline const Vector3& 
BoundingBox::GetMinimas() const
{
	return _min;
}

inline const Vector3& 
BoundingBox::GetMaximas() const
{
	return _max;
}

}