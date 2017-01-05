#pragma once

#include "Mesh.h"

namespace vengine {

class VoxelMesh : public Mesh
{
public:
	void Draw(Renderer* renderer);

	static void SetAtlas(unsigned int atlas);
	static void SetAtlas(const std::string& name);
	static unsigned int GetAtlas();

protected:

	static unsigned int _atlas;

	void ActivateAttributes();
	void FillInfo(RenderInfo* info);
};

}