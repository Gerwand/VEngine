#include "VoxelArrayManager.h"

namespace vengine {

VoxelArrayManager::~VoxelArrayManager()
{
	for (NameIndex::iterator i = _nameIndex.begin(); i != _nameIndex.end(); ++i) {
		_varrays.Release(i->second);
	}
}

VoxelArrayManager::HVArray
VoxelArrayManager::GetVoxelArray(const std::string& name)
{
	NameIndexInsertRc rc = _nameIndex.insert(std::make_pair(name, HVArray()));

	//If this element is new
	if (rc.second) {
		VoxelArray3D* voxels = _varrays.Acquire(rc.first->second);
		//if (shader->Init(name, type)) {
		voxels->Init(name);
	}

	return (rc.first->second);
}

void
VoxelArrayManager::DeleteVoxelArray(HVArray hvarray)
{
	VoxelArray3D* voxels = _varrays.GetItem(hvarray);
	assert(voxels != NULL, "Invalid handle %u.", hvarray.GetHandle());

	_nameIndex.erase(voxels->GetName());
	_varrays.Release(hvarray);
}

void
VoxelArrayManager::DeleteAllVoxelArrays()
{
	for (NameIndex::iterator i = _nameIndex.begin(); i != _nameIndex.end(); ++i) {
		_varrays.Release(i->second);
	}
	_nameIndex.clear();
}

const Voxel&
VoxelArrayManager::GetVoxel(HVArray hvarray, int x, int y, int z)
{
	VoxelArray3D* voxels = _varrays.GetItem(hvarray);
	assert(voxels != NULL, "Invalid handle %u.", hvarray.GetHandle());

	return voxels->Get(x, y, z);
}

void 
VoxelArrayManager::SetDimension(HVArray hvarray, int x, int y, int z)
{
	VoxelArray3D* voxels = _varrays.GetItem(hvarray);
	assert(voxels != NULL, "Invalid handle %u.", hvarray.GetHandle());

	voxels->SetDimension(x, y, z);
}

void
VoxelArrayManager::SetVoxels(HVArray hvarray, unsigned char *voxels)
{
	VoxelArray3D* varray = _varrays.GetItem(hvarray);
	assert(voxels != NULL, "Invalid handle %u.", hvarray.GetHandle());

	varray->SetTypes(voxels);
}

void 
VoxelArrayManager::SetVoxelSize(HVArray hvarray, float size)
{
	VoxelArray3D* voxels = _varrays.GetItem(hvarray);
	assert(voxels != NULL, "Invalid handle %u.", hvarray.GetHandle());

	voxels->SetVoxelSize(size);
}

void 
VoxelArrayManager::GenerateMesh(HVArray hvarray, VoxelMesh* mesh)
{
	VoxelArray3D* voxels = _varrays.GetItem(hvarray);
	assert(voxels != NULL, "Invalid handle %u.", hvarray.GetHandle());

	voxels->GenerateMesh(mesh);
}

void 
VoxelArrayManager::GetDimension(HVArray hvarray, int* x, int* y, int* z)
{
	VoxelArray3D* voxels = _varrays.GetItem(hvarray);
	assert(voxels != NULL, "Invalid handle %u.", hvarray.GetHandle());

	voxels->GetDimension(x, y, z);
}

float
VoxelArrayManager::GetVoxelSize(HVArray hvarray)
{
	VoxelArray3D* voxels = _varrays.GetItem(hvarray);
	assert(voxels != NULL, "Invalid handle %u.", hvarray.GetHandle());

	return voxels->GetVoxelSize();
}

const Vector3&
VoxelArrayManager::GetCenter(HVArray hvarray)
{
	VoxelArray3D* voxels = _varrays.GetItem(hvarray);
	assert(voxels != NULL, "Invalid handle %u.", hvarray.GetHandle());

	return voxels->GetCenter();
}

}