#pragma once

#include "MeshedObject.h"
#include "Engine/Physic/BoundingBox.h"
#include "Resources/Renderables/Lines.h"
#include "Engine/Physic/CollisionInfo.h"

namespace vengine {
/*
* Extends functionality of the meshed objects, adding very basic physic and AABB collider.
*/
class PhysicalObject : public MeshedObject
{
public:
	/* Constructor that is initializing physical properties of the object and setting it's name */
	PhysicalObject(const std::string& name = "PhysicalObject");
	/* Copy constructor, used for cloning obejcts */
	PhysicalObject(const PhysicalObject& source);

	/* Set AABB collider for the object */
	void SetCollider(const BoundingBox& collider);
	/* Get current AABB collider of the object */
	const BoundingBox& GetCollider() const;
	BoundingBox& GetCollider();

	/* Set collider's offset from the center of the object */
	void SetColliderOffset(const Vector3& offset);

	/* Check if object's position has changed since last Update */
	bool HasChanged() const;
	/*
	* Set that object is static - physic is not apllied to static objects and collision checks are skipped in handler,
	* however, collision still can be handled in inheritated class
	*/
	void SetStatic(bool isStatic);
	/* Checks if object is static */
	bool IsStatic() const;

	/* Add force vector to the object. Heavier objects requires higher mass */
	void AddForce(const Vector3& force);

	/* Set mass of the object */
	void SetMass(float mass);
	/* Set bounciness of the object */
	void SetBounciness(float bounciness);
	/* Set bounciness of the object */
	void SetFriction(float friction);
	/* Get mass of the object */
	float GetMass() const;
	/* Get bounciness of the object */
	float GetBounciness() const;
	/* Get friction of the object */
	float GetFriction() const;

	/* Set new transform of the object and updates position of the bounding box, because AABB uses global positioning */
	void SetTransform(const Transform& transform);

	/* Used for enabling calling Instantiate */
	virtual GameObject* Clone() const;
	/* Handler for collision detections */
	virtual void OnCollision(const CollisionInfo& collision);
protected:
	BoundingBox _collider;			/* AABB collider */
	bool _isStatic;					/* Indicates that object is static or not */

	Vector3 _velocity;				/* Velocity of the object*/
	Vector3 _acceleration;			/* Temporary acceleration, will be zeroes after applying physic */
	Vector3 _colliderOffset;		/* Offset of the AABB from the center of the object */
	
	float _mass = 1.0f;				/* Mass of the object */
	float _terrainFriction = 2.0f;	/* Indicates friction - will be applied to grounded objects slowing theirs speed */
	float _bounciness = 0.1f;		/* Bounciness - how much velocity will be preserved after bumping from terrain */

	bool _grounded = false;			/* Indicates that object is standing on the ground */

	/* It is setting initial position of the collider depending of the object and updating transform's matrix. */
	virtual void OnInit(); 
	/* If proper debug option is enabled, it is drawing collider around object */
	virtual void OnDraw(Renderer* renderer);
	/* Applies physic to the non static objects */
	virtual void OnPhysic();

	/* Value for gravity force that will be applied to each non static physical object each frame */
	static Vector3 _gravityForce;
private:
	/* Calculating velocity of the object and adding gravity force */
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
PhysicalObject::HasChanged() const
{
	/* If object is static we are assuming it has not changed. Else, check if position changed. */
	return _isStatic ? false : _transform.GetLastPosition() != _transform.GetPosition();
}

inline void 
PhysicalObject::SetStatic(bool isStatic)
{
	_isStatic = isStatic;
}

inline bool 
PhysicalObject::IsStatic() const
{
	return _isStatic;
}

inline GameObject* 
PhysicalObject::Clone() const
{
	return new PhysicalObject(*this);
}

inline void
PhysicalObject::SetColliderOffset(const Vector3& offset)
{
	_colliderOffset = offset;
}

inline void
PhysicalObject::SetTransform(const Transform& transform)
{
	GameObject::SetTransform(transform);
	_collider.SetPosition(_transform.GetWorldPosition());
}

inline void
PhysicalObject::SetMass(float mass)
{
	_mass = mass;
}

inline float 
PhysicalObject::GetMass() const
{
	return _mass;
}

inline void 
PhysicalObject::SetBounciness(float bounciness)
{
	_bounciness = bounciness;
}

inline float
PhysicalObject::GetBounciness() const
{
	return _bounciness;
}

inline void
PhysicalObject::SetFriction(float friction)
{
	_terrainFriction = friction;
}

inline float
PhysicalObject::GetFriction() const
{
	return _terrainFriction;
}

}