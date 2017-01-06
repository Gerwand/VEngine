#pragma once

#include "Objects/PhysicalObject.h"
#include "Resources/Voxels/Chunk.h"
#include "Resources/Renderables/Lines.h"
#include "Engine/Physic/RayIntersection.h"

#include <queue>
#include <list>

namespace vengine {
typedef std::list<PhysicalObject *> PhysicalObjects;
typedef std::queue<PhysicalObject *> PhysicalObjectsQueue;
typedef std::vector<PhysicalObject *> PhysicalObjectsVector;

typedef std::queue<Chunk *> ChunksQueue;
typedef std::vector<Chunk *> Chunks;

typedef std::vector<CollisionInfo> CollisionsInfo;

class Octree 
{
public:
	Octree();
	Octree(const BoundingBox& area);
	~Octree();

	void SetBoundingArea(const BoundingBox& area);

	int GetObjectsCount();
	bool HasChild();
	bool HasParent();
	bool IsRoot();
	bool IsSmallestLeaf();

	void UpdateTree();
	void Update();

	void Add(PhysicalObject* object);
	void Add(const PhysicalObjects& objects);

	void Add(Chunk* chunk);
	void Add(const Chunks& chunks);

	void Insert(const Voxel& voxel, Vector3 coordinates);

	void Draw(Renderer* renderer);
	void DrawDebug(Renderer* renderer);

	void CheckRayCollision(Ray *ray, RayIntersection* intersectionInfo);

	Octree* GetRoot();

	std::string ToString(int lvl = 0);
private:
	BoundingBox _area;

	Octree *_parent;
	Octree *_children[8];
	
	//Physical objects variables
	PhysicalObjectsQueue _pendingObjects;
	PhysicalObjects _objects;
	uint8_t _physicChildren;

	//Chunks variables
	ChunksQueue _pendingChunks;
	Chunks _chunks;
	Chunk* _chunk;
	VoxelMesh* _chunkMesh;
	uint8_t _chunkChildren;


	//Time constrains
	int _availableLifetime;
	int _timeToLive;

	//Constant variables
	const float _minimumSize = (float)Chunk::dimension;
	const int _initialMaxLifetime = 8;
	const int _maximumLifetime = 64;

	static bool _built;

	//Private methods
	Octree(const BoundingBox& area, const PhysicalObjects& objects);
	Octree(const BoundingBox& area, const Chunks& chunks);

	void BuildTree();

	Octree* CreateChildNode(const BoundingBox& area, const PhysicalObjects& objects);
	Octree*	CreateChildNode(const BoundingBox& area, PhysicalObject *object);

	Octree* CreateChildNode(const BoundingBox& area, const Chunks& objects);
	Octree* CreateChildNode(const BoundingBox& area, Chunk* object, VoxelMesh* chunkMesh = nullptr);

	void SubdivideNode(BoundingBox regions[8]);
	void Insert(PhysicalObject* object);
	void Insert(Chunk* chunk, VoxelMesh* chunkMesh = nullptr);


	void UpdateChunk();
	void UpdateObject();
	void BuildChunk(BoundingBox childAreas[8]);
	void BuildObject(BoundingBox childAreas[8]);
	void RemoveUnusedChildren();
	void CollisionCheck();

	void GetAllCollisions(CollisionsInfo* infos);
	void CheckTerrainIntersections(PhysicalObject* object, CollisionInfo* info);
	void GetObjectsList(const BoundingBox& box, PhysicalObjectsVector* physicalObjects);
	void GetCollidingChunksList(const BoundingBox& box, Chunks* collidedChunks);

	void AddLines(Vectors* lines);
};

inline void 
Octree::SetBoundingArea(const BoundingBox& area)
{
	_area = area;
}


inline int 
Octree::GetObjectsCount()
{
	return _objects.size();
}

inline bool
Octree::IsSmallestLeaf()
{
	const Vector3& dim = _area.GetDimension();
	return (dim.x <= _minimumSize && dim.y <= _minimumSize && dim.z <= _minimumSize);
}

inline bool 
Octree::HasChild()
{
	return (_chunkChildren | _physicChildren) != 0;
}

inline bool 
Octree::HasParent()
{
	return _parent != nullptr;
}

inline bool 
Octree::IsRoot()
{
	return _parent == nullptr;
}

inline Octree* 
Octree::GetRoot()
{
	Octree* node = this;
	while (!node->IsRoot()) {
		node = node->_parent;
	}

	return node;
}

}