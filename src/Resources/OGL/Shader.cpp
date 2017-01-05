#include "Shader.h"

namespace vengine {

Shader::Shader() : _handle(0), _type(INVALID)
{

}

Shader::~Shader()
{
	Delete();
}

int 
Shader::Init(const std::string& name, ShaderType type)
{
	assert(!IsValid(), "Cannot init shader when handle is not empty.");
	_handle = glCreateShader(type);
	if (_handle == 0) {
		printf("Failed to create shader!\n");
		return VE_NOHANDLE;
	}
	_name = name;
	_type = type;

	return 0;
}

void 
Shader::Delete()
{
	if (IsValid()) {
		glDeleteShader(_handle);
		_handle = 0;
		_type = INVALID;
		_logLen = 0;
		_compileLog.clear();
	}
}

int 
Shader::Compile()
{
	assert(IsValid(), "Unknown shader type, cannot compile.");	
	
	glCompileShader(_handle);

	GLint compileStatus;
	glGetShaderiv(_handle, GL_INFO_LOG_LENGTH, &_logLen);
	glGetShaderiv(_handle, GL_COMPILE_STATUS, &compileStatus);

	int error = 0;
	if (compileStatus == GL_FALSE)
		error = VE_EVAL;

	_compileLog.resize(_logLen);

	glGetShaderInfoLog(_handle, _logLen, NULL, _compileLog.data());

	return error;
}

void
Shader::SetSource(const std::string& sourceCode)
{
	assert(IsValid(), "Invalid handle. Probably handle was not initialized.");
	
	_sourceCode = sourceCode;
	const GLchar *cSrc = _sourceCode.c_str();
	glShaderSource(_handle, 1, &cSrc, NULL);
}

void
Shader::SetType(ShaderType type)
{
	assert(type != INVALID, "Cannot change the type of the shader to INVALID.");

	if (type != _type) {
		Delete();
		Init(_name, type);
		SetSource(_sourceCode);
	}
}

const std::string&
Shader::GetName() const
{
	return _name;
}

GLuint 
Shader::GetGLHandle() const
{
	return _handle;
}

Shader::ShaderType
Shader::GetType() const
{
	return _type;
}

Shader::operator GLuint() const
{
	return _handle;
}

const char*
Shader::GetCompileLog() const
{
	return _compileLog.data();
}

bool
Shader::IsValid() const
{
	return _type != 0;
}

}