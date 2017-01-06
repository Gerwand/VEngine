#pragma once

#include "PhysicalObject.h"

namespace vengine {

class Projectile : public PhysicalObject
{
public:
	Projectile(const std::string& name = "Projectile");
	Projectile(const Projectile& source);
	virtual GameObject* Clone();

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
Projectile::Clone()
{
	return new Projectile(*this);
}

inline void
Projectile::OnCollision(const CollisionInfo& collision)
{
	if (collision.HasCollidedWithTerrain())
		GameObject::Destroy(this);
}

}