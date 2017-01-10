#pragma once

#include "Math/Vector3.h"

#include <vector>

namespace vengine {

/*
* Bounding box is the class representing AABB collider - Axis Aligned Bounding Box.
* It can check collision with other AABB or points.
*/
class BoundingBox {
public:
	/* Default constructor, doing nothing */
	BoundingBox();
	/* Copying constructor */
	BoundingBox(const BoundingBox& source);
	/*
	* Creates new AABB.
	*
	* @param center center of the AABB
	* @param length x axis dimension of the AABB
	* @param height y axis dimension of the AABB
	* @param width z axis dimension of the AABB
	*/
	BoundingBox(const Vector3& center, float length, float height, float width);
	/*
	* Creates new AABB.
	*
	* @param center center of the AABB
	* @param dimension x, y, and z dimension of the AABB
	*/
	BoundingBox(const Vector3& center, const Vector3& dimension);

	/* Copies AABB */
	const BoundingBox& operator=(const BoundingBox& source);

	/* Returns center of the AABB */
	const Vector3& GetPosition() const;
	Vector3& GetPosition();
	
	/* Get x, y and z dimensions of the AABB */
	const Vector3& GetDimension() const;
	/*
	* Get dimensions of the AABB
	*
	* @param length x axis dimension of the AABB
	* @param height y axis dimension of the AABB
	* @param width z axis dimension of the AABB
	*/
	void GetDimensions(float* length, float* height, float* width) const;

	/* AABB can be treated as sphere, where radius is the diagonal/2 of the AABB*/
	float GetRadius() const;
	/* Returns p-vertex of the AABB according to normal vector of the plane. Used for checking collisions with the frustum */
	void GetPVertex(Vector3* pVertex, const Vector3& normal) const;
	/* Returns n-vertex of the AABB according to normal vector of the plane */
	void GetNVertex(Vector3* nVertex, const Vector3& normal) const;

	/* Set position and dimensions of the AABB, same as constructor */
	BoundingBox& Set(const Vector3& center, float length, float height, float width);
	BoundingBox& Set(const Vector3& center, const Vector3& dimension);

	/* Set center of the AABB */
	BoundingBox& SetPosition(const Vector3& center);

	/* Set dimensions of the AABB */
	BoundingBox& SetDimension(const Vector3& dimension);
	BoundingBox& SetDimension(float length, float height, float width);

	/*
	* Check is object is fully inside object. Also true if touching borders of the AABB,
	* however cannot be outside of the AABB
	*/
	bool IsContaining(const Vector3& point) const;
	bool IsContaining(const BoundingBox& boundingBox) const;

	/* Check if AABBs are colliding. True if they are intersecting */
	bool IsColliding(const BoundingBox& other) const;

	/* AABB is valid if all dimensions are different than 0 */
	bool IsValid() const;

	/* Inserts points representing AABB's lines into the vector of Vector3's, used for drawing colliders. */
	void InsertLines(std::vector<Vector3>* points) const;

	/* Get minimal world coordinates of the AABB */
	const Vector3& GetMinimas() const;
	/* Get maximal world coordinates of the AABB */
	const Vector3& GetMaximas() const;

private:
	Vector3 _center;		/* Center of the AABB */
	/* Union describing dimensions of the AABB */
	union {
		struct {
			float _length;
			float _height;
			float _width;
		};
		Vector3 _dimension;
	};

	Vector3 _halfDimension; /* Precomputed half dimension to speed up many calculations */

	Vector3 _min;			/* Minimal coordinates of the AABB */
	Vector3 _max;			/* Maximal coordinates of the AABB */
	float _radius;			/* Radius if AABB has to be treated as a sphere */
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