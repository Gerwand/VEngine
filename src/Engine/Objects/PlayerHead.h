#pragma once

#include "GameObject.h"

namespace vengine {

/*
* Class that is representing player's head and is handling mouse movement.
* This class must exists as separate from player, because this object will be rotating verticaly.
* If the player would be rotating verticaly, any existing model would be rotated too, and we want
* to avoid that, however, arm will be attached to the head and it must copy head's transformations
* to be always in front of the head.
*/
class PlayerHead : public GameObject {
public:
	/* Simple constructor setting name and camera as nullptr */
	PlayerHead(const std::string& name = "Player");
	/* Copying constructor */
	PlayerHead(const PlayerHead& source);

	/* Destroys camera that is stored into head */
	virtual ~PlayerHead();

	/* Used for cloning */
	virtual GameObject* Clone() const;

	/* Attaches camera to the head - camera will be destroyed with the object! */
	void SetCamera(CameraFPP* camera);
	/* Get camera attached to the head */
	CameraFPP* GetCamera() const;

private:
	CameraFPP* _camera;	/* Camera object attached to a player head */

	virtual void OnUpdate();
	/*
	* Sets camera position as position of the head. Must be in late update to prevent shaking if
	* player is colliding with other object.
	*/
	virtual void OnLateUpdate();
};

inline
PlayerHead::PlayerHead(const std::string& name) : GameObject(name)
{
	_camera = nullptr;
}

inline
PlayerHead::PlayerHead(const PlayerHead& source) : GameObject(source)
{
	_camera = source._camera;
}

inline
PlayerHead::~PlayerHead()
{
	delete _camera;
}

inline GameObject*
PlayerHead::Clone() const
{
	return new PlayerHead(*this);
}

inline void
PlayerHead::SetCamera(CameraFPP* camera)
{
	_camera = camera;
}

inline CameraFPP*
PlayerHead::GetCamera() const
{
	return _camera;
}

}