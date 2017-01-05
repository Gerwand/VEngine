#pragma once

#include "Assert.h"
#include <glad/glad.h>

#include <SOIL2/SOIL2.h>
#include <string>

namespace vengine {

class Texture
{
public:
	Texture();
	~Texture();

	int Init(const std::string& name);
	void Delete();

	int LoadTexture(const std::string& path);

	void Bind(int unit = 0);
	static void Unbind(int unit = 0);

	bool IsValid();

	GLuint GetGLHandle() const;
	const std::string& GetName() const;
	operator GLuint() const;
private:
	GLuint _handle;
	std::string _name;
};

}