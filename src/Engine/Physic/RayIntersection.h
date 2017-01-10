#pragma once

#include "Ray.h"
#include "BoundingBox.h"
#include "Engine/Objects/PhysicalObject.h"
#include "Resources/Voxels/Chunk.h"

namespace vengine {

/*
* Class keeping information about ray's collisions with terrain (currently).
*/
class RayIntersection {
public:
	/* Initialize collision info's with nullptrs */
	RayIntersection();

	/*
	* Test intersection with chunk by shooting ray, until:
	*  1. Collision with voxel occurs
	*  2. Ray will exceed chunk's coordinates range
	*  3. Ray will end
	*
	* @return True if collision has been detected, else false
	*/
	bool TestIntersection(Ray* ray, Chunk* chunk);
	
	/* Check if any collision has been found */
	bool CollisionFound() const;

	/* Get chunk that collided with ray */
	Chunk* GetCollidedChunk() const;
	/* Get collided voxel world coordinates */
	const Vector3& GetVoxelCoordinates() const;
private:
	Ray* _ray;					/* Pointer to the ray that had been tested */
	Chunk* _hittedChunk;		/* Pointer to the chunk that had been */
	Vector3 _voxelCoordinates;  /* Collided voxel world coordinates */
};

inline Chunk*
RayIntersection::GetCollidedChunk() const
{
	return _hittedChunk;
}

inline const Vector3&
RayIntersection::GetVoxelCoordinates() const
{
	return _voxelCoordinates;
}

inline bool 
RayIntersection::CollisionFound() const
{
	return _hittedChunk != nullptr;
}

inline
RayIntersection::RayIntersection() : _voxelCoordinates(Vector3::zeroes)
{
	_hittedChunk = nullptr;
	_ray = nullptr;
}

inline bool
RayIntersection::TestIntersection(Ray* ray, Chunk* chunk)
{
	assert(chunk != nullptr, "Trying to check collision with null chunk");
	assert(ray != nullptr, "Trying shoot null ray");
	_ray = ray;
	
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