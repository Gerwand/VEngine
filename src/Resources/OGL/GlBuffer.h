#pragma once

//#define GLEW_STATIC
//#include <GL/glew.h>

#include "Errors.h"
#include "Assert.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

namespace vengine {

class GlBuffer
{
public:
	enum MapFlags {
		DYNAMIC			= GL_DYNAMIC_STORAGE_BIT,
		READ			= GL_MAP_READ_BIT,
		WRITE			= GL_MAP_WRITE_BIT,
		PERSISTENT		= GL_MAP_PERSISTENT_BIT,
		COHERENT		= GL_MAP_COHERENT_BIT,
		STORAGE_CPU		= GL_CLIENT_STORAGE_BIT,
		INVALIDATE_RANGE = GL_MAP_INVALIDATE_RANGE_BIT,
		INVALIDATE_BUFFER = GL_MAP_INVALIDATE_BUFFER_BIT,
		FLUSH_EXPLICIT = GL_MAP_FLUSH_EXPLICIT_BIT,
		UNSYCHRONIZED = GL_MAP_UNSYNCHRONIZED_BIT
	};

	enum DataFlags {
		STATIC_DRAW = GL_STATIC_DRAW,
		DYNAMIC_DRAW = GL_DYNAMIC_DRAW
	};

	enum Channels {
		ONE = GL_RED,
		TWO = GL_RG,
		THREE = GL_RGB,
		FOUR = GL_RGBA
	};

	enum BindTarget {
		VERTEX = GL_ARRAY_BUFFER,
		INDICES = GL_ELEMENT_ARRAY_BUFFER
	};

	GlBuffer();
	~GlBuffer();

	int Init();
	void Delete();

	void Bind(BindTarget target);
	void Unbind(BindTarget target);
	void ReserveMutableSizeData(const void* data, GLsizeiptr size, DataFlags usage);
	void ReserveFixedSizeData(const void* data, GLsizeiptr size, GLbitfield flags);
	void ChangeData(GLintptr start, const void* data, GLsizeiptr size);
	void* Map(GLintptr start, GLsizeiptr length, GLbitfield access);
	void Fill(GLintptr start, const void* data, GLsizeiptr size, GLenum dataType, Channels channels);
	static void Copy(const GlBuffer& source, const GlBuffer& destination,
					 GLintptr srcStart, GLintptr dstStart, GLsizeiptr size);


	GLuint GetGLHandle() const;
	GLsizeiptr GetSize() const;
	bool IsValid() const;

	operator GLuint() const;

private:
	GLuint _handle;
	GLsizeiptr _bufferSize;
	GLbitfield _flags;
};

}