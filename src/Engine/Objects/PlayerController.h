#pragma once

#include "PhysicalObject.h"
#include "Engine/Physic/RayIntersection.h"
#include "Engine/Octree.h"
#include "Resources/Managers/GameObjectManager.h"

namespace vengine {

class PlayerController : public PhysicalObject {
public:
	PlayerController(const std::string& name = "Player");
	PlayerController(const PlayerController& source);

	virtual GameObject* Clone();

	void SetCamera(CameraFPP* camera);
	void SetOctree(Octree* octree);
	void SetArm(GameObject* arm);

	float speed = 5.0f;
	float runModifier = 2.0f;
	float jumpForce = 20.0f;
	float digDistance = 5.0f;
private:
	enum Tools {
		NONE,
		SWORD,
		PICKAXE,
		DIRT,
		GRASS,
		WOOD,
		STONE
	};

	CameraFPP* _camera;
	Voxel::Type _activeItem;
	Lines _voxLines;
	Octree* _octree;
	RayIntersection rayInfo;

	GameObject *_arm;

	Tools _tool;

	virtual void OnUpdate();
	virtual void OnInit();
	virtual void OnDraw(Renderer* renderer);
	bool grounded = false;

	void Move();
	void ChangeBlocks();
	void ChooseTool();
	void AttachTool();

	static std::string toolNames[7];
};


inline
PlayerController::PlayerController(const std::string& name) : PhysicalObject(name)
{
	_camera = nullptr;
	_octree = nullptr;
	_mass = 2.0f;
	_bounciness = 0.0f;
	_tool = NONE;
}

inline
PlayerController::PlayerController(const PlayerController& source) : PhysicalObject(source)
{
	speed = source.speed;
	runModifier = source.runModifier;

	_camera = source._camera;
	_octree = source._octree;
	_tool = source._tool;
}

inline GameObject*
PlayerController::Clone()
{
	return new PlayerController(*this);
}

inline void 
PlayerController::SetCamera(CameraFPP* camera)
{
	_camera = camera;
}

inline void 
PlayerController::SetOctree(Octree* octree)
{
	_octree = octree;
}

inline void 
PlayerController::SetArm(GameObject* arm)
{
	_arm = arm;
}

}