#pragma once

#include "Engine/Objects/GameObject.h"
#include "Resources/Voxels/Chunk.h"

namespace vengine {

/*
 * Union representing directions as a bitfields to preserve space
 * and allow for quickly call proper directions using bit operations. 
 */
union Directions {
	Directions() : directions(0) {};
	Directions(const Directions& source) { directions = source.directions; };
	const Directions& operator=(const Directions& source) { directions = source.directions; return *this; }

	struct {
		unsigned char top : 1;
		unsigned char bottom : 1;
		unsigned char north : 1;
		unsigned char south : 1;
		unsigned char east : 1;
		unsigned char west : 1;
	};
	unsigned char directions;
};

typedef std::vector<Vector3> Vectors;
typedef std::vector<Chunk *> Chunks;
typedef std::vector<Directions> DirectionsVector;

/* 
 * Predeclaration, cause we want to store pointer to the node
 * in case we would like to insert object when collision occurs 
 */
class Octree;

/*
* 
*/
class CollisionInfo
{
public:
	enum HitDirections {
		TOP = 0x01,
		BOTTOM = 0x02,
		NORTH = 0x04,
		SOUTH = 0x08,
		EAST = 0x10,
		WEST = 0x20
	};

	/* Creates info with empty values */
	CollisionInfo();
	/* Creates info with empty values, adding pointer to the Octree's node */
	CollisionInfo(Octree* node);

	/* Set objects that had collided */
	void SetCollisionObject(GameObject* source, GameObject* other);

	/* Add side of the AABB where it collided with terrain */
	void AddHitDirection(const Directions& direction);
	/* Add side of the AABB where it collided with terrain */
	void SetHitDirection(const Directions& direction);

	/* Set Octree's node as a staring point for inserting objects into it in collision handlers */
	void SetNode(Octree* node);

	/* Check if info object keeps info about any collision */
	bool HasCollided() const;

	/* Check if object has collided with the terrain from any of the given sides */
	bool HasCollidedWithTerrain(unsigned char sides) const;
	/* Check if object has collided with the terrain from any side */
	bool HasCollidedWithTerrain() const;

	/* Check if object collided with other object */
	bool HasCollidedWithObject() const;

	/* Get pointer to the other object */
	GameObject* GetOtherObject() const;
	/* Get object that collided with other object (in collision handler it will be equal to 'this' poitner) */
	GameObject* GetSourceObject() const;

	/* Get directions of collisions with terrain */
	const Directions& GetHittedDirections() const;

	/* Get node at which collision had been detected */
	Octree* GetCollisionNode() const;
private:
	Directions _directions;		/* Directions of the collision with terrain to give collision handler additional info */

	GameObject* _sourceObject;	/* Pointer to the source object */
	GameObject* _otherObject;	/* Pointer to the other object */

	Octree* _node;				/* Pointer to the node where collision occured */
};

inline
CollisionInfo::CollisionInfo()
{
	_sourceObject = nullptr;
	_otherObject = nullptr;
	_node = nullptr;
}

inline
CollisionInfo::CollisionInfo(Octree* node) : CollisionInfo()
{
	_node = node;
}

inline void
CollisionInfo::SetCollisionObject(GameObject* source, GameObject* other)
{
	_sourceObject = source;
	_otherObject = other;
}

inline void
CollisionInfo::AddHitDirection(const Directions& direction)
{
	_directions.directions |= direction.directions;
}

inline void
CollisionInfo::SetHitDirection(const Directions& direction)
{
	_directions = direction;
}

inline void
CollisionInfo::SetNode(Octree* node)
{
	_node = node;
}

inline bool
CollisionInfo::HasCollided() const
{
	return _directions.directions != 0 || _otherObject != 0;
}

inline bool
CollisionInfo::HasCollidedWithTerrain(unsigned char sides) const
{
	return (_directions.directions & sides) != 0;
}

inline bool
CollisionInfo::HasCollidedWithTerrain() const
{
	return _directions.directions != 0;
}

inline bool
CollisionInfo::HasCollidedWithObject() const
{
	return _otherObject != nullptr;
}

inline GameObject*
CollisionInfo::GetOtherObject() const
{
	return _otherObject;
}

inline GameObject*
CollisionInfo::GetSourceObject() const
{
	return _sourceObject;
}

inline const Directions& 
CollisionInfo::GetHittedDirections() const
{
	return _directions;
}

inline Octree* 
CollisionInfo::GetCollisionNode() const
{
	return _node;
}

}