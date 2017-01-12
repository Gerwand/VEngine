#pragma once

#include "Math/PerlinNoise.h"
#include "Resources/Voxels/Voxel.h"
#include "Resources/Voxels/Chunk.h"

namespace vengine {

/* Class for generating terrain with given properties using Perlin noise*/
class TerrainGenerator
{
public:
	/* Uses default seed 518331203u and values for smooth terrain */
	TerrainGenerator();
	/* Construct object with given seed */
	TerrainGenerator(unsigned int seed);
	/* Construct object with default seed and sample settings */
	TerrainGenerator(int seaOffset, float rockOffset, float rockSharpness);
	/* Allows to customize some of the parameters */
	TerrainGenerator(unsigned int seed, int seaOffset, float rockOffset, float rockSharpness);

	/* Set seed for the generators */
	void SetSeed(unsigned int seed);

	/* Set offset from the sea lvl */
	void SetSeaOffset(int offset);

	/* Set how deep rocks should appear */
	void SetRockOffset(float offset);
	/* Set how sharp rocks will differ from offset */
	void SetRockSharpness(float rockSharpness);

	/* Set global terrain smoothness */
	void SetSmoothness(float smoothness);

	/* Set local details of the terrain */
	void SetDetails(float details);
	/* Set how terrain will differ in height */
	void SetSpread(float spread);

	/* Get voxel with given coords */
	Voxel GetVoxel(int x, int y, int z);

	/* Fill chunk with values from generator and return true if generated chunk is not empty */
	bool GetChunk(Chunk* source);

private:
	static const float _baseDetails; /* Details given by user will be divided by this value */

	PerlinNoise _perlinGenerator; /* Perlin noise generator */
	int _seaOffset;			/* Offset from the sea level */
	float _spread;			/* Diversity in high of the terrain */
	float _rockOffset;		/* How deep rocks will appear below dirt */
	float _rockSharpness;	/* The higher value, the rocks will be spread more randomly near offset */
	float _details;			/* Local details of the terrain, it is detail given by user divided by _baseDetails */
	float _smoothness;		/* Smoothness of the terrain in global scale */

	unsigned int _seed;		/* Seed for the generators */

	std::default_random_engine _generator;			/* Generator used for rocks distribution */
	std::normal_distribution<float> _distribution;	/* Normal distribution of the rocks */
};


inline void
TerrainGenerator::SetSeed(unsigned int seed)
{
	_seed = seed;
	_perlinGenerator.SetSeed(_seed);
	_generator.seed(_seed);
}

inline void
TerrainGenerator::SetSeaOffset(int offset)
{
	_seaOffset = offset;
}

inline void
TerrainGenerator::SetRockOffset(float offset)
{
	_rockOffset = offset;
	_distribution = std::normal_distribution<float>(_rockOffset, _rockSharpness);
}

inline void
TerrainGenerator::SetRockSharpness(float rockSharpness)
{
	_rockSharpness = rockSharpness;
	_distribution = std::normal_distribution<float>(_rockOffset, _rockSharpness);
}


inline void 
TerrainGenerator::SetSmoothness(float smoothness)
{
	_smoothness = smoothness;
}


inline void 
TerrainGenerator::SetDetails(float details)
{
	_details = _baseDetails/details;
}

inline void 
TerrainGenerator::SetSpread(float spread)
{
	_spread = spread;
}

}