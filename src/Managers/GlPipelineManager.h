#pragma once

#include "Handle.h"
#include "HandleManager.h"
#include "Singleton.h"
#include "GlPipeline.h"
#include "GlProgramManager.h"

#include <string>
#include <map>

namespace vengine {

/**
*	Class representating GL Programs manager for managing GlPrograms.
*	It inheritances from class Singleton, so only one object of this class can exist at any time.
*	It it highly advisable to create instance of singleton ShaderManager before using this class.
*/
class GlPipelineManager : public Singleton<GlPipelineManager>
{
public:
	/* Tag type for GlPipeline handle. */
	struct PipelineTag {};
	/* Handle for GlPipeline. */
	typedef Handle<PipelineTag> HPipeline;
private:
	/* HandleManager for GlPipeline. */
	typedef HandleManager<GlPipeline, HPipeline> HPipelineManager;

	/**
	*	Map associating handle with the name of the resource.
	*	As third template parameter, class for string comparison is passed.
	*/
	typedef std::map<std::string, HPipeline, istring_less> NameIndex;
	/* Typedef for inserting item into the map */
	typedef std::pair<typename NameIndex::iterator, bool> NameIndexInsertRc;

public:
	/**
	*	Default constructor doing nothing.
	*/
	GlPipelineManager();
	/**
	*	Destructor frees all earlier allocated resources.
	*/
	~GlPipelineManager();

	/**
	*	Get handle to the GlPipeline with given name. If it is opened for first time, resource is allocated, initialized and
	*	associated with the handle. Default program is separable.
	*
	*	@param name unique name of the GlPipeline.
	*	@return Handle to the GlPipeline resource.
	*/
	HPipeline GetPipeline(const std::string& name);

	/**
	*	Delete given GlPipeline.
	*
	*	@param hpipeline handle to the GlPipeline.
	*/
	void DeletePipeline(HPipeline hpipeline);

	/**
	*	Delete all GlPipelines in manager.
	*/
	void DeleteAllPipelines();

	/**
	*	Bind given pipeline as active to the current active graphical pipeline.
	*
	*	@param hpipeline handle to the GlPipeline.
	*/
	void BindPipeline(HPipeline hpipeline);

	/**
	*	Change one or more stages of the pipeline from the given program.
	*
	*	@param hpipeline handle to the GlPipeline.
	*	@param program handle to the GlProgram from which stages will be used.
	*	@param stages one or more values from GlProgram::stages enum type determining stages for changing.
	*/
	void ChangeStage(HPipeline hpipeline, GlProgramManager::HProgram hprogram, GLbitfield stages);

	/**
	*	Returns gl handle of the GlPipeline.
	*
	*	@param hpipeline handle to the GlPipeline.
	*	@return GL handle, 0 in case of empty handle/error.
	*/
	GLuint GetGlHandle(HPipeline hpipeline);


	GLuint GetProgram(HPipeline hpipeline, GlProgram::Stage stage);

private:
	HPipelineManager _pipelines;	/* Manager for handles.					*/
	NameIndex _nameIndex;			/* Map associating names with handles.	*/
	HPipeline _active;
};

/* Define for easier access to manager, like global variable. */
#define pipelineManager GlPipelineManager::GetSingleton()

}