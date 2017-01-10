#pragma once

#include "PhysicalObject.h"

namespace vengine {

/* 
* Class projectile extends physical object, however it is not interacting phsyically
* with other objects, only with terrain.
*/
class Projectile : public PhysicalObject
{
public:
	/* Simple constructor setting name */
	Projectile(const std::string& name = "Projectile");
	/* Copying constructor */
	Projectile(const Projectile& source);

	/* Used for cloning */
	virtual GameObject* Clone() const;

	/*
	* It overrides default physical object collision handling.
	* Whenever projectile hits the ground, it will be destroyed.
	*/
	virtual void OnCollision(const CollisionInfo& collision);
};

inline
Projectile::Projectile(const std::string& name) : PhysicalObject(name)
{
}

inline
Projectile::Projectile(const Projectile& source) : PhysicalObject(source)
{
}

inline GameObject* 
Projectile::Clone() const
{
	return new Projectile(*this);
}

inline void
Projectile::OnCollision(const CollisionInfo& collision)
{
	if (collision.HasCollidedWithTerrain())
		GameObject::Destroy(this);

	return;
}

}