#include "Mesh.h"


namespace vengine {

bool Mesh::_wireFrame = false;
Vector3 Mesh::_wireColor = Vector3(0.0f, 1.0f, 0.0f);

void
Mesh::Init(const std::string& name)
{
	Renderable::Init();	
	_name = name;
	_tex = 0;
	_textured = true;
}

void
Mesh::FillInfo(RenderInfo* info)
{
	Renderable::FillInfo(info);
	if (_wireFrame) {
		info->wired = true;
		info->color = _wireColor;
	}
	else {
		info->wired = false;

		if (_textured && _tex != 0 ) {
			info->tex = _tex;
			info->textured = true;
		}
		else {
			info->tex = 0;
			info->textured = false;
		}
	}
	info->drawType = GL_TRIANGLES;
}

void
Mesh::Draw(Renderer* renderer)
{
	RenderInfo info;

	DrawStart(&info);

	renderer->Draw(info, Renderer::STANDARD);

	DrawEnd();
}

void 
Mesh::SetTexture(unsigned int tex)
{
	_tex = tex;
}

void
Mesh::SetTexture(const std::string& name)
{
	unsigned int tex = textureManager.GetTexture(name);
	_tex = tex;
}

unsigned int 
Mesh::GetTexture() const
{
	return _tex;
}


}