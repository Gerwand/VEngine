#include "Octree.h"

namespace vengine {

bool Octree::_built = false;

Octree::Octree() :
	_timeToLive(-1), _availableLifetime(_initialMaxLifetime),
	_parent(NULL), _children{},
	_chunkMesh(nullptr), _chunk(nullptr), _chunkChildren(0),
	_physicChildren(0),
	_area(Vector3::zeroes, Vector3::zeroes)
{
}

Octree::Octree(const BoundingBox& area, const PhysicalObjects& objects) :
	_timeToLive(-1), _availableLifetime(_initialMaxLifetime),
	_parent(NULL), _children{},
	_chunkMesh(nullptr), _chunk(nullptr), _chunkChildren(0),
	_physicChildren(0),
	_area(area), _objects(objects)
{
}

Octree::Octree(const BoundingBox& area) :
	_timeToLive(-1), _availableLifetime(_initialMaxLifetime),
	_parent(NULL), _children{},
	_chunkMesh(nullptr), _chunk(nullptr), _chunkChildren(0),
	_physicChildren(0),
	_area(area)
{
}

Octree::Octree(const BoundingBox& area, const Chunks& chunks) :
	_timeToLive(-1), _availableLifetime(_initialMaxLifetime),
	_parent(NULL), _children{},
	_chunkMesh(nullptr), _chunk(nullptr), _chunkChildren(0),
	_physicChildren(0),
	_area(area), _chunks(chunks)
{
}

Octree::~Octree()
{
	if (!_chunks.empty())
		delete _chunks.front();
	delete _chunkMesh;
}

void
Octree::UpdateTree()
{
	if (!_built) {
		while (!_pendingObjects.empty()) {
			_objects.push_back(_pendingObjects.front());
			_pendingObjects.pop();
		}
		while (!_pendingChunks.empty()) {
			_chunks.push_back(_pendingChunks.front());
			_pendingChunks.pop();
		}

		BuildTree();
		_built = true;
	}
	else {
		while (!_pendingObjects.empty()) {
			Insert(_pendingObjects.front());
			_pendingObjects.pop();
		}
		while (!_pendingChunks.empty()) {
			Insert(_pendingChunks.front());
			_pendingChunks.pop();
		}
	}
}



void
Octree::BuildChunk(BoundingBox childAreas[8])
{
	Chunks childChunks[8];

	for (Chunks::iterator it = _chunks.begin(); it != _chunks.end();) {
		Chunk* chunk = *it;
		if (chunk->IsEmpty()) {
			++it;
			continue;
		}

		Vector3 center = chunk->GetCenter() + chunk->GetOffset();
		bool fits = false;
		for (int i = 0; i < 8; i++) {
			if (childAreas[i].IsContaining(center)) {
				childChunks[i].push_back(chunk);
				it = _chunks.erase(it);
				fits = true;
				break;
			}
		}
		if (!fits)
			++it;
	}

	for (int i = 0; i < 8; ++i) {
		if (!childChunks[i].empty()) {
			_children[i] = CreateChildNode(childAreas[i], childChunks[i]);
			_chunkChildren |= (uint8_t)(1 << i);
			_children[i]->BuildTree();
		}
	}
}

void
Octree::BuildObject(BoundingBox childAreas[8])
{
	if (_objects.size() <= 1)
		return;

	PhysicalObjects childObjects[8];

	//Check if object can fit in only one subdivided region
	for (PhysicalObjects::iterator it = _objects.begin(); it != _objects.end();) {
		PhysicalObject *phys = *it;
		bool fitted = false;
		for (int i = 0; i < 8; i++) {
			assert(phys->GetCollider().GetDimension() != Vector3::zeroes, "Warning! Physical object have wrong collider");

			if (childAreas[i].IsContaining(phys->GetCollider())) {
				childObjects[i].push_back(phys);
				it = _objects.erase(it);
				fitted = true;
				break;
			}
		}
		if (!fitted)
			++it;
	}

	//Check if any object can be passed down
	for (int i = 0; i < 8; ++i) {
		if (!childObjects[i].empty()) {
			if (_children[i] == nullptr) {
				//If there was no child, create one
				_children[i] = CreateChildNode(childAreas[i], childObjects[i]);
			}
			else {
				//If there was children, only pass objects without creation
				_children[i]->_objects = childObjects[i];
			}
			//Set proper bit to indicate that children have objects inside
			_physicChildren |= (uint8_t)(1 << i);
		}
	}
}

void
Octree::BuildTree()
{
	if (IsSmallestLeaf()) {
		assert(_chunks.size() <= 1, "Too many chunks in the leaf node: %d", _chunks.size());
		if (!_chunks.empty()) {
			_chunk = _chunks.back();
			_chunks.clear();
			assert(_chunkMesh == nullptr, "Mesh should be nullptr when creating tree, there is: %s", _chunkMesh->GetName().c_str());
			_chunkMesh = new VoxelMesh;
			_chunkMesh->Init(_chunk->GetName());
			_chunk->GenerateMesh(_chunkMesh);
		}
		return;
	}

	BoundingBox childAreas[8];
	SubdivideNode(childAreas);

	BuildChunk(childAreas);
	BuildObject(childAreas);

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
	if (child->IsSmallestLeaf()) {
		child->_chunk = chunk;
		child->_chunkMesh = chunkMesh;
	}
	child->_parent = this;

	return child;
}

void
Octree::UpdateChunk()
{
	if (_chunk == nullptr)
		return;

	if (!IsSmallestLeaf()) {
		Insert(_chunk, _chunkMesh);
		_chunk = nullptr;
		_chunkMesh = nullptr;
		return;
	}

	//Check if is empty
	if (_chunk->IsEmpty()) {
		delete _chunk;
		delete _chunkMesh;

		_chunk = nullptr;
		_chunkMesh = nullptr;
		return;
	}

	if (_chunk->HasChanged() && !_chunk->IsEmpty()) {
		if (_chunkMesh == nullptr) {
			_chunkMesh = new VoxelMesh;
			_chunkMesh->Init(_chunk->GetName());
		}
		_chunk->GenerateMesh(_chunkMesh);
	}

}


void
Octree::UpdateObject()
{

}


void
Octree::RemoveUnusedChildren()
{
	uint8_t usedChildren = _physicChildren | _chunkChildren;

	for (uint8_t used = usedChildren, i = 0; used > 0; used >>= 1, ++i) {
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

	if (_objects.empty() && _chunk == nullptr) {
		if (!HasChild()) {
			if (_timeToLive == -1)
				_timeToLive = _availableLifetime;
			else if (_timeToLive > 0)
				--_timeToLive;
		}
	}
	else {
		if (_timeToLive != -1) {
			if (_availableLifetime <= _maximumLifetime)
				_availableLifetime *= 2;
			_timeToLive = -1;
		}
	}

	UpdateChunk();

	/*
	 * Get list of all changed objects, do it before updating
	 * all objects to do not check same objects two times
	 */
	PhysicalObjects changed;

	/*
	 * If both children are the same, we can only check moved children, because last time it was checked, they were good.
	 * However, if there were new nodes created by chunks, we must check all objects to ensure that they will go into new nodes.
	 */
	bool childChanged = _physicChildren != _chunkChildren;
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


		//TODO - maybe check if node != this???
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
	for (PhysicalObjects::iterator it = _objects.begin(); it != _objects.end(); ++it) {
		PhysicalObject* srcObj = *it;
		PhysicalObjectsVector objects;
		GetObjectsList(srcObj->GetCollider(), &objects);

		for (PhysicalObjectsVector::iterator itObj = objects.begin(); itObj != objects.end(); ++itObj) {
			PhysicalObject* colObj = *itObj;
			if (colObj == srcObj)
				continue;
			if (srcObj->GetCollider().IsColliding(colObj->GetCollider())) {
				CollisionInfo info(this);
				info.SetCollisionObject(srcObj, colObj);
				infos->push_back(info);
				info.SetCollisionObject(colObj, srcObj);
				infos->push_back(info);
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

	/* If we cannot go any further put it into node. */
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
		/* Inform about item being outside game range, and as for now do nothing */
		assert(false, "Item %s out of playable area: %s", object->GetName().c_str(), object->GetTransform().GetPosition().ToString().c_str());
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

		/* If it did not fit in any child, there had to be an error */
		if (!fits)
			assert(false, "Chunk out of bound of the octree");
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

	//Upper, front and right first

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

	if (!camera->IsVisibleBig(_area)) {
		//std::cout << std::endl << "Node not visible: " << _area.GetPosition() << _area.GetDimension() << std::endl;
		return;
	}

	if (_chunk != nullptr) {
		/* There should not be any empty chunk, bo just to make sure, just draw it */
		if (!_chunk->IsEmpty()) {
			assert(_chunkMesh != nullptr, "%s mesh not existing", _chunk->GetName().c_str());
			renderer->SetModelMatrix(_chunk->GetModelMatrix());
			_chunkMesh->Draw(renderer);
		}
	}


	/* Render only chunks */
	for (uint8_t used = _chunkChildren, i = 0; used > 0; used >>= 1, ++i)
		if (used & 1)
			_children[i]->Draw(renderer);

}

static inline std::string toBinary(uint8_t num)
{
	uint8_t i;
	std::string out;
	for (i = 0x80; i != 0; i >>= 1)
		out += (num&i) ? '1' : '0';

	return out;
}

std::string
Octree::ToString(int lvl)
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
	for (PhysicalObjects::iterator it = _objects.begin(); it != _objects.end(); ++it)
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

	boxes.SetColor(Vector3(240 / 255.0f, 230 / 255.0f, 140 / 255.0f));

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
	//There should go collision check with all objects

	/* Check collision with chunk if it is at bottom */
	if (IsSmallestLeaf()) {
		if (_chunk == nullptr) {
			while (!ray->HasEnded() && _area.IsContaining(ray->Shoot()));
		}
		else {
			intersectionInfo->TestIntersection(ray, _chunk);
		}

		if (ray->HasEnded() || intersectionInfo->CollisionFound())
			return;
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

	/*
	* Because of octree minimum size is equal to chunk dimension,
	* we can only check one point to determine location.
	*/


	bool fitted = false;

	while (!fitted && !ray->HasEnded()) {
		Vector3 rayPos = ray->GetCurrentPosition();
		if (_area.IsContaining(rayPos)) {
			if (HasChild())
				for (uint8_t used = _physicChildren | _chunkChildren, i = 0; used > 0; used >>= 1, ++i)
					if (used & 1)
						if (childAreas[i].IsContaining(rayPos)) {
							_children[i]->CheckRayCollision(ray, intersectionInfo);
							return;
						}

			ray->Advance(0.5f);
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
			if (node->IsRoot() && !node->_area.IsContaining(ray->GetCurrentPosition()))
				return;

			node->CheckRayCollision(ray, intersectionInfo);
		}
	}
}

void
Octree::GetObjectsList(const BoundingBox& box, PhysicalObjectsVector* physicalObjects)
{
	physicalObjects->insert(physicalObjects->end(), _objects.begin(), _objects.end());

	if (IsSmallestLeaf())
		return;
	
	if (HasChild())
		for (uint8_t used = _physicChildren, i = 0; used > 0; used >>= 1, ++i)
			if (used & 1 && _children[i]->_area.IsColliding(box))
				_children[i]->GetObjectsList(box, physicalObjects);
}

void 
Octree::GetCollidingChunksList(const BoundingBox& box, Chunks* collidedChunks)
{
	/* At chunk if at bottom */
	if (IsSmallestLeaf()) {
		if (_chunk != nullptr)
			collidedChunks->push_back(_chunk);
		return;
	}

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

	for (int n = 0; n < 8; ++n) {
		Octree* node = this;

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

		if (node->IsSmallestLeaf() && node->_chunk != nullptr)
			if (std::find(collidedChunks->begin(), collidedChunks->end(), node->_chunk) == collidedChunks->end())
				collidedChunks->push_back(node->_chunk);
	}
}

void
Octree::CheckTerrainIntersections(PhysicalObject* object, CollisionInfo* info)
{
	/* Check collision with chunk if it is at bottom */

	assert(object->GetCollider().GetDimension().x <= Chunk::dimension &&
		   object->GetCollider().GetDimension().y <= Chunk::dimension &&
		   object->GetCollider().GetDimension().z <= Chunk::dimension,
		   "Object's size cannot be higher than chunk size!");

	BoundingBox childAreas[8];
	SubdivideNode(childAreas);

	/*
	* Because of octree minimum size is equal to chunk dimension,
	* we can only check one point to determine location.
	*/

	if (!IsSmallestLeaf()) {
		if (_area.IsContaining(object->GetCollider())) {
			for (uint8_t used = _physicChildren | _chunkChildren, i = 0; used > 0; used >>= 1, ++i)
				if (used & 1)
					if (childAreas[i].IsContaining(object->GetCollider())) {
						_children[i]->CheckTerrainIntersections(object, info);
						return;
					}
		}
	}

	Chunks usedChunks;
	const BoundingBox& obj = object->GetCollider();
	GetCollidingChunksList(obj, &usedChunks);

	if (usedChunks.empty())
		return;

	Chunk *chunks[8] = {};



	const static int dirOffsets[3] = { 1, 2, 4 };
	for (Chunks::iterator it = usedChunks.begin(); it != usedChunks.end(); ++it) {
		Chunk* chunk = *it;
		const Vector3& chunkOffset = chunk->GetCenter() + chunk->GetOffset();
		const Vector3 objCenter = obj.GetPosition();

		int index = 0;
		//It means that chunk is on left side
		if (objCenter.x <= chunkOffset.x)
			index += dirOffsets[0];
		if (objCenter.y <= chunkOffset.y)
			index += dirOffsets[1];
		if (objCenter.z <= chunkOffset.z)
			index += dirOffsets[2];

		chunks[index] = chunk;
	}

	const static int minX[8] = { 0, 2, 4, 6, 1, 3, 5, 7 };
	const static int minY[8] = { 0, 1, 4, 5, 2, 3, 6, 7 };
	const static int minZ[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	bool found[3] = {};
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


	//Calculate collider 


	int yMin = (int)floor(obj.GetMinimas().y - minOffsets.y);
	int yMax = (int)floor(obj.GetMaximas().y - minOffsets.y);
	int xMin = (int)floor(obj.GetMinimas().x - minOffsets.x);
	int xMax = (int)floor(obj.GetMaximas().x - minOffsets.x);
	int zMin = (int)floor(obj.GetMinimas().z - minOffsets.z);
	int zMax = (int)floor(obj.GetMaximas().z - minOffsets.z);

	if (xMin < 0)
		xMin = 0;
	if (yMin < 0)
		yMin = 0;
	if (zMin < 0)
		zMin = 0;

	int minCoords[3] = { xMin, yMin, zMin };
	int maxCoords[3] = { xMax, yMax, zMax };

	/* Check from bottom only if it's higher than 2 blocks */

	int trunCoords[3];
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


	bool collisions[6] = {};
	bool anyVoxel = false;

	
	for (int i = 0; i < 3; ++i) {
		int ind[] = { i, (i + 1) % 3, (i + 2) % 3 };
		const int colU = ind[0];
		const int colV = ind[1];
		const int colD = ind[2];

		if (maxCoords[colD] - minCoords[colD] > 0) {
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

					//If chunk is not existing, we want to advance to next chunk (if it is not existing for condition will fail)
					if (chunks[vIndex] == nullptr) {
						if (v >= Chunk::dimension)
							break;
						else
							v = Chunk::dimension - 1;
						continue;
					}
					trunCoords[colD] = minCoords[colD];
					const Voxel& firstVox = chunks[vIndex]->GetLocal(trunCoords[0], trunCoords[1], trunCoords[2]);
					if (!firstVox.IsEmpty()) {
						bool empty = true;
						int dIndex = vIndex;

						bool trunD = truncated[colD];
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
							if (!nextVox.IsEmpty()) {
								empty = false;
								anyVoxel = true;
								break;
							}
						}
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

	for (int i = 0; i < 3; ++i) {
		int ind[] = { i, (i + 1) % 3, (i + 2) % 3 };
		const int colU = ind[0];
		const int colV = ind[1];
		const int colD = ind[2];


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
					const Voxel& firstVox = chunks[vIndex + offset]->GetLocal(trunCoords[0], trunCoords[1], trunCoords[2]);
					if (!firstVox.IsEmpty()) {
						bool empty = true;
						int dIndex = vIndex;

						bool trunD = truncated[colD];
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
	/*
	* We want to insert chunk as deep as we can, they will not change as dynamically
	* as physical objects, and it will be easier to check collisions that way.
	*/
	if (IsSmallestLeaf()) {
		if (_chunk == nullptr)
			_chunk = new Chunk(_area.GetMinimas());

		if (_chunk->Get(coordinates).IsEmpty())
			_chunk->Set(coordinates, voxel.GetType());
		else
			std::cout << "\nBusy\n";
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
					Vector3 offset;
					for (int i = 0; i < 3; ++i) {
						if (coordinates[i] >= 0.0f)
							offset[i] = ((int)coordinates[i] / 16) * 16.0f;
						else {
							offset[i] = ((int)coordinates[i] / 16 - 1) * 16.0f ;
							if ((int)coordinates[i] % 16 == 0)
								offset[i] += 1;
						}
					}
					Chunk* chunk = new Chunk(offset);
					chunk->Set(coordinates, voxel.GetType());
					Insert(chunk);
				}
				_chunkChildren |= (uint8_t)(1 << i);

				fits = true;
				break;
			}
		}

		/* If it did not fit in any child, there had to be an error */
		if (!fits)
			assert(false, "Vox out of bound of the octree");
	}
	else if (IsRoot()) {
		/* Inform about chunk being outside game range, and as for now do nothing */
		assert(false, "Vox out of playable area");
	}
}

}