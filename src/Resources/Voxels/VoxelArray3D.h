#pragma once

#include "Assert.h"

#include "Math/Vector3.h"
#include "Voxel.h"
#include "Resources/Renderables/VoxelMesh.h"

namespace vengine {

/* Class representing voxel array in 3 dimensions.*/
class VoxelArray3D {
public:
	VoxelArray3D();
	VoxelArray3D(int sx, int sy, int sz);

	~VoxelArray3D();

	void Init(const std::string& name, int sx = 0, int sy = 0, int sz = 0);
	void Clear();
	
	const Voxel& Get(int x, int y, int z) const;
	const Voxel& Get(int *coords) const;
	const Voxel& Get(const Vector3& coords) const;

	const std::string& GetName() const;

	/* Set voxel array from unsigned char array. Dimension of the voxel array must be defined before calling this funtion. */
	void SetTypes(unsigned char *types);

	/* Set dimension of the Voxel Array */
	void SetDimension(int x, int y, int z);

	/* Size in world units of each voxel. */
	void SetVoxelSize(float size);

	void GetDimension(int *x, int *y, int *z) const;
	float GetVoxelSize() const;

	/* Generate mesh for the voxel array and save it into voxel mesh object. All existing vertices inside this object will be deleted */
	void GenerateMesh(VoxelMesh* mesh);

	/* Check if VoxelArray is containing only empty voxels */
	bool IsEmpty();
	/* Check if voxel arrah has been initialized properly and have proper size  */
	bool IsValid() const;

	/* Get center of the voxel array */
	const Vector3& GetCenter();

protected:
	std::string _name; 
	Voxel* _voxels;		/* 1D Array containing all voxels for better cache managing */
	float _voxelSize;	/* Size of each voxel */
	int _numElements;	/* Total number of voxels */
	bool _empty;		/* If we did not created any quads during generating mesh, voxel array is empty. */

	union {
		int _dimension[3];
		struct {
			int _sx;
			int _sy;
			int _sz;
		};
	};

	Vector3 _center; /* Center of the voxel array */

	/* Insert quad with given properties into the mesh */
	void InsertQuad(VoxelMesh* mesh, const Voxel& voxel, Voxel::Side side, bool front, int w, int h,
					const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4);
};



inline void 
VoxelArray3D::GetDimension(int *x, int *y, int *z) const
{
	*x = _sx;
	*y = _sy;
	*z = _sz;
}

inline void 
VoxelArray3D::SetVoxelSize(float size)
{
	_voxelSize = size;
	_center = Vector3((float)_sx, (float)_sy, (float)_sz) / 2.0f * _voxelSize;
}

inline float 
VoxelArray3D::GetVoxelSize() const
{
	return _voxelSize;
}

inline const std::string&
VoxelArray3D::GetName() const
{
	return _name;
}

inline bool 
VoxelArray3D::IsEmpty()
{
	return _empty;
}

inline const Vector3& 
VoxelArray3D::GetCenter()
{
	return _center;
}

}