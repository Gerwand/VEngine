#pragma once


#include "HandleManager.h"
#include "Singleton.h"
#include "Resources/Voxels/VoxelArray3D.h"

#include <string>
#include <map>

namespace vengine {

/* Game manager used for storing and managing objects created from voxels. */
class VoxelArrayManager : public Singleton<VoxelArrayManager>
{
private:
	struct VATag {};

public:
	typedef Handle<VATag> HVArray;

private:
	typedef HandleManager<VoxelArray3D, HVArray> HVArrayManager;
	typedef std::map<std::string, HVArray, istring_less> NameIndex;
	typedef std::pair<typename NameIndex::iterator, bool> NameIndexInsertRc;

public:
	~VoxelArrayManager();

	HVArray GetVoxelArray(const std::string& name);

	void DeleteVoxelArray(HVArray hvarray);
	void DeleteAllVoxelArrays();

	void SetDimension(HVArray hvarray, int x, int y, int z);
	void SetVoxels(HVArray hvarray, unsigned char *voxels);
	void SetVoxelSize(HVArray hvarray, float size);
	void GenerateMesh(HVArray hvarray, VoxelMesh* mesh);

	const Voxel& GetVoxel(HVArray hvarray, int x, int y, int z);
	void GetDimension(HVArray hvarray, int* x, int* y, int* z);
	float GetVoxelSize(HVArray hvarray);
	const Vector3& GetCenter(HVArray hvarray);

private:
	HVArrayManager _varrays;	/* Manager for handles.					*/
	NameIndex _nameIndex;				/* Map associating names with handles.	*/
};

/* Define for easier access to manager, like global variable. */
#define voxelArrayManager VoxelArrayManager::GetSingleton()

}