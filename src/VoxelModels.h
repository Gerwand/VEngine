#pragma once
namespace vengine {

static unsigned char swordVoxels[] = {
	0, 0, 3, 3, 3, 0, 0,
	0, 0, 0, 4, 0, 0, 0,
	0, 0, 0, 4, 0, 0, 0,
	3, 3, 3, 3, 3, 3, 3,
	3, 0, 4, 4, 4, 0, 3,
	0, 0, 4, 4, 4, 0, 0,
	0, 0, 4, 4, 4, 0, 0,
	0, 0, 4, 4, 4, 0, 0,
	0, 0, 4, 4, 4, 0, 0,
	0, 0, 0, 4, 0, 0, 0
};

static struct {
	const int x = 7;
	const int y = 10;
	const int z = 1;
} swordDim;

static float swordSize = 0.035f;

static unsigned char dirt[] = { 1 };
static unsigned char grass[] = { 2 };
static unsigned char wood[] = { 3 };
static unsigned char stone[] = { 4 };

static struct {
	const int x = 1;
	const int y = 1;
	const int z = 1;
} cubeDim;

static float cubeSize = 0.1f;

static unsigned char pickVoxels[] = {
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	4, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 4,
	4, 4, 4, 4, 0, 0, 3, 3, 0, 0, 4, 4, 4, 4,
	0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0,
	0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0,
	4, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 4,
	4, 4, 4, 4, 0, 0, 3, 3, 0, 0, 4, 4, 4, 4,
	0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0,
	0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0
};

static struct {
	const int x = 14;
	const int y = 20;
	const int z = 2;
} pickDim;

static float pickSize = 0.018f;

static unsigned char armVoxels[] = {
	7, 6, 8
};

static struct {
	const int x = 3;
	const int y = 1;
	const int z = 1;
} armDim;

static float armSize = 0.075f;

static unsigned char skeletonHead[] = { 9 };
static float skelHeadSize = 0.4f;

static unsigned char skelVoxels[] = {
	0,  10,  0, 10,  0,
	0,  10,  0, 10,  0,
	0,  10,  0, 10,  0,
	0,  10,  0, 10,  0,
	0,  10, 10, 10,  0,
	0,  10, 10, 10,  0,
	0,  10, 10, 10,  0,
	0,  10, 10, 10,  0,
	10, 10, 10, 10, 10,
	0,  0,  10,  0,  0,

	0,  10,  0, 10,  0,
	0,  10,  0, 10,  0,
	0,  10,  0, 10,  0,
	0,  10,  0, 10,  0,
	0,  10, 10, 10,  0,
	0,  10, 10, 10,  0,
	0,  10, 10, 10,  0,
	0,  10, 10, 10,  0,
	10, 10, 10, 10, 10,
	0,   0,  0,  0,  0,

	0,   0,  0,  0,  0,
	0,   0,  0,  0,  0,
	0,   0,  0,  0,  0,
	0,   0,  0,  0,  0,
	0,   0,  0,  0,  0,
	0,   0,  0,  0,  0,
	0,   0,  0,  0,  0,
	0,   0,  0,  0,  0,
	10,  0,  0,  0, 10,
	0,   0,  0,  0,  0,

	0,   0,  0,  0,  0,
	0,   0,  0,  0,  0,
	0,   0,  0,  0,  0,
	0,   0,  0,  0,  0,
	0,   0,  0,  0,  0,
	0,   0,  0,  0,  0,
	0,   0,  0,  0,  0,
	0,   0,  0,  0,  0,
	10,  0,  0,  0, 10,
	0,   0,  0,  0,  0,

	0,   0,  0,  0,  0,
	0,   0,  0,  0,  0,
	0,   0,  0,  0,  0,
	0,   0,  0,  0,  0,
	0,   0,  0,  0,  0,
	0,   0,  0,  0,  0,
	0,   0,  0,  0,  0,
	0,   0,  0,  0,  0,
	10,  0,  0,  0, 10,
	0,   0,  0,  0,  0
};
static struct {
	const int x = 5;
	const int y = 10;
	const int z = 5;
} skelDim;
static float skelSize = 0.15f;
}