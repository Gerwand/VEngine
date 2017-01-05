#pragma once

#include "GameObject.h"
#include "Managers/MeshManager.h"
#include "Time.h"

namespace vengine {

class MeshedObject : public GameObject
{
public:
	MeshedObject(const std::string& name = "Object");
	MeshedObject(const MeshedObject& source);

	void SetMesh(unsigned int mesh);
	unsigned int GetMesh();

	virtual GameObject* Clone();
protected:
	unsigned int _mesh;
	virtual void OnDraw(Renderer *renderer);
};



inline void
MeshedObject::SetMesh(unsigned int mesh)
{
	_mesh = mesh;
}

inline unsigned int
MeshedObject::GetMesh()
{
	return _mesh;
}

inline GameObject* 
MeshedObject::Clone()
{
	return new MeshedObject(*this);
}

}