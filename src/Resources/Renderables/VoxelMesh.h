#pragma once

#include "Mesh.h"

namespace vengine {

/*
* Special mesh object, that is using global atlas and is mapping textures in special way on quads.
* UV's are mapped in special way to repeat one given type of the texture in whole quad. 
*/
class VoxelMesh : public Mesh
{
public:
	virtual void Draw(Renderer* renderer);

	static void SetAtlas(unsigned int atlas);
	static void SetAtlas(const std::string& name);
	static unsigned int GetAtlas();

protected:
	static unsigned int _atlas;

	virtual void ActivateAttributes();
	virtual void FillInfo(RenderInfo* info);
};

}