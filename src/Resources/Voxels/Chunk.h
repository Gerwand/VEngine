#pragma once

#include "VoxelArray3D.h"
#include "Math/Matrix4.h"

namespace vengine {

/* Extension of Voxel Array. It is cube, name is generated according to position and can be modified at the runtime. */
class Chunk : private VoxelArray3D {
public:
	Chunk();
	Chunk(const Vector3& offset);

	/* Set offset in world coordinates. This way we can spare some space by not storing offset in each voxel. */
	void SetOffset(const Vector3& offset);

	/* Set voxel using local chunk coordinates */
	void SetLocal(int x, int y, int z, unsigned char type);
	/* Set voxel using world coordinates */
	void Set(const Vector3& coordinates, unsigned char type);

	/* Get voxel using local chunk coordinates */
	const Voxel& GetLocal(int x, int y, int z) const;
	/* Get voxel using world coordinates*/
	const Voxel& Get(const Vector3& coordinates);

	/* Check if point is inside chunk. */
	bool IsInside(const Vector3& coordinates);

	const Vector3& GetOffset();
	
	/* Center of the chunk */
	const Vector3& GetCenter();
	const std::string& GetName() const;

	/* Check if chunk has changed since last mesh generation */
	bool HasChanged();
	/* Check if voxel have all NONE voxels */
	bool IsEmpty();

	/* Generate mesh for the chunk */
	void GenerateMesh(VoxelMesh* mesh);
	/* Get model matrix of the chunk */
	const Matrix4& GetModelMatrix();

	static const int dimension = 16;
private:
	bool _changed;			/* Check if chunk changed since last mesh generation */
	Vector3 _offset;		 /* Offset of the chunk in the world coordinates. Left lower corner. */
	Vector3 _constraintHigh; /* Maximum world coordinates that will not exceed chunk coordinates */

	Matrix4 _model;		/* Model matrix of the chunk */
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