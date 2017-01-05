#pragma once
//#define GLEW_STATIC
//#include <GL/glew.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "Assert.h"


namespace vengine {

class Shader {
public:
	/**
	*	Types of shader to be used in program
	*/
	enum ShaderType {
		INVALID = 0U,							/* This special value describes error value.	*/
		VERTEX = GL_VERTEX_SHADER,				/* Vertex shader.								*/
		GEOMETRY = GL_GEOMETRY_SHADER,			/* Geometry shader.								*/
		FRAGMENT = GL_FRAGMENT_SHADER,			/* Fragment shader.								*/
		TESS_CONTROL = GL_TESS_CONTROL_SHADER,
		TESS_EVAL = GL_TESS_EVALUATION_SHADER,
	};

	/**
	*	Default constructor, initialized empty resource.
	*/
	Shader();
	/**
	*	If resource was initialized, delete it.
	*/
	~Shader();

	/**
	*	Inititialize shader resource. Must be called before using any of the operations.
	*
	*	@param name unique name for the Shader.
	*	@param type type of the shader, must be other than INVALID.
	*	@return Error code, 0 if succeed.
	*/
	int Init(const std::string& name, ShaderType type);
	/**
	*	Clear all shader resources  and marks shader type as invalid.
	*/
	void Delete();

	/**
	*	Compile shader. Object must be initialized before compilation.	
	*
	*	@return Error code, 0 if succeed.
	*/
	int Compile();

	/**
	*	Set source code for the shader. Must be done after initialization.
	*
	*	@param sourceCode source code for the shader in GLSL
	*/
	void SetSource(const std::string& sourceCode);
	/**
	*	Changes type of the shader. If shader was compiled before, it must be compiled again.
	*
	*	@param type new type of the shader.
	*/
	void SetType(ShaderType type);
	
	/**
	*	Get name of the shader.
	*
	*	@return Name of the shader.
	*/
	const std::string& GetName() const;
	/**
	*	Get OpenGL shader handle.
	*
	*	@return OpenGL handle for the shader.
	*/
	GLuint GetGLHandle() const;
	/**
	*	Get type of the shader.
	*
	*	@return Type of the shader.
	*/
	ShaderType GetType() const;
	/**
	*	Return compile log from last compilation.
	*
	*	@return Compile log from last compilation.
	*/
	const char* GetCompileLog() const;
	
	/**
	*	Check if shader was properly initialized.
	*
	*	@return True if shader was properly initialzied, false if not.
	*/
	bool IsValid() const;

	/**
	*	Converts shader to the GLuint OpenGL shader handle.
	*
	*	@return OpenGL handle for the shader.
	*/
	operator GLuint() const;

private:
	GLuint		_handle;			/* OpenGL handle for the shader.	*/
	std::string _name;				/* Unique name of the shader.		*/
	std::string _sourceCode;		/* Source code of the shader.		*/
	ShaderType	_type;				/* Type of the shader.				*/
	GLsizei		_logLen;			/* Length of the compile log.		*/	
	std::vector<char> _compileLog;	/* Compile log.						*/
};


}