#pragma once

#include "Assert.h"

#include "Math/Vector3.h"
#include "Math/Quaternion.h"
#include "Math/Matrix4.h"

namespace vengine {

/*
* Transform represents local object transformations: scale, rotation and transformation.
* It also delivers modeling matrices.
*/
class Transform {
	friend class GameObject; /* We are giving acces to this class to GameObejct, to simplify configuration of the transform */
public:
	/* Default constructor, setting identity rotation, location at (0,0,0) and (1,1,1) scale. */
	Transform();
	/* Copies location, rotation and scale from the other object, however, parent of the transform is not copied */
	Transform(const Transform& other);

	/* Resets position, rotation and scale to default values setted by constructor */
	void Reset();

	/* Copies location, rotation and scale from the other object */
	const Transform& Set(const Transform& other);
	/* Sets local position, scale and rotation */
	const Transform& Set(const Vector3& position, Vector3 scale, Quaternion rotation);
	/* Set local position */
	const Transform& SetPosition(const Vector3& position);
	/* Set local scale */
	const Transform& SetScale(const Vector3& scale);
	/* Set local rotation using quaternion */
	const Transform& SetRotation(const Quaternion& rotation);

	/*
	* Set parent of the object - parent's transformations will be added to object transformations,
	* required for hierarchical objects.
	*/
	void SetParent(Transform* parent);

	/* Get local position */
	Vector3& GetPosition();
	const Vector3& GetPosition() const;

	/* Get local scale */
	Vector3& GetScale();
	const Vector3& GetScale() const;

	/* Get local rotation */
	Quaternion& GetRotation();
	const Quaternion& GetRotation() const;
	
	/* Get last object's local position */
	const Vector3& GetLastPosition() const;

	/* Get object's position in world coordinates */
	const Vector3& GetWorldPosition();

	/* Get model matrix representing global transform of the object - result of the multiplication with all parents */
	const Matrix4& GetModelMatrix();

	/* Recalculate model matrix of the object */
	void UpdateMatrix();

	/* Compare two Transforms checking their current transform */
	bool operator==(const Transform& other);
private:
	Vector3 _position;		/* Local position of the object */
	Vector3 _scale;			/* Scale of the object */
	Quaternion _rotation;	/* Rotation of the object */

	/* Used for checking if matrices have to be recalculated */
	Vector3 _lastPosition;		/* Last position of the object */
	Vector3 _lastScale;			/* Last scale of the object */
	Quaternion _lastRotation;	/* Last rotation of the object */

	Vector3 _worldPosition; /* Global position of the object */

	Matrix4 _modelMatrix;	/* Model matrix which is result of multiplication with all parents */
	Matrix4 _localModelMatrix; /* Local model matrix, stored to preserve some operations if the object is static */

	Transform* _parent;	/* Parent transform of the object */
	bool _recalculated; /* Indicates that model matrix has been recalculated */

	/* Used after updating all object's matrices in the hierarchy, to indicate, that matrices are actual */
	void ResetState();
};


inline Transform::Transform(const Transform& other)
{
	Set(other);
}

inline const Transform&
Transform::Set(const Transform& other)
{
	_position = other._position;
	_scale = other._scale;
	_rotation = other._rotation;

	return *this;
}

inline const Transform&
Transform::Set(const Vector3& position, Vector3 scale, Quaternion rotation)
{
	_position = position;
	_scale = scale;
	_rotation = rotation;

	return *this;
}

inline const Transform&
Transform::SetPosition(const Vector3& position)
{
	_position = position;
	return *this;
}

inline const Transform&
Transform::SetScale(const Vector3& scale)
{
	_scale = scale;
	return *this;
}
inline const Transform&
Transform::SetRotation(const Quaternion& rotation)
{
	_rotation = rotation;
	return *this;
}

inline Vector3&
Transform::GetPosition()
{
	return _position;
}

inline Vector3&
Transform::GetScale()
{
	return _scale;
}

inline Quaternion&
Transform::GetRotation()
{
	return _rotation;
}

inline const Vector3&
Transform::GetPosition() const
{
	return _position;
}

inline const Vector3&
Transform::GetScale() const
{
	return _scale;
}

inline const Quaternion&
Transform::GetRotation() const
{
	return _rotation;
}

inline bool
Transform::operator==(const Transform& other)
{
	/* We want to compare only current values */
	return (_position == other._position &&
			_scale == other._scale &&
			_rotation == other._rotation);
}

inline void 
Transform::ResetState()
{
	_recalculated = false;
}

inline void 
Transform::SetParent(Transform* parent)
{
	_parent = parent;
}

inline const Vector3&
Transform::GetWorldPosition()
{
	/* If we have parent, we must add all transforms up to the root */
	if (_parent != nullptr) 
		_worldPosition = Vector3(_parent->GetModelMatrix() * GetPosition());
	else 
		_worldPosition = GetPosition();
	
	return _worldPosition;
}

inline const Vector3& 
Transform::GetLastPosition() const
{
	return _lastPosition;
}

}