#include "GlBuffer.h"

namespace vengine {

GlBuffer::GlBuffer() : _handle(0), _bufferSize(0), _flags(0)
{

}


GlBuffer::~GlBuffer()
{
	Delete();
}

int
GlBuffer::Init()
{
	assert(!IsValid(), "Buffer handle is busy, cannot initialize earlier initialized buffer.");

	glCreateBuffers(1, &_handle);
	if (_handle == 0)
		return VE_NOHANDLE;

	return 0;
}


void
GlBuffer::Delete()
{
	if (IsValid()) {
		glDeleteBuffers(1, &_handle);
		_handle = 0;
		_flags = 0;
		_bufferSize = 0;
	}
}


void
GlBuffer::Bind(BindTarget target)
{
	assert(IsValid(), "Cannot bind unitialized buffer.");

	glBindBuffer(target, _handle);
}
void
GlBuffer::Unbind(BindTarget target)
{
	glBindBuffer(target, 0);
}
void
GlBuffer::ReserveMutableSizeData(const void* data, GLsizeiptr size, DataFlags usage)
{
	assert(IsValid(), "Cannot set data of unitialized buffer.");

	_bufferSize = size;
	glNamedBufferData(_handle, size, data, usage);
}


void 
GlBuffer::ReserveFixedSizeData(const void* data, GLsizeiptr size, GLbitfield flags)
{
	assert(IsValid(), "Cannot set data of unitialized buffer.");
	
	_bufferSize = size;
	glNamedBufferStorage(_handle, size, data, flags);
}

void 
GlBuffer::ChangeData(GLintptr start, const void* data, GLsizeiptr size)
{
	assert(IsValid(), "Cannot change buffer data, must be initalized first");
	assert(start + size < _bufferSize, "Size of the written data would exceed buffer size");
	assert((_flags & DYNAMIC) != 0, "Buffer is not dynamic. Cannot modify data");

	glNamedBufferSubData(_handle, start, size, data);

}

void*
GlBuffer::Map(GLintptr start, GLsizeiptr length, GLbitfield access)
{
	assert(IsValid(), "Cannot map buffer, must be initalized first");
	assert((access & READ) != 0 ? (_flags & READ) != 0 : true,
		   "Cannot map buffer for reading. It was not intialized as map read");
	assert((access & WRITE) != 0 ? (_flags & WRITE) != 0 : true,
		   "Cannot map buffer for writting. It was not intialized as map write");
	assert((access & PERSISTENT) != 0 ? (_flags & PERSISTENT) != 0 : true,
		   "Cannot map buffer as persisting. It was not intialized with proper flag.");
	assert((access & COHERENT) != 0 ? (_flags & COHERENT) != 0 : true,
		   "Cannot map buffer as coherent. It was not intialized with proper flag.");
	assert(start + length < _bufferSize, "Size of the mapped data would exceed buffer size");

	return glMapNamedBufferRange(_handle, start, length, access);
}

void 
GlBuffer::Fill(GLintptr start, const void* data, GLsizeiptr size, GLenum dataType, Channels channels)
{
	assert(IsValid(), "Cannot set buffer, must be initalized first");
	assert(start < _bufferSize, "Start byte exceeds maximum buffer size");

	glClearNamedBufferSubData(_handle, dataType, start, size, dataType, channels, data);
}

void
GlBuffer::Copy(const GlBuffer& source, const GlBuffer& destination,
			   GLintptr srcStart, GLintptr dstStart, GLsizeiptr size)
{
	assert(source.IsValid() && destination.IsValid(),
		   "Cannot copy buffers, must be initalized first");
	assert(srcStart + size < source.GetSize(), "Source data range exceeds buffer size");
	assert(dstStart + size < destination.GetSize(), "Destination data range exceeds buffer size");

	glCopyNamedBufferSubData(source, destination, srcStart, dstStart, size);
}


GLuint
GlBuffer::GetGLHandle() const
{
	return _handle;
}

GLsizeiptr
GlBuffer::GetSize() const
{
	return _bufferSize;
}
bool
GlBuffer::IsValid() const
{
	return _handle != 0;
}

GlBuffer::operator GLuint() const
{
	return _handle;
}
}