#pragma once


#include "Engine/GameObject.h"
#include "Chunk.h"
namespace vengine {
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

class Octree;

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

	CollisionInfo();
	CollisionInfo(Octree* node);

	void SetCollisionObject(GameObject* source, GameObject* other);

	//void AddCollisionChunk(Chunk* chunk);
	//void AddCollisionChunk(const Chunks& chunks);

	//void AddVoxelCoord(const Vector3& coord);
	//void AddVoxelCoords(const Vectors& coords);

	void AddHitDirection(const Directions& direction);
	void SetHitDirection(const Directions& direction);

	void SetNode(Octree* node);

	bool HasCollided() const;

	bool HasCollidedWithTerrain(unsigned char sides) const;

	bool HasCollidedWithTerrain() const;

	bool HasCollidedWithObject() const;

	GameObject* GetOtherObject() const;
	GameObject* GetSourceObject() const;
	//const Chunks& GetHittedChunks();
	const Directions& GetHittedDirections() const;
	//const Directions& GetHittedVoxelsCoords();

	Octree* GetCollisionNode() const;
private:

	//Chunks _hittedChunks;
	//Vectors _voxelCoords;
	Directions _directions;

	GameObject* _sourceObject;
	GameObject* _otherObject;

	Octree* _node;
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