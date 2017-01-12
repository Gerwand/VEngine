#include "GlProgramManager.h"

namespace vengine {

GlProgramManager::GlProgramManager() : _active(0)
{
}

GlProgramManager::~GlProgramManager()
{
	for (NameIndex::iterator i = _nameIndex.begin(); i != _nameIndex.end(); ++i) {
		_programs.Release(i->second);
	}
}

GlProgramManager::HProgram
GlProgramManager::GetProgram(const std::string& name)
{
	NameIndexInsertRc rc = _nameIndex.insert(std::make_pair(name, HProgram()));

	//If this element is new
	if (rc.second) {
		GlProgram* program = _programs.Acquire(rc.first->second);
		if (program->Init(name, true)) {
			DeleteProgram(rc.first->second);
			rc.first->second = HProgram();
		}
	}

	return (rc.first->second);
}

void
GlProgramManager::DeleteProgram(HProgram hprogram)
{
	GlProgram* program = _programs.GetItem(hprogram);
	assert(program != nullptr, "Invalid handle.");

	_nameIndex.erase(program->GetName());
	_programs.Release(hprogram);

	if (_active == hprogram)
		_active = 0;
}

void
GlProgramManager::DeleteAllPrograms()
{
	for (NameIndex::iterator i = _nameIndex.begin(); i != _nameIndex.end(); ++i) {
		_programs.Release(i->second);
	}
	_nameIndex.clear();
	_active = 0;
}

int
GlProgramManager::LinkProgram(HProgram hprogram)
{
	GlProgram* program = _programs.GetItem(hprogram);
	assert(program != nullptr, "Invalid handle.");

	return program->Link();
}

bool
GlProgramManager::LinkAllPrograms()
{
	bool error = false;
	for (NameIndex::iterator i = _nameIndex.begin(); i != _nameIndex.end(); ++i) {
		HProgram hprogram = i->second;
		int rc = LinkProgram(hprogram);
		if (rc) {
			DEBUG_INFO("Failed to link program %s. Error: %d.", i->first.c_str(), rc);
			error |= true;
		}
	}

	return !error;
}

const char*
GlProgramManager::GetLinkLog(HProgram hprogram)
{
	GlProgram* program = _programs.GetItem(hprogram);
	assert(program != NULL, "Invalid handle.");

	return program->GetLinkLog();
}

GlProgramManager::GlProgramLogs
GlProgramManager::GetAllLinkLogs()
{
	GlProgramLogs logs;
	for (NameIndex::iterator i = _nameIndex.begin(); i != _nameIndex.end(); ++i) {
		HProgram hprogram = i->second;

		const char* log = GetLinkLog(hprogram);
		logs.insert(std::make_pair(i->first, log));
	}

	return logs;
}

void
GlProgramManager::AttachShader(HProgram hprogram, ShaderManager::HShader hshader)
{
	GlProgram* program = _programs.GetItem(hprogram);
	assert(program != nullptr, "Invalid program handle.");

	GLuint shaderHandle = shaderManager.GetGlHandle(hshader);

	program->AttachShader(shaderHandle);
}

GLuint
GlProgramManager::GetGlHandle(HProgram hprogram)
{
	GlProgram* program = _programs.GetItem(hprogram);
	assert(program != nullptr, "Invalid handle.");

	return *program;
}

void
GlProgramManager::SetProgramSeparable(HProgram hprogram, bool separable)
{
	GlProgram* program = _programs.GetItem(hprogram);
	assert(program != nullptr, "Invalid handle.");

	program->SetSeparable(separable);
}

int 
GlProgramManager::IsSeparable(HProgram hprogram)
{
	GlProgram* program = _programs.GetItem(hprogram);
	assert(program != nullptr, "Invalid handle.");

	return program->IsSeparable();
}

unsigned int 
GlProgramManager::GetActive()
{
	return _active;
}

void
GlProgramManager::BindProgram(HProgram hprogram)
{
	GlProgram* program = _programs.GetItem(hprogram);
	assert(program != nullptr, "Invalid handle.");

	if (_active != hprogram) {
		program->Bind();
		_active = 0;
	}
}

void
GlProgramManager::UnbindProgram()
{
	if (_active == 0)
		return;

	GlProgram* program = _programs.GetItem(_active);
	assert(program != nullptr, "Invalid handle");

	_active = 0;
}

void
GlProgramManager::SetUniform(HProgram hprogram, const std::string& name, const Matrix4& matrix)
{
	GlProgram* program = _programs.GetItem(hprogram);
	assert(program != nullptr, "Invalid handle.");

	program->SetUniform(name, matrix);
}

void
GlProgramManager::SetUniform(HProgram hprogram, const std::string& name, const Vector2& vector)
{
	GlProgram* program = _programs.GetItem(hprogram);
	assert(program != nullptr, "Invalid handle.");

	program->SetUniform(name, vector);
}

void
GlProgramManager::SetUniform(HProgram hprogram, const std::string& name, const Vector3& vector)
{
	GlProgram* program = _programs.GetItem(hprogram);
	assert(program != nullptr, "Invalid handle.");

	program->SetUniform(name,vector);
}

void
GlProgramManager::SetUniform(HProgram hprogram, const std::string& name, const Vector4& vector)
{
	GlProgram* program = _programs.GetItem(hprogram);
	assert(program != nullptr, "Invalid handle.");

	program->SetUniform(name, vector);
}

void
GlProgramManager::SetUniform(HProgram hprogram, const std::string& name, GLfloat value)
{
	GlProgram* program = _programs.GetItem(hprogram);
	assert(program != nullptr, "Invalid handle.");

	program->SetUniform(name, value);
}

void
GlProgramManager::SetUniform(HProgram hprogram, const std::string& name, GLint value)
{
	GlProgram* program = _programs.GetItem(hprogram);
	assert(program != nullptr, "Invalid handle.");

	program->SetUniform(name, value);
}

void
GlProgramManager::SetUniform(HProgram hprogram, const std::string& name, GLuint value)
{
	GlProgram* program = _programs.GetItem(hprogram);
	assert(program != nullptr, "Invalid handle.");

	program->SetUniform(name, value);
}



}