#include "VoxelArray3D.h"

namespace vengine {

VoxelArray3D::VoxelArray3D() : _voxels(nullptr), _numElements(0), _voxelSize(1.0f)
{
	_empty = true;
}

VoxelArray3D::VoxelArray3D(int sx, int sy, int sz) : _sx(sx), _sy(sy), _sz(sz)
{
	_empty = true;

	_numElements = sx*sy*sz;
	_voxels = new Voxel[_numElements];
	_sx = sx;
	_sy = sy;
	_sz = sz;
	_voxelSize = 1.0f;

	_center = Vector3((float)_sx, (float)_sy, (float)_sz) / 2.0f * _voxelSize;
}

VoxelArray3D::~VoxelArray3D()
{
	delete[] _voxels;
}

void
VoxelArray3D::Init(const std::string& name, int sx, int sy, int sz)
{
	assert(!IsValid(), "Cannot initialize not empty voxel array.");

	_numElements = sx*sy*sz;
	_name = name;
	if (_numElements > 0) {
		_voxels = new Voxel[_numElements];
	}
	_dimension[0] = sx;
	_dimension[1] = sy;
	_dimension[2] = sz; 
	_empty = true;
	_center = Vector3((float)_sx, (float)_sy, (float)_sz) / 2.0f * _voxelSize;
}

void
VoxelArray3D::SetDimension(int x, int y, int z)
{
	delete[] _voxels;
	_numElements = x * y * z;

	if (_numElements > 0) {
		_voxels = new Voxel[_numElements];
	}
	else {
		_voxels = nullptr;
	}

	_sx = x;
	_sy = y;
	_sz = z;
	_empty = true;
	_center = Vector3((float)_sx, (float)_sy, (float)_sz) / 2.0f * _voxelSize;
}

void
VoxelArray3D::Clear()
{
	assert(IsValid(), "Cannot clear unitialized array.");
	delete[] _voxels;
	_voxels = nullptr;
	_numElements = 0;
	_voxelSize = 1.0f;
}

const Voxel& 
VoxelArray3D::Get(int x, int y, int z) const
{
	int index = x + _sx*(y + _sy*z);
	assert(index < _numElements, "Index out of bound: %d/%d", index, _numElements-1);

	return _voxels[index];
}

const Voxel&
VoxelArray3D::Get(int *coords) const
{
	int index = coords[0] + _sx*coords[1] + _sy*coords[2];
	assert(index < _numElements, "Index out of bound");

	return _voxels[index];
}

const Voxel&
VoxelArray3D::Get(const Vector3& coords) const
{
	int index = (int)coords[0] + _sx*(int)coords[1] + _sy*(int)coords[2];
	assert(index < _numElements, "Index out of bound");

	return _voxels[index];
}

/*
void
VoxelArray3D::SetTypes(unsigned char ***types)
{
	//It will be called only when loading model, so it does
	//not have to be very fast
	int index = 0;
	for (int z = 0; z < _sz; ++z) {
		for (int y = 0; y < _sy; ++y) {
			for (int x = 0; x < _sx; ++x) {
				_voxels[index++] = Voxel(types[z][y][x]);
			}
		}
	}
}
*/

void
VoxelArray3D::SetTypes(unsigned char *types)
{
	for (int i = 0; i < _numElements; ++i) 
		_voxels[i].SetType(types[i]); 
	_empty = false;
}

void
VoxelArray3D::GenerateMesh(VoxelMesh* mesh)
{
	assert(IsValid(), "Cannot generate mesh for empty Voxel Array.");

	mesh->ClearVertices();

	int total_quads = 0;
	bool front = false;
	for (int i = 0; i < 2; ++i, front = !front) {
		for (int dir = 0; dir < 3; dir++) {

			int u = (dir + 1) % 3;
			int v = (dir + 2) % 3;

			const Voxel** mask = new const Voxel*[_dimension[u] * _dimension[v]];
			memset(mask, NULL, _dimension[u] * _dimension[v] * sizeof(Voxel *));
			int coords[3] = { 0, 0, 0 };
			//This arrays helps to determine next plane from which
			//face for comparision will be get
			int next[3] = { 0, 0, 0 };
			next[dir] = 1;

			Voxel::Side face;
			//Determine which face to use
			switch (dir) {
			//Axis X
			case 0:
				face = front ? Voxel::EAST : Voxel::WEST;
				break;
			//Axis Y
			case 1:
				face = front ? Voxel::TOP: Voxel::BOTTOM;
				break;
			//Axis Z
			case 2:
				face = front ? Voxel::NORTH : Voxel::SOUTH;
				break;
			}

			//For each layer
			coords[dir] = -1;
			while (coords[dir] < _dimension[dir]) {

				//Creating mask
				int n = 0;
				for (coords[v] = 0; coords[v] < _dimension[v]; coords[v]++) {
					for (coords[u] = 0; coords[u] < _dimension[u]; coords[u]++) {
						const Voxel* earlier = (coords[dir] >= 0) ? &Get(coords[0], coords[1], coords[2]): NULL;
						const Voxel* current = (coords[dir] < _dimension[dir] - 1) ?
							&Get(coords[0] + next[0], coords[1] + next[1], coords[2] + next[2]) : NULL;

						if (earlier != NULL && earlier->IsEmpty())
							earlier = NULL;
						if (current != NULL && current->IsEmpty())
							current = NULL;

						//Merge blocks
						if (current != NULL && earlier != NULL && !earlier->IsTransparent() &&  !current->IsTransparent())
							mask[n++] = NULL;
						else
							mask[n++] = front ? earlier : current;


					}
				}

				coords[dir]++;

				//Mask
				n = 0;
				int w, h;
				for (int i = 0; i < _dimension[v]; i++) {
					for (int j = 0; j < _dimension[u]; ) {

						if (mask[n] != NULL) {
							total_quads += 1;
							//Width first
							for (w = 1; j + w < _dimension[u]; ++w) {
								int index = n + w;
								if (mask[index] == NULL || *mask[index] != *mask[n])
									break;
							}

							//Height
							bool finish = false;
							for (h = 1; i + h < _dimension[v]; h++) {
								for (int k = 0; k < w; k++) {
									int index = n + k + h * _dimension[u];
									if (mask[index] == NULL || *mask[index] != *mask[n]) {
										finish = true;
										break;
									}
								}
								if (finish) {
									break;
								}
							}

							coords[u] = j;
							coords[v] = i;

							int du[3] = { 0, 0, 0 };
							int dv[3] = { 0, 0, 0 };
							du[u] = w;
							dv[v] = h;

							Vector3 p0 = Vector3(float(coords[0]), float(coords[1]), float(coords[2])),
								p1 = Vector3(float(coords[0] + du[0]), float(coords[1] + du[1]), float(coords[2] + du[2])),
								p2 = Vector3(float(coords[0] + du[0] + dv[0]), float(coords[1] + du[1] + dv[1]), float(coords[2] + du[2] + dv[2])),
								p3 = Vector3(float(coords[0] + dv[0]), float(coords[1] + dv[1]), float(coords[2] + dv[2]));

							InsertQuad(mesh, *mask[n], face, front, w, h,
									   p0, p1, p2, p3);
							//Removing used elements
							for (int l = 0; l < h; ++l) {
								for (int k = 0; k < w; ++k) {
									mask[n + k + l * _dimension[u]] = NULL;
								}
							}
							j += w;
							n += w;
						}
						else {
							j++;
							n++;
						}
					}
				}
			}

			delete[] mask;
		}
	}
	if (total_quads == 0)
		_empty = true;
}

bool
VoxelArray3D::IsValid() const
{
	return _voxels != NULL;
}

void
VoxelArray3D::InsertQuad(VoxelMesh* mesh, const Voxel& voxel, Voxel::Side side, bool front, int w, int h,
				const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
	Vertices vertices(4);
	Indices indices(6);
	int indicesOffset = mesh->GetVertices().size();

	if (front) {
		indices[0] = indicesOffset + 2; indices[1] = indicesOffset + 3, indices[2] = indicesOffset;
		indices[3] = indicesOffset; indices[4] = indicesOffset + 1, indices[5] = indicesOffset + 2;
	}
	else {
		indices[0] = indicesOffset; indices[1] = indicesOffset + 3, indices[2] = indicesOffset + 2;
		indices[3] = indicesOffset + 2; indices[4] = indicesOffset + 1, indices[5] = indicesOffset;
	}


	vertices[0].position = p0 * _voxelSize - _center;
	vertices[1].position = p1 * _voxelSize - _center;
	vertices[2].position = p2 * _voxelSize - _center;
	vertices[3].position = p3 * _voxelSize - _center;

	switch (side) {
	case Voxel::NORTH:
		for(int i = 0; i < 4; ++i)
			vertices[i].normal = Vector3::forward;
		break;
	case Voxel::SOUTH:
		for (int i = 0; i < 4; ++i)
			vertices[i].normal = Vector3::backward;
		break;
	case Voxel::EAST:
		for (int i = 0; i < 4; ++i)
			vertices[i].normal = Vector3::right;
		break;
	case Voxel::WEST:
		for (int i = 0; i < 4; ++i)
			vertices[i].normal = Vector3::left;
		break;
	case Voxel::TOP:
		for (int i = 0; i < 4; ++i)
			vertices[i].normal = Vector3::up;
		break;
	case Voxel::BOTTOM:
		for (int i = 0; i < 4; ++i)
			vertices[i].normal = Vector3::down;
		break;
	}
	unsigned char type = voxel.GetType();

	int u, v;
	voxel.GetAtlasLocation(side, &u, &v);


	for (int i = 0; i < 4; ++i) {
		vertices[i].atlasOffset[0] = u;
		vertices[i].atlasOffset[1] = v;
	}

	bool yAxe = side == Voxel::EAST || side == Voxel::WEST;

	vertices[0].texUV.Set(0.0f, 0.0f);

	if (yAxe) {
		vertices[1].texUV.Set(0.0f, float(w));
		vertices[2].texUV.Set(float(h), float(w));
		vertices[3].texUV.Set(float(h), 0.0f);
	}
	else {
		vertices[1].texUV.Set(float(w), 0.0f);
		vertices[2].texUV.Set(float(w), float(h));
		vertices[3].texUV.Set(0, float(h));
	}
	mesh->AddVertices(vertices, indices);
}

}