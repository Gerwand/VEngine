#pragma once

#include "PerlinNoise.h"
#include "Voxel.h"
#include "Chunk.h"

namespace vengine {

class TerrainGenerator
{
public:
	TerrainGenerator();
	TerrainGenerator(unsigned int seed);
	TerrainGenerator(int seaOffset, float rockOffset, float rockSharpness);
	TerrainGenerator(unsigned int seed, int seaOffset, float rockOffset, float rockSharpness);

	void SetSeed(unsigned int seed);
	void SetSeaOffset(int offset);
	void SetRockOffset(float offset);
	void SetRockSharpness(float rockSharpness);
	void SetSmoothness(float smoothness);
	void SetDetails(float details);
	void SetSpread(float spread);

	Voxel GetVoxel(int x, int y, int z);
	bool GetChunk(Chunk* source);

private:
	static const float _baseDetails;

	PerlinNoise _perlinGenerator;
	int _seaOffset;
	float _spread;
	float _rockOffset;
	float _rockSharpness;
	float _details;
	float _smoothness;

	unsigned int _seed;

	std::default_random_engine _generator;
	std::normal_distribution<float> _distribution;
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