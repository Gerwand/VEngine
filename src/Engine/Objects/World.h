#pragma once

#include "GameObject.h"

namespace vengine {

class World : public GameObject
{
public:
	World(const std::string& name = "World");
	World(const World& source);

	virtual GameObject* Clone();
};

inline
World::World(const std::string& name) : GameObject(_name)
{
}

inline
World::World(const World& source) : GameObject(source)
{
}


inline GameObject* 
World::Clone()
{
	return new World(*this);
}

}