#pragma once

//#define GLEW_STATIC
//#include <GL/glew.h>
#include "GlProgram.h"
#include "Errors.h"
#include "Assert.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <map>



namespace vengine {

class GlPipeline
{
public:
	GlPipeline();
	~GlPipeline();

	int Init(const std::string& name);
	void Delete();

	void Bind();

	GLuint GetGLHandle() const;
	void ChangeStage(GLuint programHandle, GLbitfield stages);
	const std::string& GetName() const;

	bool IsValid() const;
	operator GLuint() const;

	GLuint GetProgram(GlProgram::Stage stage)
	{
		int i;
		for (i = 0; i < numStages; ++i)
			if (1 << i & stage)
				break;

		return _programs[i];
	}

private:

	static const int numStages = 5;

	GLuint _handle;
	std::string _name;

	GLuint _programs[numStages];


};

}