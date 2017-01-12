#include "MeshManager.h"

namespace vengine {

MeshManager::~MeshManager()
{
	for (NameIndex::iterator i = _nameIndex.begin(); i != _nameIndex.end(); ++i) {
		_meshes.Release(i->second);
	}	
}


MeshManager::HMesh
MeshManager::GetMesh(const std::string& name)
{
	NameIndexInsertRc rc = _nameIndex.insert(std::make_pair(name, HMesh()));

	//If this element is new
	if (rc.second) {
		Mesh* mesh = _meshes.Acquire(rc.first->second);
		mesh->Init(name);
	}

	return (rc.first->second);
}

MeshManager::HMesh
MeshManager::AddMesh(Mesh* source)
{
	NameIndexInsertRc rc = _nameIndex.insert(std::make_pair(source->GetName(), HMesh()));

	//If this element is new
	assert(rc.second, "Failed to initialize %s renderable - already existing", source->GetName().c_str());
	rc.first->second = _meshes.AddItem(source);

	return (rc.first->second);
}

void
MeshManager::DeleteMesh(HMesh hmesh)
{
	Mesh* mesh = _meshes.GetItem(hmesh);
	assert(mesh != nullptr, "Invalid handle.");

	_nameIndex.erase(mesh->GetName());
	_meshes.Release(hmesh);
}

void
MeshManager::DeleteAllMeshes()
{
	for (NameIndex::iterator i = _nameIndex.begin(); i != _nameIndex.end(); ++i) {
		_meshes.Release(i->second);
	}
	_nameIndex.clear();
}

void
MeshManager::Draw(HMesh hmesh, Renderer* renderer)
{
	Mesh* mesh = _meshes.GetItem(hmesh);
	assert(mesh != nullptr, "Invalid handle %u.", hmesh.GetHandle());

	mesh->Draw(renderer);
}

void
MeshManager::AddVertices(HMesh hmesh, const Vertices& vertices, const Indices& indices)
{
	Mesh* mesh = _meshes.GetItem(hmesh);
	assert(mesh != nullptr, "Invalid handle %u.", hmesh.GetHandle());

	mesh->AddVertices(vertices, indices);
}

void
MeshManager::SetTextured(HMesh hmesh, bool textured)
{
	Mesh* mesh = _meshes.GetItem(hmesh);
	assert(mesh != nullptr, "Invalid handle %u.", hmesh.GetHandle());

	mesh->SetTextured(textured);
}

void
MeshManager::SetTexture(HMesh hmesh, unsigned int tex)
{
	Mesh* mesh = _meshes.GetItem(hmesh);
	assert(mesh != nullptr, "Invalid handle %u.", hmesh.GetHandle());

	mesh->SetTexture(tex);
}

void
MeshManager::SetTexture(HMesh hmesh, const std::string& texName)
{
	Mesh* mesh = _meshes.GetItem(hmesh);
	assert(mesh != nullptr, "Invalid handle %u.", hmesh.GetHandle());

	mesh->SetTexture(texName);
}

}