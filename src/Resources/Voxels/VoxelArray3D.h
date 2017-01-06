#pragma once

#include "Assert.h"

#include "Math/Vector3.h"
#include "Voxel.h"
#include "Resources/Renderables/VoxelMesh.h"

namespace vengine {

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

	//void SetTypes(unsigned char ***types);
	void SetTypes(unsigned char *types);

	void SetDimension(int x, int y, int z);
	void SetVoxelSize(float size);

	void GetDimension(int *x, int *y, int *z) const;
	float GetVoxelSize() const;

	void GenerateMesh(VoxelMesh* mesh);

	bool IsEmpty();
	bool IsValid() const;

	const Vector3& GetCenter();

protected:
	std::string _name;
	Voxel* _voxels;
	float _voxelSize;
	int _numElements;
	bool _empty;

	union {
		int _dimension[3];
		struct {
			int _sx;
			int _sy;
			int _sz;
		};
	};

	Vector3 _center;

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