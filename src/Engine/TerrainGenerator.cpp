#include "TerrainGenerator.h"

namespace vengine {

const float TerrainGenerator::_baseDetails = 32.0f;

TerrainGenerator::TerrainGenerator() : _perlinGenerator(518331203u)
{
	_seed = 518331203u;
	_seaOffset = 0;
	_rockOffset = 7.0f;
	_rockSharpness = 1.0f;
	_smoothness = 256.0f;
	_details = 32.0f;
	_generator.seed(_seed);
	_distribution = std::normal_distribution<float>(_rockOffset, _rockSharpness);
}

TerrainGenerator::TerrainGenerator(unsigned int seed) : _perlinGenerator(seed)
{
	_seed = seed;
	_seaOffset = 0;
	_rockOffset = 7.0f;
	_rockSharpness = 1.0f;
	_smoothness = 256.0f;
	_details = 32.0f;
	_spread = 32.0f;
	_generator.seed(_seed);
	_distribution = std::normal_distribution<float>(_rockOffset, _rockSharpness);
}

TerrainGenerator::TerrainGenerator(int seaOffset, float rockOffset, float rockSharpness) : _perlinGenerator(518331203u)
{
	_seed = 518331203u;

	_seaOffset = seaOffset;
	_rockOffset = rockOffset;
	_rockSharpness = rockSharpness;
	_smoothness = 256.0f;
	_details = 32.0f;
	_spread = 32.0f;

	_generator.seed(_seed);
	_distribution = std::normal_distribution<float>(_rockOffset, _rockSharpness);
}

TerrainGenerator::TerrainGenerator(unsigned int seed, int seaOffset, float rockOffset, float rockSharpness) : _perlinGenerator(seed)
{
	_seed = seed;

	_seaOffset = seaOffset;
	_rockOffset = rockOffset;
	_rockSharpness = rockSharpness;
	_smoothness = 256.0f;
	_details = 32.0f; 
	_spread = 32.0f;

	_generator.seed(_seed);
	_distribution = std::normal_distribution<float>(_rockOffset, _rockSharpness);
}



Voxel
TerrainGenerator::GetVoxel(int x, int y, int z)
{
	float height = _perlinGenerator.GetNoise(x / _smoothness, z / _smoothness, 0.2f);
	float rough = _perlinGenerator.GetNoise(x / _smoothness, z / _smoothness, 0.7f);
	float detail = _perlinGenerator.GetNoise(x / _details, z / _details, 0.3f);
	height = (height + (rough * detail)) * _spread + _seaOffset;


	int rocks = (int)_distribution(_generator);

	if (y < height) {
		if (y < height - 1 - rocks)
			return Voxel(Voxel::STONE);
		else if (y == height - 1)
			return Voxel(Voxel::GRASS);
		else
			return Voxel(Voxel::DIRT);
	}
	else {
		return Voxel(Voxel::NONE);
	}
}

bool
TerrainGenerator::GetChunk(Chunk* source)
{
	const int size = Chunk::dimension;
	const Vector3& offset = source->GetOffset();
	int added = 0;

	int heightMap[size][size];
	for (int z = 0; z < size; ++z) {
		for (int x = 0; x < size; ++x) {
			float height = _perlinGenerator.GetNoise((offset.x + x) / _smoothness, (offset.z + z) / _smoothness, 0.2f);
			float rough = _perlinGenerator.GetNoise((offset.x + x) / _smoothness, (offset.z + z) / _smoothness, 0.5f);
			float detail = _perlinGenerator.GetNoise((offset.x + x) / _details, (offset.z + z) / _details, 0.3f);

			height = (height + (rough * detail)) * _spread + _seaOffset;
			heightMap[z][x] = (int)height;
		}
	}

	for (int z = 0; z < size; ++z) {
		for (int x = 0; x < size; ++x) {
			int height = heightMap[z][x];

			for (int y = 0; y < size; ++y) {
				int index = x + size * (y + z * size);
				int rocks = (int)_distribution(_generator);

				int chunkHeight = y + (int)offset.y;

				if (chunkHeight < height) {
					if (chunkHeight < height - 1 - rocks)
						source->SetLocal(x, y, z, Voxel::STONE);
					else if (chunkHeight == height - 1)
						source->SetLocal(x, y, z, Voxel::GRASS);
					else
						source->SetLocal(x, y, z, Voxel::DIRT);
					++added;
				}
				else {
					source->SetLocal(x, y, z, Voxel::NONE);
				}

			}
		}
	}

	return (added != 0);
}

}