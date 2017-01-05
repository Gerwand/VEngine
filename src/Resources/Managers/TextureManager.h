#pragma once

#include "Handle.h"
#include "HandleManager.h"
#include "Singleton.h"
#include "Texture.h"

#include <string>
#include <map>

namespace vengine {

/**
*	Class representating ShaderManager for managing shaders.
*	It inheritances from class Singleton, so only one object of this class can exist at any time.
*	It it highly advisable to create instance of singleton ShaderFileManager before using this class.
*/
class TextureManager : public Singleton<TextureManager>
{
public:
	/* Tag type for Shader handle. */
	struct TextureTag {};
	/* Handle for Shader. */
	typedef Handle<TextureTag> HTexture;
private:
	/* HandleManager for Shader. */
	typedef HandleManager<Texture, HTexture> HTextureManager;

	/**
	*	Map associating handle with the name of the resource.
	*	As third template parameter, class for string comparison is passed.
	*/
	typedef std::map<std::string, HTexture, istring_less> NameIndex;
	/* Typedef for inserting item into the map */
	typedef std::pair<typename NameIndex::iterator, bool> NameIndexInsertRc;


public:

	/**
	*	Default constructor doing nothing.
	*/
	TextureManager();
	/**
	*	Destructor frees all earlier allocated resources.
	*/
	~TextureManager();

	/**
	*	Get handle to the shader with given name. If it is opened for first time, resource is allocated, initialized and
	*	associated with the handle. If type will be different than the previous one, new one will override older one.
	*	However, recompilation will be needed.
	*
	*	@param name unique name of the shader.
	*	@param type type of the shader.
	*	@return Handle to the Shader resource.
	*/
	HTexture GetTexture(const std::string& name);

	/**
	*	Delete given shader.
	*
	*	@param hshader handle to the shader.
	*/
	void DeleteTexture(HTexture htex);
	/**
	*	Delete all shaders in manager.
	*/
	void DeleteAllTextures();

	void BindTexture(HTexture htex, int unit = 0);

	/**
	*	Loads Shader data from file. Singleton ShaderFileManager must exist before calling this function.
	*
	*	@param hshader handle for the shader.
	*	@param fileName name of the file for reading.
	*	@return Error code, 0 if suceed.
	*/
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