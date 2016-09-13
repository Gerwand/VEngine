#include "OctreeChunk.h"

namespace vengine {

bool OctreeChunk::_ready = false;
bool OctreeChunk::_built = false;

OctreeChunk::OctreeChunk() :
	_timeToLive(-1),
	_availableLifetime(_initialMaxLifetime),
	_parent(nullptr),
	_children{},
	_usedChildren(0),
	_area(Vector3::zeroes, Vector3::zeroes)
{
}



OctreeChunk::OctreeChunk(const BoundingBox& area, const Chunks& chunks) :
	_timeToLive(-1),
	_availableLifetime(_initialMaxLifetime),
	_parent(NULL),
	_chunkMesh(nullptr),
	_children{},
	_usedChildren(0),
	_area(area),
	_chunks(chunks)
{
}

OctreeChunk::OctreeChunk(const BoundingBox& area) :
	_timeToLive(-1),
	_availableLifetime(_initialMaxLifetime),
	_parent(NULL),
	_chunkMesh(nullptr),
	_children{},
	_usedChildren(0),
	_area(area)
{
}



void
OctreeChunk::UpdateTree()
{
	if (!_built) {
		while (!_pendingChunks.empty()) {
			_chunks.push_back(_pendingChunks.front());
			_pendingChunks.pop();
		}

		BuildTree();
		_built = true;
	}
	else {
		while (!_pendingChunks.empty()) {
			Insert(_pendingChunks.front());
			_pendingChunks.pop();
		}
	}

	_ready = true;
}


void
OctreeChunk::BuildTree()
{
	//terminate the recursion if we're a leaf node
	if (_chunks.size() <= 1)
		return;



	if (IsSmallestLeaf()) {
		return;
	}

	BoundingBox childAreas[8];
	SubdivideNode(childAreas);

	
}

OctreeChunk*
OctreeChunk::CreateChildNode(const BoundingBox& area, const Chunks& objects)
{
	if (objects.empty())
		return nullptr;

	OctreeChunk *child = new OctreeChunk(area, objects);
	child->_parent = this;

	return child;
}

OctreeChunk*
OctreeChunk::CreateChildNode(const BoundingBox& area, Chunk *object, VoxelMesh* chunkMesh)
{
	OctreeChunk *child = new OctreeChunk(area);
	child->_chunkMesh = chunkMesh;
	child->_parent = this;

	return child;
}


void
OctreeChunk::Update()
{
	assert(_built, "Cannot update not built tree.");
	if (_chunks.empty()) {
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


}


void
OctreeChunk::Insert(Chunk* chunk, VoxelMesh* chunkMesh)
{
	if (_chunks.size() == 0 && _usedChildren == 0) {
		_chunks.push_back(chunk);
		_chunkMesh = chunkMesh;
		return;
	}

	if (IsSmallestLeaf()) {
		_chunks.push_back(chunk);
		assert(_chunks.size() == 1, "Two chunks in one octree node!");
		return;
	}

	BoundingBox childAreas[8];
	SubdivideNode(childAreas);

	Vector3 chunkCenter = chunk->GetCenter() + chunk->GetOffset();

	if (_area.IsContaining(chunkCenter)) {
		bool fits = false;
		for (int i = 0; i < 8; ++i) {
			if (childAreas[i].IsContaining(chunkCenter)) {
				if (_children[i] != nullptr) {
					_children[i]->Insert(chunk, chunkMesh);
				}
				else {
					_children[i] = CreateChildNode(childAreas[i], chunk, chunkMesh);   //create a new tree node with the item
					_usedChildren |= (uint8_t)(1 << i);
				}
				fits = true;
			}
		}
		if (!fits)
			assert(false, "Chunk out of bound of the octree");
	}
	else if (IsRoot()) {
		assert(false, "Item %s out of playable area", chunk->GetName().c_str());
	}

	Chunk* nodeChunk = _chunks.front();
	chunkCenter = nodeChunk->GetCenter() + nodeChunk->GetOffset();
	if (_area.IsContaining(chunkCenter)) {
		bool fits = false;
		for (int i = 0; i < 8; ++i) {
			if (childAreas[i].IsContaining(chunkCenter)) {
				if (_children[i] != nullptr) {
					_children[i]->Insert(nodeChunk, _chunkMesh);
				}
				else {
					_children[i] = CreateChildNode(childAreas[i], nodeChunk, _chunkMesh);   //create a new tree node with the item
					_usedChildren |= (uint8_t)(1 << i);
				}
				fits = true;
			}
		}
		_chunkMesh = nullptr;
		_chunks.clear();

		if (!fits)
			assert(false, "Chunk out of bound of the octree");
	}
	else if (IsRoot()) {
		assert(false, "Item %s out of playable area", chunk->GetName().c_str());
	}
}

void
OctreeChunk::SubdivideNode(BoundingBox *regions)
{
	const Vector3& center = _area.GetPosition();
	Vector3 half = _area.GetDimension() / 2.0f;
	Vector3 quart = half / 2.0f;

	//Front
	regions[0].Set(Vector3(center.x - quart.x, center.y - quart.y, center.z + quart.z), half);
	regions[1].Set(Vector3(center.x + quart.x, center.y - quart.y, center.z + quart.z), half);
	regions[2].Set(Vector3(center.x + quart.x, center.y + quart.y, center.z + quart.z), half);
	regions[3].Set(Vector3(center.x - quart.x, center.y + quart.y, center.z + quart.z), half);

	//Back
	regions[4].Set(Vector3(center.x - quart.x, center.y - quart.y, center.z - quart.z), half);
	regions[5].Set(Vector3(center.x + quart.x, center.y - quart.y, center.z - quart.z), half);
	regions[6].Set(Vector3(center.x + quart.x, center.y + quart.y, center.z - quart.z), half);
	regions[7].Set(Vector3(center.x - quart.x, center.y + quart.y, center.z - quart.z), half);

}

void
OctreeChunk::Draw(Renderer* renderer)
{
	//renderer->SetModelMatrix(Matrix4::identity);
	//_area.Draw(renderer, Vector3(135 / 255.0f, 206 / 255.0f, 250 / 255.0f));

	/*
	Lines lines;
	lines.Init();
	lines.SetColor(Vector3(1.0f, 0.0f, 0.0f));
	for (PhysicalObjects::iterator it = _objects.begin(); it != _objects.end(); ++it) {
		lines.AddLine(_area.GetPosition(), (*it)->GetTransform().GetPosition());
	}
	lines.Draw(renderer);
	*/

	if (!_chunks.empty()) {
		Chunk* chunk = _chunks.front();
		if (!chunk->IsEmpty()) {
			assert(_chunkMesh != nullptr, "%s mesh not existing", chunk->GetName().c_str());
			renderer->SetModelMatrix(chunk->GetModelMatrix());
			_chunkMesh->Draw(renderer);
		}
			
	}

	for (uint8_t used = _usedChildren, i = 0; used > 0; used >>= 1, ++i)
		if (used & 1)
			_children[i]->Draw(renderer);
}

std::string toBinary(uint8_t num)
{
	uint8_t i;
	std::string out;
	for (i = 0x80; i != 0; i >>= 1)
		out += (num&i) ? '1' : '0';

	return out;
}

/*
std::string
OctreeChunk::ToString(int lvl)
{
	std::string output;
	std::string indent;
	for (int i = 0; i <= lvl; ++i)
		indent += "\t>";

	output += indent;
	output += "OctreeChunk lvl: " + std::to_string(lvl);
	output += "\n" + indent + "Time to live: " + std::to_string(_timeToLive);
	output += "\n" + indent + "BoundingBox center, dimension: ";
	output += _area.GetPosition().ToString() + _area.GetDimension().ToString();
	output += "\n" + indent + "List of objects:\n";
	for (PhysicalObjects::iterator it = _objects.begin(); it != _objects.end(); ++it)
		output += indent + (*it)->GetName() + " " + (*it)->GetTransform().GetPosition().ToString() + "\n";
	output += indent + "End of objects\n";
	output += indent + "Children: " + toBinary(_usedChildren) + "\n";

	for (uint8_t used = _usedChildren, i = 0; used > 0; used >>= 1, ++i)
		if (used & 1)
			output += _children[i]->ToString(lvl + 1);

	return output;
}
*/

void
OctreeChunk::Add(Chunk* chunk)
{
	_pendingChunks.push(chunk);
	_ready = false;
}

void
OctreeChunk::Add(const Chunks& chunks)
{
	for (Chunks::const_iterator it = chunks.begin(); it != chunks.end(); ++it) {
		Chunk* chunk = *it;
		_pendingChunks.push(chunk);
	}
	_ready = false;
}
}