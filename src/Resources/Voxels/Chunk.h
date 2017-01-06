#pragma once

#include "VoxelArray3D.h"
#include "Math/Matrix4.h"

namespace vengine {

class Chunk : private VoxelArray3D {
public:
	Chunk();
	Chunk(const Vector3& offset);

	void SetOffset(const Vector3& offset);
	void SetLocal(int x, int y, int z, unsigned char type);
	void Set(const Vector3& coordinates, unsigned char type);
	const Voxel& GetLocal(int x, int y, int z) const;
	const Voxel& Get(const Vector3& coordinates);
	bool IsInside(const Vector3& coordinates);

	const Vector3& GetOffset();
	const Vector3& GetCenter();
	const std::string& GetName() const;

	bool HasChanged();
	bool IsEmpty();
	void GenerateMesh(VoxelMesh* mesh);
	const Matrix4& GetModelMatrix();

	static const int dimension = 16;
private:
	bool _changed;
	Vector3 _offset;
	Vector3 _constraintHigh;

	Matrix4 _model;
};


inline bool 
Chunk::IsInside(const Vector3& coordinates)
{
	Vector3 coords(floor(coordinates.x), floor(coordinates.y), floor(coordinates.z));
	return (between(coords.x, _offset.x, _constraintHigh.x) &&
			between(coords.y, _offset.y, _constraintHigh.y) &&
			between(coords.z, _offset.z, _constraintHigh.z));
}


}