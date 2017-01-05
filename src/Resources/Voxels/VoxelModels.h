#pragma once

static unsigned char swordVoxels[] = {
	0, 0, 3, 3, 3, 0, 0,
	0, 0, 0, 4, 0, 0, 0,
	0, 0, 0, 4, 0, 0, 0,
	3, 3, 3, 3, 3, 3, 3,
	3, 0, 4, 4, 4, 0, 3,
	0, 0, 4, 4, 4, 0, 0,
	0, 0, 4, 2, 4, 0, 0,
	0, 0, 4, 2, 4, 0, 0,
	0, 0, 4, 4, 4, 0, 0,
	0, 0, 2, 4, 2, 0, 0,
	0, 0, 0, 2, 0, 0, 0,

	0, 0, 3, 3, 3, 0, 0,
	0, 0, 0, 4, 0, 0, 0,
	0, 0, 0, 4, 0, 0, 0,
	3, 3, 3, 3, 3, 3, 3,
	3, 0, 4, 4, 4, 0, 3,
	0, 0, 4, 4, 4, 0, 0,
	0, 0, 4, 2, 4, 0, 0,
	0, 0, 4, 2, 4, 0, 0,
	0, 0, 4, 4, 4, 0, 0,
	0, 0, 2, 4, 2, 0, 0,
	0, 0, 0, 2, 0, 0, 0

};
static struct {
	const int x = 7;
	const int y = 11;
	const int z = 2;
} swordSize;

static unsigned char cube[] = {
	1
};
static struct {
	const int x = 1;
	const int y = 1;
	const int z = 1;
} cubeSize;
