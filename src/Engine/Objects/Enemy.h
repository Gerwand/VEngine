#pragma once

#include "PhysicalObject.h"

namespace vengine {

/* Simple class representing enemy, for now, doing the same thing as physical object. Example for hierarchy. */
class Enemy : public PhysicalObject
{
public:
	Enemy(const std::string& name = "Enemy");
	Enemy(const Enemy& source);

	virtual GameObject* Clone();

	void SetPlayer(GameObject* player);

protected:
	virtual void OnUpdate();

	GameObject* _player = nullptr;
};

inline
Enemy::Enemy(const std::string& name) : PhysicalObject(name)
{
}

inline
Enemy::Enemy(const Enemy& source) : PhysicalObject(source)
{
}

inline GameObject*
Enemy::Clone()
{
	return new Enemy(*this);
}

inline void
Enemy::SetPlayer(GameObject* player)
{
	_player = player;
}

inline void
Enemy::OnUpdate()
{
	assert(_player != nullptr, "Player is nullptr!");
	//There should go AI code
}

}