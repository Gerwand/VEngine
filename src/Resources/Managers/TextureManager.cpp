#include "TextureManager.h"

namespace vengine {

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	for (NameIndex::iterator i = _nameIndex.begin(); i != _nameIndex.end(); ++i) {
		_textures.Release(i->second);
	}
}

TextureManager::HTexture
TextureManager::GetTexture(const std::string& name)
{
	NameIndexInsertRc rc = _nameIndex.insert(std::make_pair(name, HTexture()));

	//If this element is new
	if (rc.second) {
		Texture* tex = _textures.Acquire(rc.first->second);
		if (tex->Init(name)) {
			DeleteTexture(rc.first->second);
			rc.first->second = HTexture();
		}
	}

	return (rc.first->second);
}

void
TextureManager::DeleteTexture(HTexture htex)
{
	Texture* tex = _textures.GetItem(htex);
	assert(tex != nullptr, "Invalid handle.");

	_nameIndex.erase(tex->GetName());
	_textures.Release(htex);
}

void
TextureManager::DeleteAllTextures()
{
	for (NameIndex::iterator i = _nameIndex.begin(); i != _nameIndex.end(); ++i) {
		_textures.Release(i->second);
	}
	_nameIndex.clear();
}

int
TextureManager::LoadTexture(HTexture htex, const std::string& fileName)
{
	Texture* tex = _textures.GetItem(htex);
	assert(tex != nullptr, "Invalid handle.");

	return tex->LoadTexture(fileName);
}

GLuint
TextureManager::GetGlHandle(HTexture htex)
{
	Texture* tex = _textures.GetItem(htex);
	assert(tex != nullptr, "Invalid handle.");

	return *tex;
}

void
TextureManager::BindTexture(HTexture htex, int unit)
{
	Texture* tex = _textures.GetItem(htex);
	assert(tex != nullptr, "Invalid handle.");

	if (_activeTex[unit] != htex) {
		_activeTex[unit] = htex;
		tex->Bind(unit);
	}
}

}