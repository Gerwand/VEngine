#pragma once

#include "Assert.h"
#include <glad/glad.h>

#include <SOIL2/SOIL2.h>
#include <string>

namespace vengine {

/* Class representing texture */
class Texture
{
public:
	Texture();
	~Texture();

	/* Initializes texture resource */
	int Init(const std::string& name);
	/* Delete texture resource */
	void Delete();

	/* Load texture from the file */
	int LoadTexture(const std::string& path);

	/* Bind texture to given sampler */
	void Bind(int unit = 0);
	/* Unbind texture from given sampler */
	static void Unbind(int unit = 0);

	/* Check if resource is valid */
	bool IsValid();
	
	/* Get name of the resource */
	const std::string& GetName() const;

	/* Get OpenGL texture handle */
	GLuint GetGLHandle() const;
	/* Convert object to it's OpenGL handle */
	operator GLuint() const;
private:
	GLuint _handle;		/* OpenGL handle */
	std::string _name;
};

}