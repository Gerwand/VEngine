#pragma once

#include "PhysicalObject.h"
#include "Engine/Physic/RayIntersection.h"
#include "Engine/Octree.h"
#include "Resources/Managers/GameObjectManager.h"

namespace vengine {

/* PlayerControler is responsible for reading input and moving player. However it is not responsible for transforming mouse input */
class PlayerController : public PhysicalObject {
public:
	/* Default constructor, that initializes everything */
	PlayerController(const std::string& name = "Player");
	/* Copy constructor used for cloning */
	PlayerController(const PlayerController& source);

	/* Function that clones whole branch */
	virtual GameObject* Clone() const;

	/* Sets camera that will be attached to the player - now it becomes ownership of the player, and it will be destroyed with the player */
	void SetCamera(CameraFPP* camera);
	CameraFPP* GetCamera() const;

	/* Set pointer to the active octree - needed for insterting voxels into the tree */
	void SetOctree(Octree* octree);
	Octree* GetOctree() const;

	/* Set pointer to the object that represents arm - needed for attaching proper tools to the arm */
	void SetArm(GameObject* arm);
	GameObject* GetArm() const;

	/* Set moving speed of the player in m/s */
	void SetSpeed(float speed);
	float GetSpeed() const;

	/* Set how many times player should be faster when running */
	void SetRunModifier(float runModifier);
	float GetRunModifier() const;

	/* Set how high player should be jumping */
	void SetJumpForce(float jumpForce);
	float GetJumpForce() const;

	/* Set how far player should be able to dig block in meters */
	void SetDigDistance(float digDistance);
	float GetDigDistance() const;

protected:
	/* Tool types that player can hold */
	enum Tools {
		NONE,		/* Nothing */
		SWORD,		/* Sword for throwing */
		PICKAXE,	/* Pickaxe for digging */
		DIRT,		/* Dirt for building dirt voxels */
		GRASS,		/* Grass for building grass voxels */
		WOOD,		/* Wood for building wood voxels */
		STONE		/* Stone for building stone voxels */
	};

	float _speed = 5.0f;		/* Speed of the player */
	float _runModifier = 2.0f;	/* How much faster the player will be when running */
	float _jumpForce = 20.0f;	/* Force that will be applied during jumping */
	float _digDistance = 5.0f;	/* How far in meters player can dig */

	CameraFPP* _camera;			/* Pointer to the player's camera, needed for shooting rays into voxels */
	Octree* _octree;			/* Actree inside which spawned objects will be inserted */

	GameObject *_arm;			/* Pointer to object representing player's arm, tools will be inserted there */
	Tools _tool;				/* Current selected tool */

	static std::string _toolNames[7]; /* Names of the tools that are stored in the gameObjectManager */

	/*
	* Each frame, if mouse cursor is disabled, we are checking if player changed tool, if blocks had been
	* selected for change and player movement is handled.
	*/
	virtual void OnUpdate();
	/* It is initializing Lines object */
	virtual void OnInit();
	/* If voxel had been hit by ray, it will be highlighted */
	virtual void OnDraw(Renderer* renderer);

	/* Handles player movement */
	void Move();
	/* Modifies blocks, depending on selected tool */
	void ChangeBlocks();
	/* Checks if other tool was selected */
	void ChooseTool();
	/*  Spawns proper model for the arm model */
	void AttachTool();

private:
	Lines _voxLines;		  /* Renderable object for drawing border around selected voxel */
	RayIntersection _rayInfo; /* Ray info about hitted voxels for sharing with all functions */
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
	_speed = source._speed;
	_runModifier = source._runModifier;

	_camera = source._camera;
	_octree = source._octree;
	_tool = source._tool;
}

inline GameObject*
PlayerController::Clone() const
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


inline CameraFPP* 
PlayerController::GetCamera() const
{
	return _camera;
}

inline Octree* 
PlayerController::GetOctree() const
{
	return _octree;
}

inline GameObject*
PlayerController::GetArm() const
{
	return _arm;
}

inline void 
PlayerController::SetSpeed(float speed)
{
	_speed = speed;
}

inline float 
PlayerController::GetSpeed() const
{
	return _speed;
}

inline void 
PlayerController::SetRunModifier(float runModifier)
{
	_runModifier = runModifier;
}

inline float 
PlayerController::GetRunModifier() const
{
	return _runModifier;
}

inline void 
PlayerController::SetJumpForce(float jumpForce)
{
	_jumpForce = jumpForce;
}

inline float 
PlayerController::GetJumpForce() const
{
	return _jumpForce;
}

inline void 
PlayerController::SetDigDistance(float digDistance)
{
	_digDistance = digDistance;
}

inline float 
PlayerController::GetDigDistance() const
{
	return _digDistance;
}

}