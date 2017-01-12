#pragma once


#include "HandleManager.h"
#include "Singleton.h"
#include "Resources/Renderables/Mesh.h"

#include <string>
#include <map>

namespace vengine {

/* Class similar to other managers, used for mesh managing */
class MeshManager : public Singleton<MeshManager>
{
public:
	struct MeshTag {};
	typedef Handle<MeshTag> HMesh;

private:
	typedef HandleManager<Mesh, HMesh> HMeshManager;
	typedef std::map<std::string, HMesh, istring_less> NameIndex;
	typedef std::pair<typename NameIndex::iterator, bool> NameIndexInsertRc;

public:
	~MeshManager();

	/* Get new mesh or return handle to existing one */
	HMesh GetMesh(const std::string& name);
	/* Add new mesh to the manager. Return handle. Used for polymorphism */
	HMesh AddMesh(Mesh* source);

	void DeleteMesh(HMesh hmesh);
	void DeleteAllMeshes();

	void Draw(HMesh hmesh, Renderer* renderer);

	void AddVertices(HMesh hmesh, const Vertices& vertices, const Indices& indices);

	/* Set if mesh should be textured or colored */
	void SetTextured(HMesh hmesh, bool textured);
	void SetTexture(HMesh hmesh, unsigned int tex);
	void SetTexture(HMesh hmesh, const std::string& texName);

private:
	HMeshManager _meshes;	/* Manager for handles.					*/
	NameIndex _nameIndex;	/* Map associating names with handles.	*/
};

/* Define for easier access to manager, like global variable. */
#define meshManager MeshManager::GetSingleton()

}