#include "PerlinNoise.h"

namespace vengine {

PerlinNoise::PerlinNoise()
{
	SetSeed(_seed);
}

PerlinNoise::PerlinNoise(unsigned int seed)
{
	_seed = seed;
	SetSeed(_seed);
}


void
PerlinNoise::SetSeed(unsigned int seed) 
{
	_permutation.resize(256);
	std::iota(_permutation.begin(), _permutation.end(), 0);

	std::default_random_engine engine(seed);
	std::shuffle(_permutation.begin(), _permutation.end(), engine);

	_permutation.insert(_permutation.end(), _permutation.begin(), _permutation.end());
}

float 
PerlinNoise::Fade(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float
PerlinNoise::Lerp(float t, float a, float b)
{
	return fma(t, b - a, a);
}

float 
PerlinNoise::Grad(int hash, float x, float y, float z)
{
	int h = hash & 15;
	float u = h < 8 ? x : y;
	float v = h < 4 ? y : (h == 12 || h == 14 ? x : z);

	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float
PerlinNoise::GetNoise(float x, float y, float z) {
	// Find the unit cube that contains the point
	int X = (int)floor(x) & 255;
	int Y = (int)floor(y) & 255;
	int Z = (int)floor(z) & 255;

	// Find relative x, y,z of point in cube
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	// Compute fade curves for each of x, y, z
	float u = Fade(x);
	float v = Fade(y);
	float w = Fade(z);

	int A = _permutation[X] + Y, AA = _permutation[A] + Z, AB = _permutation[A + 1] + Z,      // HASH COORDINATES OF
		B = _permutation[X + 1] + Y, BA = _permutation[B] + Z, BB = _permutation[B + 1] + Z;      // THE 8 CUBE CORNERS,

																 
	// AND ADD BLENDED RESULTS FROM  8 CORNERS  OF CUBE
	float p1 = Lerp(u, Grad(_permutation[AA], x, y, z), Grad(_permutation[BA], x - 1, y, z));
	float p2 = Lerp(u, Grad(_permutation[AB], x, y - 1, z), Grad(_permutation[BB], x - 1, y - 1, z));
	float p3 = Lerp(u, Grad(_permutation[AA + 1], x, y, z - 1), Grad(_permutation[BA + 1], x - 1, y, z - 1));
	float p4 = Lerp(u, Grad(_permutation[AB + 1], x, y - 1, z - 1), Grad(_permutation[BB + 1], x - 1, y - 1, z - 1));


	return Lerp(w, Lerp(v, p1, p2), Lerp(v, p3, p4));
}

}