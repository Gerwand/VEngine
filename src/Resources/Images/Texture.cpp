#include "Texture.h"

namespace vengine {


Texture::Texture() : _handle(0)
{

}
Texture::~Texture()
{
	glDeleteTextures(1, &_handle);
}

int
Texture::Init(const std::string& name)
{
	assert(!IsValid(), "Cannot initialize already initialized texture");

	glGenTextures(1, &_handle);
	if (_handle == 0) {
		printf("Failed to generate texture!\n");
		return VE_NOHANDLE;
	}
	_name = name;

	return 0;
}
void
Texture::Delete() 
{
	assert(IsValid(), "Cannot delete unitiliazed texture.");

	glDeleteTextures(1, &_handle);
	_handle = 0;
}

int
Texture::LoadTexture(const std::string& path)
{
	assert(IsValid(), "Cannot load texture for unitialized handle");
	unsigned int flags = SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MIPMAPS |
		SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_TEXTURE_REPEATS;
	int rc = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, _handle, flags);
	if (rc = 0)
		return VE_EIO;

	return 0;
}

void
Texture::Bind(int unit)
{
	assert(unit < 32, "Texture unit number is higher than 32!");
	unsigned int texID = GL_TEXTURE0 + unit;

	glActiveTexture(texID);
	glBindTexture(GL_TEXTURE_2D, _handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void
Texture::Unbind(int unit)
{
	assert(unit < 32, "Texture unit number is higher than 32!");
	unsigned int texID = GL_TEXTURE0 + unit;

	glActiveTexture(texID);
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool 
Texture::IsValid()
{
	return _handle != 0;
}

GLuint 
Texture::GetGLHandle() const
{
	return _handle;
}

const std::string& 
Texture::GetName() const
{
	return _name;
}

Texture::operator GLuint() const
{
	return _handle;
}

}