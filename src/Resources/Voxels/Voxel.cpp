#include "Voxel.h"

namespace vengine {

const Voxel::AtlasCoords Voxel::_offsets[NUM_TYPES * 6] = {
	//DIRT
	{ 0, texsPerRow - 1 }, //NORTH
	{ 0, texsPerRow - 1 }, //SOUTH
	{ 0, texsPerRow - 1 }, //EAST
	{ 0, texsPerRow - 1 }, //WEST
	{ 0, texsPerRow - 1 }, //TOP
	{ 0, texsPerRow - 1 }, //BOTTOM
	//GRASS
	{ 2, texsPerRow - 1 }, //NORTH
	{ 2, texsPerRow - 1 }, //SOUTH
	{ 2, texsPerRow - 1 }, //EAST
	{ 2, texsPerRow - 1 }, //WEST
	{ 1, texsPerRow - 1 }, //TOP
	{ 0, texsPerRow - 1 }, //BOTTOM
	//WOOD
	{ 4, texsPerRow - 1 }, //NORTH
	{ 4, texsPerRow - 1 }, //SOUTH
	{ 4, texsPerRow - 1 }, //EAST
	{ 4, texsPerRow - 1 }, //WEST
	{ 3, texsPerRow - 1 }, //TOP
	{ 3, texsPerRow - 1 }, //BOTTOM
	//STONE
	{ 5, texsPerRow - 1 }, //NORTH
	{ 5, texsPerRow - 1 }, //SOUTH
	{ 5, texsPerRow - 1 }, //EAST
	{ 5, texsPerRow - 1 }, //WEST
	{ 5, texsPerRow - 1 }, //TOP
	{ 5, texsPerRow - 1 }, //BOTTOM
	//LEAFS
	{ 6, texsPerRow - 1 }, //NORTH
	{ 6, texsPerRow - 1 }, //SOUTH
	{ 6, texsPerRow - 1 }, //EAST
	{ 6, texsPerRow - 1 }, //WEST
	{ 6, texsPerRow - 1 }, //TOP
	{ 6, texsPerRow - 1 }, //BOTTOM
	//CLOTH1
	{ 7, texsPerRow - 1 }, //NORTH
	{ 7, texsPerRow - 1 }, //SOUTH
	{ 7, texsPerRow - 1 }, //EAST
	{ 7, texsPerRow - 1 }, //WEST
	{ 7, texsPerRow - 1 }, //TOP
	{ 7, texsPerRow - 1 }, //BOTTOM
	//CLOTH2
	{ 8, texsPerRow - 1 }, //NORTH
	{ 8, texsPerRow - 1 }, //SOUTH
	{ 8, texsPerRow - 1 }, //EAST
	{ 8, texsPerRow - 1 }, //WEST
	{ 8, texsPerRow - 1 }, //TOP
	{ 8, texsPerRow - 1 }, //BOTTOM
	//BODY
	{ 9, texsPerRow - 1 }, //NORTH
	{ 9, texsPerRow - 1 }, //SOUTH
	{ 9, texsPerRow - 1 }, //EAST
	{ 9, texsPerRow - 1 }, //WEST
	{ 9, texsPerRow - 1 }, //TOP
	{ 9, texsPerRow - 1 }, //BOTTOM
	//SKELETONFACE
	{ 10, texsPerRow - 1 }, //NORTH
	{ 11, texsPerRow - 1 }, //SOUTH
	{ 11, texsPerRow - 1 }, //EAST
	{ 11, texsPerRow - 1 }, //WEST
	{ 11, texsPerRow - 1 }, //TOP
	{ 11, texsPerRow - 1 }, //BOTTOM
	//SKELETON
	{ 11, texsPerRow - 1 }, //NORTH
	{ 11, texsPerRow - 1 }, //SOUTH
	{ 11, texsPerRow - 1 }, //EAST
	{ 11, texsPerRow - 1 }, //WEST
	{ 11, texsPerRow - 1 }, //TOP
	{ 11, texsPerRow - 1 } //BOTTOM
};

Voxel::Voxel() : _type(NONE)
{

}

Voxel::Voxel(unsigned char type) : _type(type)
{

}


Voxel::Voxel(const Voxel& source)
{
	_type = source._type;
}

void
Voxel::SetType(unsigned char type)
{
	_type = type;
}

unsigned char
Voxel::GetType() const
{
	return _type;
}

bool
Voxel::IsEmpty() const
{
	return _type == NONE;
}

bool
Voxel::operator==(const Voxel& other) const
{
	return _type == other._type;
}

bool
Voxel::operator!=(const Voxel& other) const
{
	return _type != other._type;
}

Voxel::operator unsigned char()
{
	return _type;
}

void 
Voxel::GetAtlasLocation(Side face, int* x, int* y) const
{
	assert(_type > 0, "Trying to get type for NONE voxel type");
	assert(_type <= NUM_TYPES, "Trying to get type for NONE voxel type");
	int index = (_type - 1) * 6 + face;
	
	const AtlasCoords& coords = _offsets[index];

	*x = coords.x;
	*y = coords.y;
}

}