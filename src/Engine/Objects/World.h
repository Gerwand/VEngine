#pragma once

#include "PhysicalObject.h"
#include "Engine/Octree.h"

namespace vengine {
extern bool debugDraw;

class World : public GameObject
{
public:
	World(const std::string& name = "World");
	World(const World& source);

	virtual GameObject* Clone();

protected:
	
	virtual void OnUpdate();
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

inline void 
World::OnUpdate()
{
	if (Input::IsPressed(GLFW_KEY_TAB))
		if (Input::GetCursorMode())
			Input::DisableCursor();
		else
			Input::EnableCursor();
}

}