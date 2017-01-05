#include "GlPipeline.h"


namespace vengine {

GlPipeline::GlPipeline() : _handle(0)
{
	for (int i = 0; i < 5; ++i)
		_programs[i] = 0;
}

GlPipeline::~GlPipeline()
{
	Delete();
}

int 
GlPipeline::Init(const std::string& name)
{
	assert(!IsValid(), "Pipeline handle is busy, cannot initialize earlier initialized pipeline.");

	glCreateProgramPipelines(1, &_handle);
	if (_handle == 0)
		return VE_NOHANDLE;
	_name = name;

	return 0;
}
void 
GlPipeline::Delete()
{
	assert(IsValid(), "Cannot delete uninitialized pipeline");
	glDeleteProgramPipelines(1, &_handle);
	_handle = 0;
	for (int i = 0; i < 5; ++i)
		_programs[i] = 0;
}

void 
GlPipeline::Bind()
{
	assert(IsValid(), "Cannot bind unitialized pipeline.");
	glBindProgramPipeline(_handle);
}

GLuint 
GlPipeline::GetGLHandle() const
{
	return _handle;
}

void 
GlPipeline::ChangeStage(GLuint programHandle, GLbitfield stages)
{
	assert(IsValid(), "Cannot change stage for unitialized pipeline.");
	glUseProgramStages(_handle, stages, programHandle);

	for (int i = 0; i < numStages; ++i) {
		if ((1 << i) & stages)
			_programs[i] = programHandle;
	}
}

const std::string& 
GlPipeline::GetName() const
{
	return _name;
}

bool
GlPipeline::IsValid() const
{
	return _handle != 0;
}

GlPipeline::operator GLuint() const
{
	return _handle;
}



}