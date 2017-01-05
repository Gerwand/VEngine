#include "VertexArray.h"

namespace vengine {

VertexArray::VertexArray() : _handle(0)
{

}
VertexArray::~VertexArray()
{
	if (IsValid())
		glDeleteVertexArrays(1, &_handle);
}

void 
VertexArray::Init()
{
	assert(!IsValid(), "Cannot initialize already initialized VAO");

	glCreateVertexArrays(1, &_handle);
}
void
VertexArray::Delete()
{
	assert(IsValid(), "Cannot destroy not initialized VAO");

	glCreateVertexArrays(1, &_handle);
	_handle = 0;
}

void
VertexArray::Bind()
{
	glBindVertexArray(_handle);
	_binded = true;
}
void
VertexArray::Unbind()
{
	_binded = false;
	glBindVertexArray(0);
}
void
VertexArray::ActivateBinded(GLuint attribute, GLuint numParams, GLenum type, GLsizei size, GLvoid* start)
{
	assert(IsBinded(), "Activating parameter for unbinded VAO!");
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, numParams, GL_FLOAT, GL_FALSE, size,
		start);
}

GLuint 
VertexArray::GetGLHandle() const
{
	return _handle;
}

bool
VertexArray::IsValid() const
{
	return _handle != 0;
}
bool
VertexArray::IsBinded() const
{
	return _binded;
}

VertexArray::operator GLuint() const
{
	return _handle;
}
}