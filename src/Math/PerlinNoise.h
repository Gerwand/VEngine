#pragma once

#include <vector>
#include <random>
#include <numeric>
#include <algorithm>

namespace vengine {
class PerlinNoise {
public:
	PerlinNoise();
	PerlinNoise(unsigned int seed);

	void SetSeed(unsigned int seed);

	float GetNoise(float x, float y, float z);

private:
	unsigned int _seed = 518331203u;
	std::vector<int> _permutation;


	float Fade(float t);
	float Lerp(float t, float a, float b);
	float Grad(int hash, float x, float y, float z);
};

}