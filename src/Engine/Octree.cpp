#include "Octree.h"

namespace vengine {

bool Octree::_built = false;

Octree::Octree() :
	_children{}, _area(Vector3::zeroes, Vector3::zeroes)
{
	_timeToLive = -1;
	_availableLifetime = _initialAvailableLifetime;
	_chunkChildren = 0;
	_physicChildren = 0;
	_chunkMesh = nullptr;
	_chunk = nullptr;
	_parent = nullptr;
}

Octree::Octree(const BoundingBox& area, const PhysicalObjects& objects) :
	_children{}, _area(area), _objects(objects)
{
	_timeToLive = -1;
	_availableLifetime = _initialAvailableLifetime;
	_chunkChildren = 0;
	_physicChildren = 0;
	_chunkMesh = nullptr;
	_chunk = nullptr;
	_parent = nullptr;
}

Octree::Octree(const BoundingBox& area) :
	_children{}, _area(area)
{
	_timeToLive = -1;
	_availableLifetime = _initialAvailableLifetime;
	_chunkChildren = 0;
	_physicChildren = 0;
	_chunkMesh = nullptr;
	_chunk = nullptr;
	_parent = nullptr;
}

Octree::Octree(const BoundingBox& area, const Chunks& chunks) :
	_parent(NULL), _children{},
	_area(area), _chunks(chunks)
{
	_timeToLive = -1;
	_availableLifetime = _initialAvailableLifetime;
	_chunkChildren = 0;
	_physicChildren = 0;
	_chunkMesh = nullptr;
	_chunk = nullptr;
	_parent = nullptr;
}

Octree::~Octree()
{
	/* And delete chunk resources if existing */
	delete _chunk;
	delete _chunkMesh;
}

void
Octree::UpdateTree()
{
	/*
	* For the first time we are building tree naively, from scratch,
	* checking each object with each child and subdiving each node only once
	*/
	if (!_built) {
		/* Add objects... */
		while (!_pendingObjects.empty()) {
			_objects.push_back(_pendingObjects.front());
			_pendingObjects.pop();
		}
		/* Add chunks... */
		while (!_pendingChunks.empty()) {
			_chunks.push_back(_pendingChunks.front());
			_pendingChunks.pop();
		}

		BuildTree();
		_built = true;
	}
	/* If tree is built, there should not be any objects waiting for inserting */
	else {
		/* Add objects... */
		while (!_pendingObjects.empty()) {
			Insert(_pendingObjects.front());
			_pendingObjects.pop();
		}
		/* Add chunks... */
		while (!_pendingChunks.empty()) {
			Insert(_pendingChunks.front());
			_pendingChunks.pop();
		}
	}
}



void
Octree::BuildChunk(BoundingBox childAreas[8])
{
	/* Representing childs containing chunks that fits into them */
	Chunks childChunks[8];

	/* For each chunk we want to find proper child */
	for (Chunks::iterator it = _chunks.begin(); it != _chunks.end(); ++it) {
		Chunk* chunk = *it;

		/* We are not inserting empty chunks */
		if (chunk->IsEmpty()) {
			delete (*it);
			continue;
		}

		/* We do not have to check collisions with the whole chunk, we can check if its center point is inside child's area */
		Vector3 center = chunk->GetCenter() + chunk->GetOffset();
		bool fits = false;
		/* Check to which child it belongs */
		for (int i = 0; i < 8; i++) {
			if (childAreas[i].IsContaining(center)) {
				/* Add chunk to the proper children */
				childChunks[i].push_back(chunk);
				fits = true;
				break;
			}
		}
		/* If nowhere, chunk was invalid, we can delete it */
		if (!fits) 
			delete (*it);
	}

	/* Now we must create nodes to store chunks into them */
	for (int i = 0; i < 8; ++i) {
		if (!childChunks[i].empty()) {
			_children[i] = CreateChildNode(childAreas[i], childChunks[i]);
			/* Set bitfield that this child is holding non empty chunk */
			_chunkChildren |= (uint8_t)(1 << i);
		}
	}

	_chunks.clear();
}

void
Octree::BuildObject(BoundingBox childAreas[8])
{
	/* If there is only one object, we can stop */
	if (_objects.size() <= 1)
		return;

	PhysicalObjects childObjects[8];

	/* Check if object can fit in only one subdivided region */
	for (PhysicalObjects::iterator it = _objects.begin(); it != _objects.end();) {
		PhysicalObject *phys = *it;
		bool fitted = false;
		for (int i = 0; i < 8; i++) {
			if (childAreas[i].IsContaining(phys->GetCollider())) {
				childObjects[i].push_back(phys);
				it = _objects.erase(it);
				fitted = true;
				break;
			}
		}
		/* If it cannot fit into any region, it must stay there */
		if (!fitted)
			++it;
	}

	/* Check if any object can be passed down */
	for (int i = 0; i < 8; ++i) {
		if (!childObjects[i].empty()) {
			/* Child could exist, because we inserted chunks earlier */
			if (_children[i] == nullptr) {
				/* If there was no child, create one */
				_children[i] = CreateChildNode(childAreas[i], childObjects[i]);
			}
			else {
				/* If there was children, only pass objects without creation */
				_children[i]->_objects = childObjects[i];
			}

			/* Set proper bit to indicate that children have objects inside */
			_physicChildren |= (uint8_t)(1 << i);
		}
	}
}

void
Octree::BuildTree()
{
	if (IsSmallestLeaf()) {
		/* There can be only one chunk in each node */
		assert(_chunks.size() <= 1, "Too many chunks in the leaf node: %d", _chunks.size());

		/* If there is no chunk, we can exit */
		if (_chunks.empty())
			return;

		/* Add chunk from the list */
		_chunk = _chunks.back();
		_chunks.clear();

		assert(_chunkMesh == nullptr, "Mesh should be nullptr when creating tree, there is: %s", _chunkMesh->GetName().c_str());
		
		_chunkMesh = new VoxelMesh;
		
		_chunkMesh->Init(_chunk->GetName());
		_chunk->GenerateMesh(_chunkMesh);
	}

	BoundingBox childAreas[8];
	/* Subdivide only once and pass it as argument to both chunks and child areas */
	SubdivideNode(childAreas);

	/* First build chunks */
	BuildChunk(childAreas);
	/* Then build objects */
	BuildObject(childAreas);

	/* Run build tree for created children */
	for (uint8_t used = _chunkChildren | _physicChildren, i = 0; used > 0; used >>= 1, ++i)
		if (used & 1)
			_children[i]->BuildTree();

}

Octree*
Octree::CreateChildNode(const BoundingBox& area, const PhysicalObjects& objects)
{
	if (objects.empty())
		return nullptr;

	Octree *child = new Octree(area, objects);
	child->_parent = this;

	return child;
}

Octree*
Octree::CreateChildNode(const BoundingBox& area, PhysicalObject *object)
{
	Octree *child = new Octree(area);
	child->_objects.push_back(object);
	child->_parent = this;

	return child;
}

Octree*
Octree::CreateChildNode(const BoundingBox& area, const Chunks& chunks)
{
	if (chunks.empty())
		return nullptr;

	Octree *child = new Octree(area, chunks);
	child->_parent = this;

	return child;
}

Octree*
Octree::CreateChildNode(const BoundingBox& area, Chunk *chunk, VoxelMesh* chunkMesh)
{
	Octree *child = new Octree(area);
	
	assert(child->IsSmallestLeaf(), "Cannot create child node with chunk if it is not smallest leaf");
	
	child->_chunk = chunk;
	child->_chunkMesh = chunkMesh;

	child->_parent = this;

	return child;
}

void
Octree::UpdateChunk()
{
	if (_chunk == nullptr)
		return;

	/*Check if chunk is empty and delete it if so */
	if (_chunk->IsEmpty()) {
		delete _chunk;
		delete _chunkMesh;

		_chunk = nullptr;
		_chunkMesh = nullptr;
		return;
	}

	/* Generate mesh if chunk has changed */
	if (_chunk->HasChanged()) {
		if (_chunkMesh == nullptr) {
			_chunkMesh = new VoxelMesh;
			_chunkMesh->Init(_chunk->GetName());
		}
		_chunk->GenerateMesh(_chunkMesh);
	}

}

void
Octree::RemoveUnusedChildren()
{
	uint8_t usedChildren = _physicChildren | _chunkChildren;

	for (uint8_t used = usedChildren, i = 0; used > 0; used >>= 1, ++i) {
		/* If branch timed out, delete it */
		if ((used & 1) && _children[i]->_timeToLive == 0) {
			delete _children[i];
			_children[i] = nullptr;

			_chunkChildren &= ~(uint8_t)(1 << i);
			_physicChildren &= ~(uint8_t)(1 << i);
		}
	}

	/* Check for unused chunk nodes */
	for (uint8_t used = _chunkChildren, i = 0; used > 0; used >>= 1, ++i) {
		if (used & 1) {
			/*
			* If child node does not have any chunk children and chunk is null, indicate
			* in bitfield that it is not storing any chunks
			*/
			if (_children[i]->_chunkChildren == 0 && _children[i]->_chunk == nullptr) {
				_chunkChildren &= ~(uint8_t)(1 << i);
			}
		}
	}

	/* Check for unused physic objects nodes */
	for (uint8_t used = _physicChildren, i = 0; used > 0; used >>= 1, ++i) {
		if (used & 1) {
			/*
			 * If child node does not have any physical object children and there is no
			 * any object inside, indicate in bitfield that this child is not storing any objects
			 */
			if (_children[i]->_physicChildren == 0 && _children[i]->_objects.empty()) {
				_physicChildren &= ~(uint8_t)(1 << i);
			}
		}
	}
}

void
Octree::Update()
{
	assert(_built, "Cannot update not built tree.");

	/* If there are no any items in the node and it has no children, start counting down it's lifetime */
	if (_objects.empty() && _chunk == nullptr) {
		if (!HasChild()) {
			/* Initialize for the first time */
			if (_timeToLive == -1)
				_timeToLive = _availableLifetime;
			else if (_timeToLive > 0)
				--_timeToLive;
		}
	}
	/* Else, we must double the maximum lifetime */
	else {
		if (_timeToLive != -1) {
			if (_availableLifetime < _maximumLifetime)
				_availableLifetime *= 2;
			_timeToLive = -1;
		}
	}

	/* Update all chunks before physical objects */
	UpdateChunk();

	/*
	 * Get list of all changed objects, do it before updating position in nodes of 
	 * all objects to do not check the same object two times
	 */
	PhysicalObjects changed;

	/*
	 * If both children are the same, we can only check moved children, because last time it was checked, they were good.
	 * However, if there are new nodes created when adding, we must check all objects to ensure that they will go into new nodes.
	 */
	bool childChanged = _physicChildren < _chunkChildren;
	for (PhysicalObjects::iterator it = _objects.begin(); it != _objects.end();) {
		PhysicalObject* phys = *it;

		/* Check is object is still alive */
		if (phys->IsDestroyed()) {
			it = _objects.erase(it);
			continue;
		}
		else {
			++it;
		}

		if (phys->HasChanged() || childChanged)
			changed.push_back(phys);
	}

	/* Insert again changed objects */
	for (PhysicalObjects::iterator it = changed.begin(); it != changed.end(); ++it) {
		PhysicalObject* phys = *it;
		Octree *node = this;

		/* Push object up if it is no more fitting into node */
		while (!node->_area.IsContaining(phys->GetCollider()))
			if (node->HasParent())
				node = node->_parent;
			else
				break;

		/* Remove pushed object */
		for (PhysicalObjects::iterator pos = _objects.begin(); pos != _objects.end(); ++pos) {
			if (*pos == phys) {
				pos = _objects.erase(pos);
				break;
			}
		}

		/* Insert it into the parent */
		node->Insert(phys);
	}

	/* Update all used children nodes */
	uint8_t usedChildren = _physicChildren | _chunkChildren;
	for (uint8_t used = usedChildren, i = 0; used > 0; used >>= 1, ++i)
		if (used & 1) {
			assert(_children[i] != nullptr, "Children is null!");
			_children[i]->Update();

		}

	/* Update bitfields and remove unused branches */
	RemoveUnusedChildren();

	CollisionCheck();
}

void
Octree::GetAllCollisions(CollisionsInfo* infos)
{
	/* Check all objects from current node with objects from all child nodes */
	for (PhysicalObjects::iterator it = _objects.begin(); it != _objects.end(); ++it) {
		PhysicalObject* srcObj = *it;
		PhysicalObjectsVector objects;
		/* Get list of objects colliding with this object */
		GetObjectsList(srcObj, &objects);

		for (PhysicalObjectsVector::iterator itObj = objects.begin(); itObj != objects.end(); ++itObj) {
			PhysicalObject* colObj = *itObj;
			if (colObj == srcObj)
				continue;
			if (srcObj->GetCollider().IsColliding(colObj->GetCollider())) {
				CollisionInfo info(this);
				info.SetCollisionObject(srcObj, colObj);
				infos->push_back(info);

				/* If collided object is from child node, we should run collision against this object */
				if (std::find(_objects.begin(), _objects.end(), colObj) == _objects.end()) {
					info.SetCollisionObject(colObj, srcObj);
					infos->push_back(info);
				}
			}
		}

		CollisionInfo info(this);
		info.SetCollisionObject(srcObj, nullptr);
		CheckTerrainIntersections(srcObj, &info);

		if(info.HasCollidedWithTerrain())
			infos->push_back(info);
	}
}

void 
Octree::CollisionCheck()
{
	CollisionsInfo infos;
	GetAllCollisions(&infos);

	for (CollisionsInfo::iterator it = infos.begin(); it != infos.end(); ++it) {
		const CollisionInfo& info = *it;
		PhysicalObject *obj = (PhysicalObject *)(info.GetSourceObject());
		obj->OnCollision(info);
	}
}

void
Octree::Insert(PhysicalObject* object)
{
	/* If it is first item and it does not have any children */
	if (_objects.empty() && !HasChild()) {
		_objects.push_back(object);
		return;
	}

	/* If we cannot go any further put it into node */
	if (IsSmallestLeaf()) {
		_objects.push_back(object);
		return;
	}

	BoundingBox childAreas[8];
	SubdivideNode(childAreas);

	/* Check if object is within game range */
	if (_area.IsContaining(object->GetCollider())) {

		bool fits = false;

		/* Check inside each child node */
		for (int i = 0; i < 8; ++i) {
			if (childAreas[i].IsContaining(object->GetCollider())) {
				/* If node is already existing, just insert it */
				if (_children[i] != nullptr) {
					_children[i]->Insert(object);
				}
				/* In other case, we must create child node */
				else {
					_children[i] = CreateChildNode(childAreas[i], object);
				}
				_physicChildren |= (uint8_t)(1 << i);

				fits = true;
			}
		}
		/* If it did not fit in any child, store it here */
		if (!fits)
			_objects.push_back(object);
	}
	else if (IsRoot()) {
		/* Teleport player to the random spawn point if he is outside game range */
		if (object->CompareTag("Player")) {
			object->SetTransform(Transform().SetPosition(Vector3(0.0f, 50.0f, 0.0f)));
			Insert(object);			
		}
		/* Destroy all other objects */
		else
			GameObject::Destroy(object);
	}
} 


void
Octree::Insert(Chunk* chunk, VoxelMesh* chunkMesh)
{
	/*
	 * We want to insert chunk as deep as we can, they will not change as dynamically
	 * as physical objects, and it will be easier to check collisions that way.
	 */
	if (IsSmallestLeaf()) {
		assert(_chunk != nullptr, "There is already chunk in that node: %s", _chunk->GetName().c_str());
		_chunk = chunk;
		return;
	}

	BoundingBox childAreas[8];
	SubdivideNode(childAreas);

	/*
	 * Because of octree minimum size is equal to chunk dimension,
	 * we can only check one point to determine location.
	 */
	Vector3 chunkCenter = chunk->GetCenter() + chunk->GetOffset();

	/* Check if chunk is within game range */
	if (_area.IsContaining(chunkCenter)) {
		bool fits = false;

		/* Check inside each child node */
		for (int i = 0; i < 8; ++i) {
			if (childAreas[i].IsContaining(chunkCenter)) {
				/* If node is already existing, just insert it */
				if (_children[i] != nullptr) {
					_children[i]->Insert(chunk, chunkMesh);
				}
				/* In other case, we must create child node */
				else {
					_children[i] = CreateChildNode(childAreas[i], chunk, chunkMesh);
					if (!_children[i]->IsSmallestLeaf())
						_children[i]->Insert(chunk);
				}
				_chunkChildren |= (uint8_t)(1 << i);

				fits = true;
				break;
			}
		}
#ifdef VE_DEBUG
		/* If it did not fit in any child, there had to be an error */
		if (!fits)
			assert(false, "Chunk out of bound of the octree");
#endif
	}
	else if (IsRoot()) {
		/* Inform about chunk being outside game range, and as for now do nothing */
		assert(false, "Chunk %s out of playable area", chunk->GetName().c_str());
	}
}


void
Octree::SubdivideNode(BoundingBox regions[8])
{
	const Vector3& center = _area.GetPosition();
	Vector3 half = _area.GetDimension() / 2.0f;
	Vector3 quart = half / 2.0f;

	/*
	* Order of regions is not random. It was sorted that way, to get proper chunks list when checking
	* collisions of the AABB with the terrain. One point can sometimes be contained by many chunks AABBs,
	* and we are taking first one, so order is important.
	*/
	regions[0].Set(Vector3(center.x + quart.x, center.y + quart.y, center.z + quart.z), half);
	regions[1].Set(Vector3(center.x - quart.x, center.y + quart.y, center.z + quart.z), half);

	regions[2].Set(Vector3(center.x + quart.x, center.y + quart.y, center.z - quart.z), half);
	regions[3].Set(Vector3(center.x - quart.x, center.y + quart.y, center.z - quart.z), half);


	regions[4].Set(Vector3(center.x + quart.x, center.y - quart.y, center.z + quart.z), half);
	regions[5].Set(Vector3(center.x - quart.x, center.y - quart.y, center.z + quart.z), half);

	regions[6].Set(Vector3(center.x + quart.x, center.y - quart.y, center.z - quart.z), half);
	regions[7].Set(Vector3(center.x - quart.x, center.y - quart.y, center.z - quart.z), half);

}

void
Octree::Draw(Renderer* renderer)
{
	CameraFPP *camera = renderer->GetActiveCamera();

	/* Do not draw invisible chunks */
	if (!camera->IsVisible(_area)) {
		return;
	}

	if (_chunk != nullptr) {
		assert(_chunkMesh != nullptr, "%s mesh not existing", _chunk->GetName().c_str());
		renderer->SetModelMatrix(_chunk->GetModelMatrix());
		_chunkMesh->Draw(renderer);
	}

	/* Render only chunks */
	for (uint8_t used = _chunkChildren, i = 0; used > 0; used >>= 1, ++i)
		if (used & 1)
			_children[i]->Draw(renderer);

}


/* Helping function */
static inline std::string toBinary(uint8_t num)
{
	uint8_t i;
	std::string out;
	for (i = 0x80; i != 0; i >>= 1)
		out += (num&i) ? '1' : '0';

	return out;
}

std::string
Octree::ToString(int lvl) const
{
	std::string output;
	std::string indent;
	for (int i = 0; i <= lvl; ++i)
		indent += "\t>";

	output += indent;
	output += "Octree lvl: " + std::to_string(lvl);
	output += "\n" + indent + "Time to live: " + std::to_string(_timeToLive);
	output += "\n" + indent + "BoundingBox center, dimension: ";
	output += _area.GetPosition().ToString() + _area.GetDimension().ToString();
	output += "\n" + indent + "List of objects:\n";
	for (PhysicalObjects::const_iterator it = _objects.begin(); it != _objects.end(); ++it)
		output += indent + (*it)->GetName() + " " + (*it)->GetTransform().GetPosition().ToString() + "\n";
	output += indent + "End of objects\n";
	output += indent + "Children: " + toBinary(_physicChildren | _chunkChildren) + "\n";

	for (uint8_t used = _physicChildren | _chunkChildren, i = 0; used > 0; used >>= 1, ++i)
		if (used & 1)
			output += _children[i]->ToString(lvl + 1);

	return output;
}


void
Octree::Add(PhysicalObject* object)
{
	assert(object->GetCollider().IsValid(), "Invalid dimension of the objects collider: %s",
		   object->GetCollider().GetDimension().ToString().c_str());
	_pendingObjects.push(object);
}

void
Octree::Add(const PhysicalObjects& objects)
{
	for (PhysicalObjects::const_iterator it = objects.begin(); it != objects.end(); ++it) {
		PhysicalObject* obj = *it;
		assert(obj->GetCollider().IsValid(), "Invalid dimension of the objects collider: %s",
			   obj->GetCollider().GetDimension().ToString().c_str());
		_pendingObjects.push(obj);
	}
}

void
Octree::AddLines(Vectors* lines)
{
	_area.InsertLines(lines);

	for (uint8_t used = _chunkChildren | _physicChildren, i = 0; used > 0; used >>= 1, ++i)
		if (used & 1)
			_children[i]->AddLines(lines);
}

void
Octree::DrawDebug(Renderer* renderer)
{
	Lines boxes;
	std::vector<Vector3> lines;
	boxes.Init();
	AddLines(&lines);

	boxes.SetColor(Vector3(0.0f, 0.0f, 0.0f));

	for (std::vector<Vector3>::iterator it = lines.begin(); it < lines.end();) {
		const Vector3& start = *it;
		++it;
		const Vector3& end = *it;
		++it;
		boxes.AddLine(start, end);
	}


	renderer->SetModelMatrix(Matrix4::identity);
	boxes.Draw(renderer);
}

void
Octree::Add(Chunk* chunk)
{
	_pendingChunks.push(chunk);
}

void
Octree::Add(const Chunks& chunks)
{
	for (Chunks::const_iterator it = chunks.begin(); it != chunks.end(); ++it) {
		Chunk* chunk = *it;
		_pendingChunks.push(chunk);
	}
}


void
Octree::CheckRayCollision(Ray *ray, RayIntersection* intersectionInfo)
{
	/* Check collision with chunk if we are at the smallest leaft */
	if (IsSmallestLeaf()) {
		/* If there is no chunk, we want to shoot our ray, until it will either end, or stop fitting in the area */
		if (_chunk == nullptr) {
			while (!ray->HasEnded() && _area.IsContaining(ray->Advance(0.25f)));
		}
		else {
			intersectionInfo->TestIntersection(ray, _chunk);
		}

		if (ray->HasEnded() || intersectionInfo->CollisionFound()) {
			return;
		}
		else {
			Octree *node = this;

			/* Push object up if it is no more fitting into node */
			while (!node->_area.IsContaining(ray->GetCurrentPosition()))
				if (node->HasParent())
					node = node->_parent;
				else
					break;

			/* Insert it into the parent */
			node->CheckRayCollision(ray, intersectionInfo);
		}

		return;
	}

	BoundingBox childAreas[8];
	SubdivideNode(childAreas);


	/* We must determine which child is holding us and */
	bool fitted = false;
	while (!fitted && !ray->HasEnded() && !intersectionInfo->CollisionFound()) {
		Vector3 rayPos = ray->GetCurrentPosition();

		/* If we are still into the area, check collision with children */
		if (_area.IsContaining(rayPos)) {
			if (HasChild())
				for (uint8_t used = _physicChildren | _chunkChildren, i = 0; used > 0; used >>= 1, ++i)
					if (used & 1)
						if (childAreas[i].IsContaining(rayPos)) {
							_children[i]->CheckRayCollision(ray, intersectionInfo);
							return;
						}

			ray->Advance(0.25f);
		}
		/* If we are not fitting, find the best parent */
		else {
			Octree *node = this;

			/* Push object up if it is no more fitting into node */
			while (!node->_area.IsContaining(ray->GetCurrentPosition()))
				if (node->HasParent())
					node = node->_parent;
				else
					break;

			/* If ray is out of game range, just drop it */
			if (node->IsRoot() && !node->_area.IsContaining(ray->GetCurrentPosition()))
				return;

			/* Insert it into the parent */
			node->CheckRayCollision(ray, intersectionInfo);
			return;
		}
	}
}

void
Octree::GetObjectsList(PhysicalObject* object, PhysicalObjectsVector* physicalObjects)
{
	physicalObjects->insert(physicalObjects->end(), _objects.begin(), _objects.end());

	if (IsSmallestLeaf())
		return;
	
	if (HasChild())
		for (uint8_t used = _physicChildren, i = 0; used > 0; used >>= 1, ++i)
			if (used & 1 && _children[i]->_area.IsColliding(object->GetCollider()))
				_children[i]->GetObjectsList(object, physicalObjects);
}

void 
Octree::GetCollidingChunksList(const BoundingBox& box, Chunks* collidedChunks)
{
	/* Add chunk if at bottom */
	if (IsSmallestLeaf()) {
		if (_chunk != nullptr)
			collidedChunks->push_back(_chunk);
		return;
	}

	/* We will be treating AABB as a points to find all chunks. Because of the size, there will be max 8 neighbours. */
	const Vector3 &min = box.GetMinimas();
	const Vector3 &max = box.GetMaximas();
	Vector3 pts[8] = {
		Vector3(min.x, min.y, min.z),
		Vector3(min.x, min.y, max.z),
		Vector3(min.x, max.y, min.z),
		Vector3(min.x, max.y, max.z),
		Vector3(max.x, min.y, min.z),
		Vector3(max.x, min.y, max.z),
		Vector3(max.x, max.y, min.z),
		Vector3(max.x, max.y, max.z)
	};

	/* Check each point */
	for (int n = 0; n < 8; ++n) {
		Octree* node = this;

		/* Find the smallest childr which contains that point */
		while (!node->IsSmallestLeaf()) {
			bool fitted = false;
			if (node->HasChild()) {
				for (uint8_t used = node->_chunkChildren, i = 0; used > 0; used >>= 1, ++i) {
					if (used & 1) {
						if (node->_children[i]->_area.IsContaining(pts[n])) {
							fitted = true;
							node = node->_children[i];
							break;
						}
					}
				}
			}
			if (!fitted)
				break;
		}

		/* If we are at the bottom, we can add chunk */
		if (node->IsSmallestLeaf() && node->_chunk != nullptr)
			if (std::find(collidedChunks->begin(), collidedChunks->end(), node->_chunk) == collidedChunks->end())
				collidedChunks->push_back(node->_chunk);
	}
}

void
Octree::CheckTerrainIntersections(PhysicalObject* object, CollisionInfo* info)
{
	assert(object->GetCollider().GetDimension().x <= Chunk::dimension &&
		   object->GetCollider().GetDimension().y <= Chunk::dimension &&
		   object->GetCollider().GetDimension().z <= Chunk::dimension,
		   "Object's size cannot be higher than chunk size!");

	BoundingBox childAreas[8];
	SubdivideNode(childAreas);

	Chunks usedChunks;
	const BoundingBox& obj = object->GetCollider();
	GetCollidingChunksList(obj, &usedChunks);

	if (usedChunks.empty())
		return;

	/* Initialize chunks as nulls */
	Chunk *chunks[8] = {};

	/* These are constant offsets when exceeding chunk dimension in each direction (x, y, z) */
	/* Details in engineering thesis */
	const int dirOffsets[3] = { 1, 2, 4 };

	/* We want to sort chunks into the structure with order presented in engineering thesis */
	for (Chunks::iterator it = usedChunks.begin(); it != usedChunks.end(); ++it) {
		Chunk* chunk = *it;
		/* Get center of the chunk */
		const Vector3& chunkOffset = chunk->GetCenter() + chunk->GetOffset();
		/* Get center of the object */
		const Vector3 objCenter = obj.GetPosition();

		int index = 0;
		/* It means that object is on left side */
		if (objCenter.x <= chunkOffset.x)
			index += dirOffsets[0];
		/* It means that object is below */
		if (objCenter.y <= chunkOffset.y)
			index += dirOffsets[1];
		/* It means that object is behind */
		if (objCenter.z <= chunkOffset.z)
			index += dirOffsets[2];

		chunks[index] = chunk;
	}

	/* Constant arrays to check initial offset when whole wall is null. This is the order of the checking */
	const int minX[8] = { 0, 2, 4, 6, 1, 3, 5, 7 };
	const int minY[8] = { 0, 1, 4, 5, 2, 3, 6, 7 };
	const int minZ[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

	/* Array to do not check same wall two times */
	bool found[3] = {};
	/* 
	* Indicates that we are using "positive" side of the wall. It means, that if vener of the obejct is in (0, 0, 0),
	* all chunks in the specific direction are above given axis (only one at once, X or Y or Z) 
	*/
	bool posSide[3] = {};
	Vector3 minOffsets;
	for (int i = 0; i < 8; ++i) {
		if (!found[0] && chunks[minX[i]] != nullptr) {
			minOffsets.x = chunks[minX[i]]->GetOffset().x;
			found[0] = true;
			posSide[0] = i > 3;
		}

		if (!found[1] && chunks[minY[i]] != nullptr) {
			minOffsets.y = chunks[minY[i]]->GetOffset().y;
			found[1] = true;
			posSide[1] = i > 3;
		}

		if (!found[2] && chunks[minZ[i]] != nullptr) {
			minOffsets.z = chunks[minZ[i]]->GetOffset().z;
			found[2] = true;
			posSide[2] = i > 3;
		}

		if (found[0] && found[1] && found[2])
			break;
	}


	/* Get total minimas and maximas */
	int yMin = (int)floor(obj.GetMinimas().y - minOffsets.y);
	int yMax = (int)floor(obj.GetMaximas().y - minOffsets.y);
	int xMin = (int)floor(obj.GetMinimas().x - minOffsets.x);
	int xMax = (int)floor(obj.GetMaximas().x - minOffsets.x);
	int zMin = (int)floor(obj.GetMinimas().z - minOffsets.z);
	int zMax = (int)floor(obj.GetMaximas().z - minOffsets.z);

	/* Clamp it, because minOffsets could be greater than maximas or minimas if all negative walls are null in given direction */
	if (xMin < 0)
		xMin = 0;
	if (yMin < 0)
		yMin = 0;
	if (zMin < 0)
		zMin = 0;

	/* Map it to array for simplier usage */
	int minCoords[3] = { xMin, yMin, zMin };
	int maxCoords[3] = { xMax, yMax, zMax };

	

	/* Indicates, that given direction was already truncated, to do not increase index multiple times inside for loop */
	bool truncated[3] = {};

	int index = 0;
	if (posSide[0]) {
		index += 1;
		truncated[0] = true;
	}
	if (posSide[1]) {
		index += 2;
		truncated[1] = true;
	}
	if (posSide[2]) {
		index += 4;
		truncated[2] = true;
	}

	/* Containing global direction info */
	bool collisions[6] = {};
	/*
	 * Contains info that we have discovered voxel at least one, in case the object passed 
	 * through first layer of the voxels and collision cannot be detected 
	 */
	bool anyVoxel = false;	
	int trunCoords[3];
	/* For each axis going into positive direction */
	for (int i = 0; i < 3; ++i) {
		/* Map index */
		int ind[] = { i, (i + 1) % 3, (i + 2) % 3 };
		/* Write is as U, V and D - dir */
		const int colU = ind[0];
		const int colV = ind[1];
		const int colD = ind[2];

		/* Check collisions only if AABB is containig at least 2 voxels in that direction */
		if (maxCoords[colD] - minCoords[colD] > 0) {
			int uIndex = index;
			bool trunU = truncated[colU];
			for (int u = minCoords[colU]; u <= maxCoords[colU]; ++u) {
				/* We must check if we exceeded chunk's cooordinates going in that direction */
				if (!trunU && u >= Chunk::dimension) {
					trunU = true;
					/* If so, we must move to the adjacent chunk in that direction */
					uIndex += dirOffsets[colU];
				}
				trunCoords[colU] = u % Chunk::dimension;

				int vIndex = uIndex;
				bool trunV = truncated[colV];
				for (int v = minCoords[colV]; v <= maxCoords[colV]; ++v) {
					if (!trunV && v >= Chunk::dimension) {
						trunV = true;
						vIndex += dirOffsets[colV];
					}
					trunCoords[colV] = v % Chunk::dimension;

					/* If chunk is not existing, we want to advance to next chunk (if it is not existing 'for' condition will fail) */
					if (chunks[vIndex] == nullptr) {
						if (v >= Chunk::dimension)
							break;
						else
							/* Advance to the next chunk */
							v = Chunk::dimension - 1;
						continue;
					}
					trunCoords[colD] = minCoords[colD];

					/* Get voxel and check bottom layer first */
					const Voxel& firstVox = chunks[vIndex]->GetLocal(trunCoords[0], trunCoords[1], trunCoords[2]);
					/* If there is something, we must check it to the top */
					if (!firstVox.IsEmpty()) {
						bool empty = true;
						int dIndex = vIndex;

						bool trunD = truncated[colD];

						/* Skip first element */
						for (int d = minCoords[colD] + 1; d <= maxCoords[colD]; ++d) {
							if (!trunD && d >= Chunk::dimension) {
								trunD = true;
								dIndex += dirOffsets[colD];
							}
							trunCoords[colD] = d % Chunk::dimension;

							if (chunks[dIndex] == nullptr) {
								if (d >= Chunk::dimension)
									break;
								else
									d = Chunk::dimension - 1;
								continue;
							}

							const Voxel& nextVox = chunks[dIndex]->GetLocal(trunCoords[0], trunCoords[1], trunCoords[2]);
							/* If there was voxel above, we cannot continue, it we must change another voxel at the bottom layer */
							if (!nextVox.IsEmpty()) {
								empty = false;
								anyVoxel = true;
								break;
							}
						}
						/* If there was no voxel above bottom one, there could be a collision from that side */
						if (empty) {
							collisions[i] = true;
						}
					}
					if (collisions[i])
						break;
				}
				if (collisions[i])
					break;
			}
		}
	}

	/* Same af for above, however there we will be checking from 'top' to the 'bottom' */
	for (int i = 0; i < 3; ++i) {
		int ind[] = { i, (i + 1) % 3, (i + 2) % 3 };
		const int colU = ind[0];
		const int colV = ind[1];
		const int colD = ind[2];

		/* We must calculate offset - it defines location of the top layer */
		int offset = maxCoords[colD] >= Chunk::dimension ? dirOffsets[colD] : 0;

		if (maxCoords[colD] - minCoords[colD] > 0 && chunks[index + offset] != nullptr) {
			int uIndex = index;

			bool trunU = truncated[colU];
			for (int u = minCoords[colU]; u <= maxCoords[colU]; ++u) {
				if (!trunU && u >= Chunk::dimension) {
					trunU = true;
					uIndex += dirOffsets[colU];
				}
				trunCoords[colU] = u % Chunk::dimension;

				int vIndex = uIndex;
				bool trunV = truncated[colV];
				for (int v = minCoords[colV]; v <= maxCoords[colV]; ++v) {
					if (!trunV && v >= Chunk::dimension) {
						trunV = true;
						vIndex += dirOffsets[colV];
					}
					trunCoords[colV] = v % Chunk::dimension;

					if (chunks[vIndex + offset] == nullptr) {
						if (v >= Chunk::dimension)
							break;
						else
							v = Chunk::dimension - 1;
						continue;
					}

					trunCoords[colD] = maxCoords[colD] % 16;
					/* We are adding offset there */
					const Voxel& firstVox = chunks[vIndex + offset]->GetLocal(trunCoords[0], trunCoords[1], trunCoords[2]);
					if (!firstVox.IsEmpty()) {
						bool empty = true;
						int dIndex = vIndex;

						bool trunD = truncated[colD];
						/* And going from bottom to top without last element */
						for (int d = minCoords[colD]; d <= maxCoords[colD] - 1; ++d) {
							if (!trunD && d >= Chunk::dimension) {
								trunD = true;
								dIndex += dirOffsets[colD];
							}
							trunCoords[colD] = d % Chunk::dimension;

							if (chunks[dIndex] == nullptr) {
								if (d >= Chunk::dimension)
									break;
								else
									d = Chunk::dimension - 1;
								continue;
							}

							const Voxel& nextVox = chunks[dIndex]->GetLocal(trunCoords[0], trunCoords[1], trunCoords[2]);
							if (!nextVox.IsEmpty()) {
								empty = false;
								anyVoxel = true;
								break;
							}
						}
						if (empty) {
							collisions[3 + i] = true;
						}
					}
					if (collisions[3 + i])
						break;
				}
				if (collisions[3 + i])
					break;
			}
		}
	}

	Directions dirs;
	/* Map array onto the directions */
	if (collisions[0])
		dirs.north = 1;
	if (collisions[1])
		dirs.east = 1;
	if (collisions[2])
		dirs.bottom = 1;

	if (collisions[3])
		dirs.south = 1;
	if (collisions[4])
		dirs.west = 1;
	if (collisions[5])
		dirs.top = 1;

	/* If there are no collisions detected and voxel was found, we must fill structure  */
	if (dirs.directions == 0 && anyVoxel)
		dirs.directions = 0xff;

	info->SetHitDirection(dirs);
}


void 
Octree::Insert(const Voxel& voxel, Vector3 coordinates)
{
	/*  Create bounding box from voxel coordinates - it will be bounding box of the voxel */
	BoundingBox box(coordinates + Vector3(0.5f, 0.5f, 0.5f), Vector3(1.0f, 1.0f, 1.0f));

	/* Check for collision with all physical objects */
	for (PhysicalObjects::iterator it = _objects.begin(); it != _objects.end(); ++it)
		if ((*it)->GetCollider().IsColliding(box))
			return;

	/* Change chunk */
	if (IsSmallestLeaf()) {
		/* if it is not existing, create one */
		if (_chunk == nullptr)
			_chunk = new Chunk(_area.GetMinimas());

		if (_chunk->Get(coordinates).IsEmpty())
			_chunk->Set(coordinates, voxel.GetType());
		return;
	}

	BoundingBox childAreas[8];
	SubdivideNode(childAreas);

	/* Check if chunk is within game range */
	if (_area.IsContaining(coordinates)) {
		bool fits = false;

		/* Check inside each child node */
		for (int i = 0; i < 8; ++i) {
			if (childAreas[i].IsContaining(coordinates)) {
				if (_children[i] != nullptr) {
					_children[i]->Insert(voxel, coordinates);
				}
				else {
					/* Calculate offset of the new chunk basing on the coordinates of the voxel */
					Vector3 offset;
					for (int i = 0; i < 3; ++i) {
						if (coordinates[i] >= 0.0f) {
							offset[i] = (float)((int)coordinates[i] / Chunk::dimension) * Chunk::dimension;
						}
						/* For negative we must use different formula */
						else {
							offset[i] = (float)((int)coordinates[i] / Chunk::dimension - 1) * Chunk::dimension;
							if ((int)coordinates[i] % 16 == 0)
								offset[i] += 1;
						}
					}
					Chunk* chunk = new Chunk(offset);
					chunk->Set(coordinates, voxel.GetType());
					Insert(chunk);
				}
				/* Add chunk info */
				_chunkChildren |= (uint8_t)(1 << i);

				fits = true;
				break;
			}
		}
#ifdef VE_DEBUG
		/* If it did not fit in any child, there had to be an error */
		if (!fits)
			assert(false, "Vox out of bound of the octree");
#endif
	}
	else if (IsRoot()) {
		assert(false, "Vox out of playable area");
	}
}

}