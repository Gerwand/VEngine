#include "GlProgram.h"

namespace vengine {

GlProgram::GlProgram() : _handle(0), _logLen(0)
{
}

GlProgram::~GlProgram()
{
	Delete();
}

int
GlProgram::Init(const std::string& name, bool isSeparable)
{
	assert(!IsValid(), "Program handle is busy, cannot initialize earlier initialized program.");

	_handle = glCreateProgram();
	if (_handle == 0)
		return VE_NOHANDLE;
	_isSeparable = isSeparable ? GL_TRUE : GL_FALSE;
	glProgramParameteri(_handle, GL_PROGRAM_SEPARABLE, _isSeparable);
	_name = name;

	return 0;
}

void
GlProgram::Delete()
{
	if (IsValid()) {
		glDeleteProgram(_handle);
		_uniforms.clear();
		_handle = 0;
		_logLen = 0;
		_linkLog.clear();
	}
}

void
GlProgram::AttachShader(GLuint shader)
{
	assert(IsValid(), "Wrong program handle, cannot attach shader to it.");
	glAttachShader(_handle, shader);
}

int 
GlProgram::Link()
{
	assert(IsValid(), "Wrong program handle, cannot perform linking.");

	glLinkProgram(_handle);

	GLint linkStatus;

	glGetProgramiv(_handle, GL_INFO_LOG_LENGTH, &_logLen);
	glGetProgramiv(_handle, GL_LINK_STATUS, &linkStatus);

	int error = 0;
	if (linkStatus == GL_FALSE)
		error = VE_EVAL;

	_linkLog.resize(_logLen);

	glGetProgramInfoLog(_handle, _logLen, NULL, _linkLog.data());

	return error;

}

void
GlProgram::Bind()
{
	glUseProgram(_handle);
}

const char*
GlProgram::GetLinkLog() const
{
	return _linkLog.data();
}

const std::string&
GlProgram::GetName() const
{
	return _name;
}
bool 
GlProgram::IsValid() const
{
	return _handle != 0;
}

GlProgram::operator GLuint() const
{
	return _handle;
}

GLuint
GlProgram::GetGLHandle() const
{
	return _handle;
}

bool
GlProgram::IsSeparable() const
{
	return _isSeparable == GL_TRUE;
}

void 
GlProgram::SetSeparable(bool isSeparable)
{
	_isSeparable = isSeparable ? GL_TRUE : GL_FALSE;
	glProgramParameteri(_handle, GL_PROGRAM_SEPARABLE, _isSeparable);
}


void
GlProgram::SetUniform(const std::string& name, GlProgram::Stage stage, const Matrix4& matrix)
{
	assert(IsValid(), "Cannot set matrix for unitiliazed uniform");
	GLint location = GetUniformLocation(name, stage);

	glProgramUniformMatrix4fv(_handle, location, 1, GL_FALSE, matrix);
}

void
GlProgram::SetUniform(const std::string& name, GlProgram::Stage stage, const Vector2& vector)
{
	assert(IsValid(), "Cannot set matrix for unitiliazed uniform");
	GLint location = GetUniformLocation(name, stage);

	glProgramUniform2fv(_handle, location, 1, vector);
}

void
GlProgram::SetUniform(const std::string& name, GlProgram::Stage stage, const Vector3& vector)
{
	assert(IsValid(), "Cannot set matrix for unitiliazed uniform");
	GLint location = GetUniformLocation(name, stage);

	glProgramUniform3fv(_handle, location, 1, vector);
}

void
GlProgram::SetUniform(const std::string& name, GlProgram::Stage stage, const Vector4& vector)
{
	assert(IsValid(), "Cannot set matrix for unitiliazed uniform");
	GLint location = GetUniformLocation(name, stage);

	glProgramUniform4fv(_handle, location, 1, vector);
}

void
GlProgram::SetUniform(const std::string& name, GlProgram::Stage stage, GLfloat value)
{
	assert(IsValid(), "Cannot set matrix for unitiliazed uniform");
	GLint location = GetUniformLocation(name, stage);

	glProgramUniform1f(_handle, location, value);
}

void
GlProgram::SetUniform(const std::string& name, GlProgram::Stage stage, GLint value)
{
	assert(IsValid(), "Cannot set matrix for unitiliazed uniform");
	GLint location = GetUniformLocation(name, stage);

	glProgramUniform1i(_handle, location, value);
}

void
GlProgram::SetUniform(const std::string& name, GlProgram::Stage stage, GLuint value)
{
	assert(IsValid(), "Cannot set matrix for unitiliazed uniform");
	GLint location = GetUniformLocation(name, stage);

	glProgramUniform1ui(_handle, location, value);
}

GLint
GlProgram::GetUniformLocation(const std::string& name, Stage stage)
{
	//UniformsRC rc = _uniforms.insert(std::make_pair(name, 0));

	//If this element is new
	//if (rc.second) {
		GLint loc = glGetUniformLocation(_handle, name.c_str());
		return loc;
	//	assert(loc >= 0, "Unknown uniform name %s", name.c_str());
		//rc.first->second = loc;
	//}

	//return (rc.first->second);
}
}