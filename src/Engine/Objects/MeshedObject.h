#pragma once

#include "GameObject.h"
#include "Resources/Managers/MeshManager.h"

namespace vengine {

/*
* Class representing object using mesh.
*/
class MeshedObject : public GameObject
{
public:
	/*
	* Basic constructor, setting mesh as 0.
	*
	* @param name first part for the name of the object
	*/
	MeshedObject(const std::string& name = "Object");
	/*
	* Copying constructor.
	*
	* @param source other meshed object
	*/
	MeshedObject(const MeshedObject& source);

	/*
	* Set mesh handle, which is granted from mesh manager. If mesh handle is 0, mesh is not drawn.
	*
	* @param mesh mesh handle
	*/
	void SetMesh(unsigned int mesh);
	/*
	* Get mesh handle. 
	*
	* @return Mesh handle.
	*/
	unsigned int GetMesh();

	/*
	* Creates exact copy of this object.
	*
	* @return Pointer to the cloned object.
	*/
	virtual GameObject* Clone();
protected:
	unsigned int _mesh;	/* Mesh handle from mesh manager */

	/* Draw mesh */
	virtual void OnDraw(Renderer *renderer);
};

inline MeshedObject::MeshedObject(const std::string& name) : GameObject(name), _mesh(0)
{
}

inline MeshedObject::MeshedObject(const MeshedObject& source) : GameObject(source), _mesh(source._mesh)
{
}

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