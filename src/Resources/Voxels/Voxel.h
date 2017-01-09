#pragma once

#include "Assert.h"
#include "Math/Vector3.h"

namespace vengine {

class Voxel {
public:
	enum Side {
		NORTH,
		SOUTH,
		EAST,
		WEST,
		TOP,
		BOTTOM
	};

	enum Type {
		NONE,
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

	void GetAtlasLocation(Side face, int* x, int* y) const;

	operator unsigned char();

	static const int texsPerRow = 16;
	bool IsTransparent() const;
	//bool GetVoxelColor(Type type, Vector3* color) const;

private:
	unsigned char _type;

	//And multiply by 6 because for each side there is other one
	static const struct AtlasCoords {
		int x;
		int y;
	} _offsets[NUM_TYPES * 6];
	//static Vector3 _colors[NUM_TYPES];
};


inline bool 
Voxel::IsTransparent() const
{
	return _type == LEAFS;
}

}