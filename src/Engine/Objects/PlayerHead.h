#pragma once

#include "GameObject.h"

namespace vengine {

class PlayerHead : public GameObject {
public:
	PlayerHead(const std::string& name = "Player");
	PlayerHead(const PlayerHead& source);

	virtual ~PlayerHead();

	virtual GameObject* Clone();

	void SetCamera(CameraFPP* camera);

private:
	CameraFPP* _camera;

	virtual void OnUpdate();
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
PlayerHead::Clone()
{
	return new PlayerHead(*this);
}

inline void
PlayerHead::SetCamera(CameraFPP* camera)
{
	_camera = camera;
}


}