#pragma once

#include "Resources/Handle.h"
#include "HandleManager.h"
#include "Singleton.h"
#include "Resources/OGL/Shader.h"
#include "ShaderFileManager.h"

#include <string>
#include <map>

namespace vengine {

/**
*	Class representating ShaderManager for managing shaders.
*	It inheritances from class Singleton, so only one object of this class can exist at any time.
*	It it highly advisable to create instance of singleton ShaderFileManager before using this class.
*/
class ShaderManager : public Singleton<ShaderManager>
{
public:
	/* Tag type for Shader handle. */
	struct ShaderTag {};
	/* Handle for Shader. */
	typedef Handle<ShaderTag> HShader;
private:
	/* HandleManager for Shader. */
	typedef HandleManager<Shader, HShader> HShaderManager;

	/**
	*	Map associating handle with the name of the resource.
	*	As third template parameter, class for string comparison is passed.
	*/
	typedef std::map<std::string, HShader, istring_less> NameIndex;
	/* Typedef for inserting item into the map */
	typedef std::pair<typename NameIndex::iterator, bool> NameIndexInsertRc;
	

public:
	/* Map for associating logs with names of the shaders. */
	typedef std::map<std::string, const char*, istring_less> ShadersLogs;
	
	/**
	*	Default constructor doing nothing.
	*/
	ShaderManager();
	/**
	*	Destructor frees all earlier allocated resources.
	*/
	~ShaderManager();

	/**
	*	Get handle to the shader with given name. If it is opened for first time, resource is allocated, initialized and
	*	associated with the handle. If type will be different than the previous one, new one will override older one.
	*	However, recompilation will be needed.
	*
	*	@param name unique name of the shader.
	*	@param type type of the shader.
	*	@return Handle to the Shader resource.
	*/
	HShader GetShader(const std::string& name, Shader::ShaderType type);

	/**
	*	Delete given shader.
	*
	*	@param hshader handle to the shader.
	*/
	void DeleteShader(HShader hshader);
	/**
	*	Delete all shaders in manager.
	*/
	void DeleteAllShaders();

	/**
	*	Compile shader.
	*
	*	@param hshader handle to the shader.
	*	@return Error code, 0 if succeed.
	*/
	int CompileShader(HShader hshader);
	/**
	*	Compiles all shaders in manager.
	*
	*	@return True if no errors occured, false if any compilation failed.
	*/
	bool CompileAllShaders();
	/**
	*	Get compile log for given shader.
	*
	*	@param hshader hadnle to the shader.
	*	@return Cstring containing compilation log, NULL in case of an error.
	*/
	const char* GetCompileLog(HShader hshader);
	/**
	*	Get compile logs for all shaders in manager.
	*
	*	@return std::map with name of the shader and it's compile log. If log was empty, it won't be inserted into the map.
	*/
	ShadersLogs GetAllCompileLogs();

	/**
	*	Set source code for the shader from string.
	*
	*	@param hshader handle for the shader.
	*	@param source source code for the shader
	*/
	void SetSourceCode(HShader hshader, const std::string& source);
	/**
	*	Change type of the shader.
	*
	*	@param hshader handle for the shader.
	*	@param type new type of the shader.
	*/
	void SetType(HShader hshader, Shader::ShaderType type);
	/**
	*	Get type of the shader.
	*
	*	@param hshader handle for the shader.
	*	@return Type of the shader.
	*/
	int GetType(HShader hshader);

	/**
	*	Loads Shader data from file. Singleton ShaderFileManager must exist before calling this function.
	*
	*	@param hshader handle for the shader.
	*	@param fileName name of the file for reading.
	*	@return Error code, 0 if suceed.
	*/
	int LoadShaderDataFromFile(HShader hshader, const std::string& fileName);

	GLuint GetGlHandle(HShader hshader);

private:
	HShaderManager _shaders;	/* Manager for handles.					*/
	NameIndex _nameIndex;		/* Map associating names with handles.	*/
};

/* Define for easier access to manager, like global variable. */
#define shaderManager ShaderManager::GetSingleton()

}