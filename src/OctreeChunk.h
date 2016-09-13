#pragma once

#include "Chunk.h"
#include "BoundingBox.h"

#include <queue>
#include <list>

namespace vengine {

class OctreeChunk
{
public:
	typedef std::list<Chunk *> Chunks;
	typedef std::queue<Chunk *> ChunksQueue;


	OctreeChunk();
	~OctreeChunk();

	OctreeChunk(const BoundingBox& area);

	void SetBoundingArea(const BoundingBox& area);

	int GetChunksCount();
	bool HasChild();
	bool HasParent();
	bool IsRoot();
	bool IsSmallestLeaf();
	void UpdateTree();
	void Update();
	void Add(Chunk* chunk);
	void Add(const Chunks& chunks);

	void Draw(Renderer* renderer);

	//std::string ToString(int lvl = 0);

private:
	BoundingBox _area;

	OctreeChunk* _parent;
	OctreeChunk* _children[8];
	VoxelMesh* _chunkMesh;
	uint8_t _usedChildren;

	//PhysicalObjects _objects;
	ChunksQueue _pendingChunks;
	Chunks _chunks;


	int _availableLifetime;
	int _timeToLive;

	static bool _ready;
	static bool _built;

	const float _minimumSize = (float)Chunk::dimension;
	const int _initialMaxLifetime = 8;
	const int _maximumLifetime = 64;


	OctreeChunk(const BoundingBox& area, const Chunks& chunks);
	void BuildTree();
	OctreeChunk* CreateChildNode(const BoundingBox& area, const Chunks& objects);
	OctreeChunk* CreateChildNode(const BoundingBox& area, Chunk* object, VoxelMesh* chunkMesh = nullptr);
	void SubdivideNode(BoundingBox *regions);
	void Insert(Chunk* chunk, VoxelMesh* chunkMesh = nullptr);
};

inline void
OctreeChunk::SetBoundingArea(const BoundingBox& area)
{
	_area = area;
}


inline int
OctreeChunk::GetChunksCount()
{
	return _chunks.size();
}

inline bool
OctreeChunk::IsSmallestLeaf()
{
	const Vector3& dim = _area.GetDimension();
	return (dim.x <= _minimumSize && dim.y <= _minimumSize && dim.z <= _minimumSize);
}

inline bool
OctreeChunk::HasChild()
{
	return _usedChildren != 0;
}

inline bool
OctreeChunk::HasParent()
{
	return _parent != nullptr;
}

inline bool
OctreeChunk::IsRoot()
{
	return _parent == nullptr;
}

}