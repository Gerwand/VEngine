#pragma once

#include "Errors.h"
#include "Assert.h"
#include "VEMath.h"
//#define GLEW_STATIC
//#include <GL/glew.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <string>
#include <vector>
#include <map>
#include "Strcmp.h"

namespace vengine {

class GlProgram 
{
public:
	enum Stage {
		VERTEX = GL_VERTEX_SHADER_BIT,
		FRAGMENT = GL_FRAGMENT_SHADER_BIT,
		GEOMETRY = GL_GEOMETRY_SHADER_BIT,
		TESS_CONTROL = GL_TESS_CONTROL_SHADER_BIT,
		TESS_EVAL = GL_TESS_EVALUATION_SHADER_BIT,
		ALL = GL_ALL_SHADER_BITS
	};

	GlProgram();
	~GlProgram();

	int Init(const std::string& name, bool isSeparable);
	void Delete();

	void AttachShader(GLuint shader);
	void SetSeparable(bool isSeparable);

	void SetUniform(const std::string& name, GlProgram::Stage stage, const Matrix4& matrix);
	void SetUniform(const std::string& name, GlProgram::Stage stage, const Vector2& value);
	void SetUniform(const std::string& name, GlProgram::Stage stage, const Vector3& value);
	void SetUniform(const std::string& name, GlProgram::Stage stage, const Vector4& value);
	void SetUniform(const std::string& name, GlProgram::Stage stage, GLfloat value);
	void SetUniform(const std::string& name, GlProgram::Stage stage, GLint value);
	void SetUniform(const std::string& name, GlProgram::Stage stage, GLuint value);

	void Bind();

	int Link();
	const char* GetLinkLog() const;

	GLuint GetGLHandle() const;
	const std::string& GetName() const;
	bool IsValid() const;
	bool IsSeparable() const;

	operator GLuint() const;

private:
	typedef std::map<std::string, GLint, istring_less> Uniforms;
	typedef std::pair<typename Uniforms::iterator, bool> UniformsRC;

	GLuint _handle; 
	GLint _isSeparable;
	std::string _name;
	GLsizei		_logLen;			/* Length of the linking log.		*/
	std::vector<char> _linkLog;		/* Linking log.						*/
	Uniforms _uniforms;

	GLint GetUniformLocation(const std::string& name, GlProgram::Stage stage);
};

}