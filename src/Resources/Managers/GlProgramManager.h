#pragma once

#include "Resources/Handle.h"
#include "HandleManager.h"
#include "Singleton.h"
#include "Resources/OGL/GlProgram.h"
#include "ShaderManager.h"

#include <string>
#include <map>

namespace vengine {

/**
*	Class representating GL Programs manager for managing GlPrograms.
*	It inheritances from class Singleton, so only one object of this class can exist at any time.
*/
class GlProgramManager : public Singleton<GlProgramManager>
{
public:
	/* Tag type for GlProgram handle. */
	struct ProgramTag {};
	/* Handle for GlProgram. */
	typedef Handle<ProgramTag> HProgram;
private:
	/* HandleManager for GlProgram. */
	typedef HandleManager<GlProgram, HProgram> HProgramManager;

	/**
	*	Map associating handle with the name of the resource.
	*	As third template parameter, class for string comparison is passed.
	*/
	typedef std::map<std::string, HProgram, istring_less> NameIndex;
	/* Typedef for inserting item into the map */
	typedef std::pair<typename NameIndex::iterator, bool> NameIndexInsertRc;


public:
	/* Map for associating logs with names of the GlPrograms. */
	typedef std::map<std::string, const char*, istring_less> GlProgramLogs;

	/**
	*	Default constructor doing nothing.
	*/
	GlProgramManager();
	/**
	*	Destructor frees all earlier allocated resources.
	*/
	~GlProgramManager();

	/**
	*	Get handle to the GlProgram with given name. If it is opened for first time, resource is allocated, initialized and
	*	associated with the handle. Default program is separable.
	*
	*	@param name unique name of the GlProgram.
	*	@return Handle to the GlProgram resource.
	*/
	HProgram GetProgram(const std::string& name);

	/**
	*	Delete given GlProgram.
	*
	*	@param hprogram handle to the GlProgram.
	*/
	void DeleteProgram(HProgram hprogram);
	/**
	*	Delete all GlPrograms in manager.
	*/
	void DeleteAllPrograms();

	/* Set uniforms of the program */
	void SetUniform(HProgram hprogram, const std::string& name, const Matrix4& matrix);
	void SetUniform(HProgram hprogram, const std::string& name, const Vector2& vector);
	void SetUniform(HProgram hprogram, const std::string& name, const Vector3& vector);
	void SetUniform(HProgram hprogram, const std::string& name, const Vector4& vector);
	void SetUniform(HProgram hprogram, const std::string& name, GLfloat value);
	void SetUniform(HProgram hprogram, const std::string& name, GLint value);
	void SetUniform(HProgram hprogram, const std::string& name, GLuint value);

	/**
	*	Attach given shader to the program. Use it before linking.
	*
	*	@param hprogram handle to the GlProgram.
	*	@param hshader handle to the Shader.
	*/
	void AttachShader(HProgram hprogram, ShaderManager::HShader hshader);

	/**
	*	Link program.
	*
	*	@param hprogram handle to the GlProgram.
	*	@return Error code, 0 if succeed.
	*/
	int LinkProgram(HProgram hprogram);
	/**
	*	Links all GlPrograms in manager.
	*
	*	@return True if no errors occured, false if any linking failed.
	*/
	bool LinkAllPrograms();
	/**
	*	Get linking log for given shader.
	*
	*	@param hprogram handle to the GlProgram.
	*	@return Cstring containing link log, NULL in case of an error.
	*/
	const char* GetLinkLog(HProgram hprogram);
	/**
	*	Get compile logs for all shaders in manager.
	*
	*	@return std::map with name of the program and it's link log. If log was empty, it won't be inserted into the map.
	*/
	GlProgramLogs GetAllLinkLogs();

	/**
	*	Returns gl handle of the GlProgram.
	*
	*	@param hprogram handle to the GlProgram.
	*	@return GL handle, 0 in case of empty handle/error.
	*/
	GLuint GetGlHandle(HProgram hprogram);

	void BindProgram(HProgram hprogram);
	void UnbindProgram();

	/**
	*	Set separability of the program. Must link again after changing
	*
	*	@param hprogram hadnle to the GlProgram.
	*	@param separable true if program should be separable, false if not.
	*	@return Error code, 0 if succeed.
	*/
	void SetProgramSeparable(HProgram hprogram, bool separable);
	/**
	*	Checks if program is separable.
	*
	*	@param hprogram hadnle to the GlProgram.
	*	@return 0 if not, 1 if it is separable, other value in case of an error.
	*/
	int IsSeparable(HProgram hprogram);

	unsigned int GetActive();
private:
	HProgramManager _programs;	/* Manager for handles.					*/
	NameIndex _nameIndex;		/* Map associating names with handles.	*/
	HProgram _active;
};

/* Define for easier access to manager, like global variable. */
#define programManager GlProgramManager::GetSingleton()

}