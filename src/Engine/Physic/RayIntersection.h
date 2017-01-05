#pragma once

#include "Ray.h"
#include "BoundingBox.h"
#include "Engine/PhysicalObject.h"
#include "Chunk.h"

namespace vengine {

class RayIntersection {
public:
	RayIntersection();

	//bool TestIntersection(const Ray& ray, PhysicalObject* object);
	bool TestIntersection(Ray* ray, Chunk* chunk);

	//bool IsSourceInside(const Ray& ray, const BoundingBox& box);
	
	bool CollisionFound();
	Chunk* GetCollidedChunk();
	const Vector3& GetVoxelCoordinates();
private:
	PhysicalObject* _hitted;
	Chunk* _hittedChunk;
	Vector3 _voxelCoordinates;
};

inline Chunk*
RayIntersection::GetCollidedChunk()
{
	return _hittedChunk;
}

inline const Vector3&
RayIntersection::GetVoxelCoordinates()
{
	return _voxelCoordinates;
}

inline bool 
RayIntersection::CollisionFound()
{
	return _hitted != nullptr || _hittedChunk != nullptr;
}

inline
RayIntersection::RayIntersection() : _voxelCoordinates(Vector3::zeroes)
{
	_hitted = nullptr;
	_hittedChunk = nullptr;
}

inline bool
RayIntersection::TestIntersection(Ray* ray, Chunk* chunk)
{
	assert(chunk != nullptr, "Trying to check collision with null chunk");

	
	Vector3 prevPos = Vector3(-1.0f);
	
	Vector3 constraintLow = chunk->GetOffset();
	Vector3 constraintHigh = constraintLow + (float)(Chunk::dimension - 1);

	while(!ray->HasEnded())	{
		const Vector3& rayPos = ray->Shoot();
		Vector3 coords(floor(rayPos.x), floor(rayPos.y), floor(rayPos.z));
		if (coords != prevPos) {
			Vector3 coords(floor(rayPos.x), floor(rayPos.y), floor(rayPos.z));
			if (between(coords.x, constraintLow.x, constraintHigh.x) &&
				between(coords.y, constraintLow.y, constraintHigh.y) &&
				between(coords.z, constraintLow.z, constraintHigh.z)) {
				prevPos = coords;

				const Voxel& voxel = chunk->Get(coords);
				if (!voxel.IsEmpty()) {
					_hittedChunk = chunk;
					_voxelCoordinates = coords;
					return true;
				}
			}
			else {
				return false;
			}
		}
	}

	return false;
}


}