#pragma once

#include "MeshedObject.h"
#include "Engine/Physic/BoundingBox.h"
#include "Resources/Renderables/Lines.h"
#include "Engine/Physic/CollisionInfo.h"

namespace vengine {

class PhysicalObject : public MeshedObject
{
public:
	PhysicalObject(const std::string& name = "PhysicalObject");
	PhysicalObject(const PhysicalObject& source);

	void SetCollider(const BoundingBox& collider);
	const BoundingBox& GetCollider() const;
	BoundingBox& GetCollider();
	void SetColliderOffset(const Vector3& offset);

	bool HasChanged();
	void SetStatic(bool isStatic);
	bool IsStatic();

	void AddForce(const Vector3& force);
	virtual void OnCollision(const CollisionInfo& collision);
	void SetTransform(const Transform& transform);

	virtual GameObject* Clone();
protected:
	BoundingBox _collider;
	bool _isStatic;

	Vector3 _velocity;
	Vector3 _acceleration;
	Vector3 _colliderOffset;

	float _mass = 1;

	float _terrainFriction = 2;
	float _bounciness = 0.1f;
	bool _grounded = false;

	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnDraw(Renderer* renderer);
	virtual void OnPhysic();

	static Vector3 _gravityForce;
private:
	void UpdatePhysic();
};

inline void
PhysicalObject::SetCollider(const BoundingBox& collider)
{
	_collider = collider;
}

inline const BoundingBox& 
PhysicalObject::GetCollider() const
{
	return _collider;
}

inline BoundingBox&
PhysicalObject::GetCollider()
{
	return _collider;
}

inline bool 
PhysicalObject::HasChanged()
{
	return _isStatic ? false : _transform.GetLastPosition() != _transform.GetPosition();
}

inline void 
PhysicalObject::SetStatic(bool isStatic)
{
	_isStatic = isStatic;
}

inline bool 
PhysicalObject::IsStatic()
{
	return _isStatic;
}

inline GameObject* 
PhysicalObject::Clone()
{
	return new PhysicalObject(*this);
}

inline void
PhysicalObject::SetColliderOffset(const Vector3& offset)
{
	_colliderOffset = offset;
}


class TO : public PhysicalObject {
public:
	CameraFPP *camera;
protected:
	void OnUpdate() {
		_transform.SetPosition(camera->GetPosition() + 5.0f * camera->GetDirection());
	}
};

inline void
PhysicalObject::SetTransform(const Transform& transform)
{
	GameObject::SetTransform(transform);
	_collider.SetPosition(_transform.GetWorldPosition());
}

}