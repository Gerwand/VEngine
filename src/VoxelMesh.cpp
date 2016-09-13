#include "VoxelMesh.h"

namespace vengine {

unsigned int VoxelMesh::_atlas = 0;

void 
VoxelMesh::ActivateAttributes()
{
	Mesh::ActivateAttributes();
	_vao.ActivateBinded(ATLAS_OFFSET, 2, GL_INT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, atlasOffset));
}

void 
VoxelMesh::FillInfo(RenderInfo* info)
{
	_tex = _atlas;
	Mesh::FillInfo(info);
}

void 
VoxelMesh::Draw(Renderer* renderer)
{
	RenderInfo info;

	DrawStart(&info);
	renderer->Draw(info, Renderer::VOXEL);
	DrawEnd();
}

void 
VoxelMesh::SetAtlas(unsigned int atlas)
{
	_atlas = atlas;
}

void
VoxelMesh::SetAtlas(const std::string& name)
{
	_atlas = textureManager.GetTexture(name);
}


unsigned int 
VoxelMesh::GetAtlas()
{
	return _atlas;
}


}