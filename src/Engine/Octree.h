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

/*
* Class Octree is responsible for:
*	collision checking,
*	storing chunks,
*	frustum culling of the chunks.
*
* It divides world into smaller regions, wich are recursively divided into smaller regions until
* size constaint is hit. Helps to manager collision checking and chunk managing with better memory usage.	
*/
class Octree 
{
public:
	/* Initializes Octree as an empty and invalid node - not setting bounding box */
	Octree();
	/* Creates node with given area */
	Octree(const BoundingBox& area);

	/* Deletes chunks and chunk meshes stored in the node */
	~Octree();

	/* Set are of range for the node. Objects within this area can be classified to his node. */
	void SetBoundingArea(const BoundingBox& area);

	/* Get number of phsyical objects stored inside this node */
	int GetObjectsCount() const;
	/* Check if node has any child */
	bool HasChild() const;
	/* Check if node has parent */
	bool HasParent() const;
	/* Check if node is root */
	bool IsRoot() const;
	/* Check if node cannot be subdivided further */
	bool IsSmallestLeaf() const;

	/* Insert all enqueued objects into the tree */
	void UpdateTree();
	/* Update all existing objects which are stored inside the tree */
	void Update();

	/* Add object to the tree. It will be enqueued and added after calling UpdateTree()*/
	void Add(PhysicalObject* object);
	/* Add objects to the tree. It will be enqueued and added after calling UpdateTree()*/
	void Add(const PhysicalObjects& objects);

	/* Chunks must be allocated before inserting and octree will delete their resources in proper time */
	/* Add chunk to the tree. It will be enqueued and added after calling UpdateTree()*/
	void Add(Chunk* chunk);
	/* Add chunks to the tree. It will be enqueued and added after calling UpdateTree()*/
	void Add(const Chunks& chunks);

	/*
	* Try to insert voxel into given coordinates. However, if any object or voxel
	* is already occupying this place, nothing will happen.
	*/
	void Insert(const Voxel& voxel, Vector3 coordinates);

	/* Draw all chunks inside the tree */
	void Draw(Renderer* renderer);
	/* Draw bounding areas of the nodes */
	void DrawDebug(Renderer* renderer);

	/*
	* Check if given ray has collided with terrain and store results in intersectionInfo.
	* To reuse ray after calling this function, ray should be reseted.
	*/
	void CheckRayCollision(Ray *ray, RayIntersection* intersectionInfo);

	/* Get pointer to the root of the tree. */
	Octree* GetRoot();

	/* Convert Octree to string - respects only physical objects, not chunks. Lvl should be left with default value */
	std::string ToString(int lvl = 0) const;
private:
	BoundingBox _area;		/* Bounding area of the node, onlt objects inside this area are belonging to this node */

	Octree *_parent;		/* Pointer to the parent node */
	Octree *_children[8];	/* All children of the octree */
	
	/* Physical objects variables */
	PhysicalObjectsQueue _pendingObjects;	/* Queue for objects that have been added through Add method. */
	PhysicalObjects _objects;				/* Objects inside the node */
	uint8_t _physicChildren;				/* Bitfield indicating which children have objects inside or their children have objects etc. */

	/* Chunks variables */
	ChunksQueue _pendingChunks;	/* Chunks pending for adding into the queue, added though Add method. */
	Chunks _chunks;				/* Chunks that are stored in this node and have to be delivered to children. Used only during initial build. This do not 
								   have to be list, because all chunks will be delivered to child nodes for sure, until only one is left */
	Chunk* _chunk;				/* Chunk assigned to this node. In other nodes than smallest ones, this should be nullptr. Empty chunks are stored as nullptr too */
	VoxelMesh* _chunkMesh;		/* Mesh for the chunk. */
	uint8_t _chunkChildren;		/* Bitfield indicating which branches are containing any not empty chunks. */

	/* Time constrains */
	int _availableLifetime;		/* Lifetime that will be assigned to timeToLive when node will become empty. It wil be increased each time the node is used. */
	int _timeToLive;			/* When this hits 0, node will be deleted if empty. */

	/* Constant variables */
	const float _minimumSize = (float)Chunk::dimension; /* Minimum dimension of the node. Chunk size is good if chunk is not too big. */
	const int _initialAvailableLifetime = 8;			/* Initial value for available lifetime */
	const int _maximumLifetime = 64;					/* Maximum availableLifetime value*/

	static bool _built; /* Indicates that tree has been built for the first time */

	/* Constructs node with given area and object list */
	Octree(const BoundingBox& area, const PhysicalObjects& objects);
	/* Constructs node with given area and chunk list */
	Octree(const BoundingBox& area, const Chunks& chunks);

	/* Builds tree for the first time - it will try to push each object as deep as it can go */
	void BuildTree();

	/* Allocate new child node with given area and objects */
	Octree* CreateChildNode(const BoundingBox& area, const PhysicalObjects& objects);
	/* Allocate new child node with given area and object */
	Octree*	CreateChildNode(const BoundingBox& area, PhysicalObject *object);

	/* Allocate new child node with given area and chunks */
	Octree* CreateChildNode(const BoundingBox& area, const Chunks& chunks);
	/* Allocate new child node with given area and chunk. Also it can set model */
	Octree* CreateChildNode(const BoundingBox& area, Chunk* chunk, VoxelMesh* chunkMesh = nullptr);

	/* Calculate bounding boxes for children */
	void SubdivideNode(BoundingBox regions[8]);
	/* Insert object into the tree, it will try to go as far as it can */
	void Insert(PhysicalObject* object);
	/* Insert chunk and it's mesh into the tree, it must go to the smallest possible node */
	void Insert(Chunk* chunk, VoxelMesh* chunkMesh = nullptr);


	void BuildChunk(BoundingBox childAreas[8]);
	void BuildObject(BoundingBox childAreas[8]);
	void RemoveUnusedChildren();

	/* Recaulculates meshes for changed chunks and deletes empty chunks */
	void UpdateChunk();
	/* Checks collisions for the objects in the node */
	void CollisionCheck();
	/* Get list of all collision for the objects in the node and save info into infos (it is vector of the CollisionInfo) */
	void GetAllCollisions(CollisionsInfo* infos);

	/* Checks intersection of the object with the terrain and save info into info*/
	void CheckTerrainIntersections(PhysicalObject* object, CollisionInfo* info);
	/* Get list of all objects */
	void GetObjectsList(PhysicalObject* object, PhysicalObjectsVector* physicalObjects);
	
	/* Get list of all chunks colliding with given bounding box */
	void GetCollidingChunksList(const BoundingBox& box, Chunks* collidedChunks);

	/* Add node's bounding box border points into the vector, used for drawing */
	void AddLines(Vectors* lines);
};

inline void 
Octree::SetBoundingArea(const BoundingBox& area)
{
	_area = area;
}


inline int 
Octree::GetObjectsCount() const
{
	return _objects.size();
}

inline bool
Octree::IsSmallestLeaf() const
{
	const Vector3& dim = _area.GetDimension();
	/* In case node would not be cuboid */
	return (dim.x <= _minimumSize && dim.y <= _minimumSize && dim.z <= _minimumSize);
}

inline bool 
Octree::HasChild() const
{
	return (_chunkChildren | _physicChildren) != 0;
}

inline bool 
Octree::HasParent() const
{
	return _parent != nullptr;
}

inline bool 
Octree::IsRoot() const
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