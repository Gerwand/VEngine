#pragma once

#include "Assert.h"
#include "Math/Vector3.h"

namespace vengine {

/*
* Class representing voxel. 
*/
class Voxel {
public:
	/* Sides of the voxels, used for determining atlas textures for each face */
	enum Side {
		NORTH = 0,
		SOUTH,
		EAST,
		WEST,
		TOP,
		BOTTOM
	};

	/* Types of the voxel */
	enum Type {
		NONE = 0,
		DIRT,
		GRASS,
		WOOD,
		STONE,
		LEAFS,
		CLOTH1,
		CLOTH2,
		BODY,
		SKELETONFACE,
		SKELETON,
		NUM_TYPES = SKELETON
	};


	Voxel();
	Voxel(const Voxel& source);
	Voxel(unsigned char type);

	void SetType(unsigned char type);
	unsigned char GetType() const;

	bool IsEmpty() const;
	bool operator==(const Voxel& other) const;
	bool operator!=(const Voxel& other) const;

	/* Get x and y coordinates of the face from the atlas texture. It is hardcoded. */
	void GetAtlasLocation(Side face, int* x, int* y) const;

	operator unsigned char();

	/* Constant information for atlas texturing. How many textures can be stored in each row of the atlas. */
	static const int texsPerRow = 16;
	bool IsTransparent() const;
private:
	unsigned char _type;

	/* Structure containing information for each face which texture from atlas should it use */
	static const struct AtlasCoords {
		int x;
		int y;
	} _offsets[NUM_TYPES * 6];	/* And multiply by 6 - for each side */
};


inline bool 
Voxel::IsTransparent() const
{
	/* Currently transparent voxels are not supported. */
	return _type == LEAFS;
}

}