#pragma once

#include "Assert.h"

#include "Math/Vector3.h"
#include "Math/Quaternion.h"
#include "Math/Matrix4.h"

namespace vengine {

class Transform {
	friend class GameObject;
public:
	Transform();
	Transform(const Transform& other);

	void Reset();

	void Set(const Transform& other);
	void Set(const Vector3& position, Vector3 scale, Quaternion rotation);
	void SetPosition(const Vector3& position);
	void SetScale(const Vector3& scale);
	void SetRotation(const Quaternion& rotation);
	void SetParent(Transform* parent);

	Vector3& GetPosition();
	Vector3& GetScale();
	Quaternion& GetRotation();
	
	const Vector3& GetPosition() const;
	const Vector3& GetLastPosition() const;
	const Vector3& GetScale() const;
	const Quaternion& GetRotation() const;
	const Vector3& GetWorldPosition();

	const Matrix4& GetModelMatrix();

	void UpdateMatrix();
	bool operator==(const Transform& other);
private:
	Vector3 _position;
	Vector3 _scale;
	Quaternion _rotation;
	Vector3 _lastPosition;
	Vector3 _lastScale;
	Quaternion _lastRotation;

	Vector3 _worldPosition;

	Matrix4 _modelMatrix;
	Matrix4 _localModelMatrix;

	Transform* _parent;
	bool _recalculated;

	void ResetState();
};


inline Transform::Transform(const Transform& other)
{
	Set(other);
}

inline void
Transform::Set(const Transform& other)
{
	_position = other._position;
	_scale = other._scale;
	_rotation = other._rotation;
}

inline void
Transform::Set(const Vector3& position, Vector3 scale, Quaternion rotation)
{
	_position = position;
	_scale = scale;
	_rotation = rotation;
}

inline void
Transform::SetPosition(const Vector3& position)
{
	_position = position;
}

inline void
Transform::SetScale(const Vector3& scale)
{
	_scale = scale;
}
inline void
Transform::SetRotation(const Quaternion& rotation)
{
	_rotation = rotation;
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
	if (_parent != nullptr)
		_worldPosition = GetPosition() + _parent->GetPosition();
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