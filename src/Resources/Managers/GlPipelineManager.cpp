#include "GlPipelineManager.h"

namespace vengine {

GlPipelineManager::GlPipelineManager()
{
}

GlPipelineManager::~GlPipelineManager()
{
	for (NameIndex::iterator i = _nameIndex.begin(); i != _nameIndex.end(); ++i) {
		_pipelines.Release(i->second);
	}
}

GlPipelineManager::HPipeline
GlPipelineManager::GetPipeline(const std::string& name)
{
	NameIndexInsertRc rc = _nameIndex.insert(std::make_pair(name, HPipeline()));

	//If this element is new
	if (rc.second) {
		GlPipeline* pipeline = _pipelines.Acquire(rc.first->second);
		if (pipeline->Init(name)) {
			DeletePipeline (rc.first->second);
			rc.first->second = HPipeline();
		}
	}

	return (rc.first->second);
}

void
GlPipelineManager::DeletePipeline(HPipeline hpipeline)
{
	GlPipeline* pipeline = _pipelines.GetItem(hpipeline);
	assert(pipeline != nullptr, "Invalid handle.");

	_nameIndex.erase(pipeline->GetName());
	_pipelines.Release(hpipeline);
}

void
GlPipelineManager::DeleteAllPipelines()
{
	for (NameIndex::iterator i = _nameIndex.begin(); i != _nameIndex.end(); ++i) {
		_pipelines.Release(i->second);
	}
	_nameIndex.clear();
}



GLuint
GlPipelineManager::GetGlHandle(HPipeline hpipeline)
{
	GlPipeline* pipeline = _pipelines.GetItem(hpipeline);
	assert(pipeline != nullptr, "Invalid handle.");

	return *pipeline;
}


void
GlPipelineManager::BindPipeline(HPipeline hpipeline)
{
	GlPipeline* pipeline = _pipelines.GetItem(hpipeline);
	assert(pipeline != nullptr, "Invalid handle.");

	if (_active != hpipeline || programManager.GetActive() != 0) {
		programManager.UnbindProgram();
		_active = hpipeline;
		pipeline->Bind();
	}
}

void
GlPipelineManager::ChangeStage(HPipeline hpipeline, GlProgramManager::HProgram hprogram, GLbitfield stages)
{
	GlPipeline* pipeline = _pipelines.GetItem(hpipeline);
	assert(pipeline != nullptr, "Invalid pipeline handle.");

	GLuint programHandle = programManager.GetGlHandle(hprogram);

	pipeline->ChangeStage(programHandle, stages);
}

GLuint
GlPipelineManager::GetProgram(HPipeline hpipeline, GlProgram::Stage stage)
{
	GlPipeline* pipeline = _pipelines.GetItem(hpipeline);
	assert(pipeline != 0, "Invalid handle.");

	return pipeline->GetProgram(stage);
}
}