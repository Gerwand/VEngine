#pragma once

#include "Resources/Handle.h"
#include "HandleManager.h"
#include "Singleton.h"
#include "Resources/Images/Texture.h"

#include <string>
#include <map>

namespace vengine {


class TextureManager : public Singleton<TextureManager>
{
public:
	struct TextureTag {};
	typedef Handle<TextureTag> HTexture;

private:
	typedef HandleManager<Texture, HTexture> HTextureManager;
	typedef std::map<std::string, HTexture, istring_less> NameIndex;
	typedef std::pair<typename NameIndex::iterator, bool> NameIndexInsertRc;

public:

	TextureManager();
	~TextureManager();


	HTexture GetTexture(const std::string& name);

	void DeleteTexture(HTexture htex);
	void DeleteAllTextures();

	void BindTexture(HTexture htex, int unit = 0);

	int LoadTexture(HTexture htex, const std::string& fileName);

	GLuint GetGlHandle(HTexture htex);

private:
	HTextureManager _textures;	/* Manager for handles.					*/
	NameIndex _nameIndex;		/* Map associating names with handles.	*/

	static const int _maxActive = 32;
	HTexture _activeTex[_maxActive];

};

/* Define for easier access to manager, like global variable. */
#define textureManager TextureManager::GetSingleton()

}