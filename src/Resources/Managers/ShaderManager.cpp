#include "ShaderManager.h"

namespace vengine {

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
	for (NameIndex::iterator i = _nameIndex.begin(); i != _nameIndex.end(); ++i) {
		_shaders.Release(i->second);
	}
}

ShaderManager::HShader
ShaderManager::GetShader(const std::string& name, Shader::ShaderType type)
{
	NameIndexInsertRc rc = _nameIndex.insert(std::make_pair(name, HShader()));

	//If this element is new
	if (rc.second) {
		Shader* shader = _shaders.Acquire(rc.first->second);
		//if (shader->Init(name, type)) {
		if (shader->Init(name, type)) {
			DeleteShader(rc.first->second);
			rc.first->second = HShader();
		}
	}
	//If existed, and the type is other than stored in shader
	else {
		Shader* shader = _shaders.GetItem(rc.first->second);
		if (type != shader->GetType()) {
			DEBUG_INFO("Changing %s shader type. Previous was: %d, new one is: %d.",
					   name.c_str(), shader->GetType(), type);
			shader->SetType(type);
		}
	}

	return (rc.first->second);
}

void
ShaderManager::DeleteShader(HShader hshader)
{
	Shader* shader = _shaders.GetItem(hshader);
	assert(shader != nullptr, "Invalid handle.");

	_nameIndex.erase(shader->GetName());
	_shaders.Release(hshader);
}

void
ShaderManager::DeleteAllShaders()
{
	for (NameIndex::iterator i = _nameIndex.begin(); i != _nameIndex.end(); ++i) {
		_shaders.Release(i->second);
	}
	_nameIndex.clear();
}

int
ShaderManager::CompileShader(HShader hshader)
{
	Shader* shader = _shaders.GetItem(hshader);
	assert(shader != nullptr, "Invalid handle.");

	return shader->Compile();
}

bool
ShaderManager::CompileAllShaders()
{
	bool error = false;
	for (NameIndex::iterator i = _nameIndex.begin(); i != _nameIndex.end(); ++i) {
		HShader hshader = i->second;
		int rc = CompileShader(hshader);
		if (rc) {
			DEBUG_INFO("Failed to compile shader %s. Error: %d.", i->first.c_str(), rc);
			error |= true;
		}
	}

	return !error;
}

const char*
ShaderManager::GetCompileLog(HShader hshader)
{
	Shader* shader = _shaders.GetItem(hshader);
	assert(shader != nullptr, "Invalid handle.");

	return shader->GetCompileLog();
}

ShaderManager::ShadersLogs
ShaderManager::GetAllCompileLogs()
{
	ShadersLogs logs;
	for (NameIndex::iterator i = _nameIndex.begin(); i != _nameIndex.end(); ++i) {
		HShader hshader = i->second;

		const char* log = GetCompileLog(hshader);
		logs.insert(std::make_pair(i->first, log));
	}

	return logs;
}

void
ShaderManager::SetSourceCode(HShader hshader, const std::string& source)
{
	Shader* shader = _shaders.GetItem(hshader);
	assert(shader != nullptr, "Invalid handle.");

	shader->SetSource(source);
}

void
ShaderManager::SetType(HShader hshader, Shader::ShaderType type)
{
	Shader* shader = _shaders.GetItem(hshader);
	assert(shader != nullptr, "Invalid handle.");

	shader->SetType(type);
}


int
ShaderManager::GetType(HShader hshader)
{

	Shader* shader = _shaders.GetItem(hshader);
	assert(shader != nullptr, "Invalid handle.");

	return shader->GetType();
}

int
ShaderManager::LoadShaderDataFromFile(HShader hshader, const std::string& fileName)
{
	int error = 0;
	char *buff = nullptr;
	ShaderFileManager::HFile hfile = shaderFileManager.GetFileRead(fileName);
	
	error = shaderFileManager.ReadData(hfile, (void **)&buff);
	if (error) {
		DEBUG_INFO("Error %d when reading data from file %s.", error, fileName.c_str());
		return error;
	}

	Shader* shader = _shaders.GetItem(hshader);
	assert(shader != nullptr, "Invalid handle.");
	shader->SetSource(buff);

	delete[] buff;
	return error;
}

GLuint
ShaderManager::GetGlHandle(HShader hshader)
{
	Shader* shader = _shaders.GetItem(hshader);
	assert(shader != nullptr, "Invalid handle.");

	return *shader;
}


}