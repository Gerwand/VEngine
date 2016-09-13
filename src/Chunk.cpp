#include "Chunk.h"

namespace vengine {


Chunk::Chunk() : _offset(Vector3::zeroes)
{
	VoxelArray3D::Init("Chunk" + _offset.ToString(), dimension, dimension, dimension);
	_changed = false;
	_model = Matrix4::GetTranslate(_center);
	_constraintHigh = _offset + (float)(Chunk::dimension - 1);
}

Chunk::Chunk(const Vector3& offset) : _offset(offset)
{
	VoxelArray3D::Init("Chunk" + _offset.ToString(), dimension, dimension, dimension);
	_changed = false;
	_model = Matrix4::GetTranslate(offset + _center);
	_constraintHigh = _offset + (float)(Chunk::dimension - 1);
}

void
Chunk::SetOffset(const Vector3& offset)
{
	_offset = offset;
	_model = Matrix4::GetTranslate(_offset + _center);
	_constraintHigh = _offset + (float)(Chunk::dimension - 1);
}

const Matrix4&
Chunk::GetModelMatrix()
{
	return _model;
}

void
Chunk::SetLocal(int x, int y, int z, unsigned char type)
{
	int index = x + dimension*(y + dimension*z);
	assert(index < _numElements, "Index out of range: %d / %d elements.", index, _numElements);
	_voxels[index] = type;

	_changed = true;
	if (_empty && type != Voxel::NONE)
		_empty = false;
}

void
Chunk::Set(const Vector3& coordinates, unsigned char type)
{
	int x = int(coordinates.x - _offset.x);
	assert(x < dimension && x >= 0, "X out of range: %d / %d dimension.", x, dimension);
	int y = int(coordinates.y - _offset.y);
	assert(y < dimension && y >= 0, "Y out of range: %d / %d dimension.", y, dimension);
	int z = int(coordinates.z - _offset.z);
	assert(z < dimension && z >= 0, "Z out of range: %d / %d dimension.", z, dimension);

	SetLocal(x, y, z, type);
}

const Voxel&
Chunk::GetLocal(int x, int y, int z) const
{

	assert(x < dimension && x >= 0, "X out of range: %d / %d dimension.", x, dimension);
	assert(y < dimension && y >= 0, "Y out of range: %d / %d dimension.", y, dimension);
	assert(z < dimension && z >= 0, "Z out of range: %d / %d dimension.", z, dimension);
	return VoxelArray3D::Get(x, y, z);
}

const Voxel&
Chunk::Get(const Vector3& coordinates)
{
	int x = (int)floor(coordinates.x - _offset.x);
	assert(x < dimension && x >= 0, "X out of range: %d / %d dimension.", x, dimension);
	int y = (int)floor(coordinates.y - _offset.y);
	assert(y < dimension && y >= 0, "Y out of range: %d / %d dimension.", y, dimension);
	int z = (int)floor(coordinates.z - _offset.z);
	assert(z < dimension && z >= 0, "Z out of range: %d / %d dimension.", z, dimension);

	return GetLocal(x, y, z);
}

const Vector3&
Chunk::GetOffset()
{
	return _offset;
}

const std::string&
Chunk::GetName() const
{
	return VoxelArray3D::GetName();
}

const Vector3& 
Chunk::GetCenter()
{
	return VoxelArray3D::GetCenter();
}

bool
Chunk::HasChanged()
{
	return _changed;
}

bool
Chunk::IsEmpty()
{
	return VoxelArray3D::IsEmpty();
}

void
Chunk::GenerateMesh(VoxelMesh* mesh)
{
	_changed = false;
	VoxelArray3D::GenerateMesh(mesh);
}

}