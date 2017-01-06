#pragma once

#include "MeshedObject.h"

namespace vengine {

/*
* Simple class representing enemy's head, for now doing the same thing as meshed object.
* Example for hierarchy.
*/
class EnemyHead : public MeshedObject
{
public:
	EnemyHead(const std::string& name = "EnemyHead");
	EnemyHead(const EnemyHead& source);

	virtual GameObject* Clone();

	void SetPlayer(GameObject* player);

protected:
	/* Currently doing nothing, but head should po pointing towards player if he is close enough */
	virtual void OnUpdate();

	GameObject* _player = nullptr;
};

inline
EnemyHead::EnemyHead(const std::string& name) : MeshedObject(name)
{
}

inline
EnemyHead::EnemyHead(const EnemyHead& source) : MeshedObject(source)
{
}

inline GameObject*
EnemyHead::Clone()
{
	return new EnemyHead(*this);
}

inline void 
EnemyHead::SetPlayer(GameObject* player)
{
	_player = player;
}

inline void
EnemyHead::OnUpdate()
{
	assert(_player != nullptr, "Player is nullptr!");
	//There should go code for looking at player
}

}