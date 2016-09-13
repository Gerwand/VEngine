#include "GlBufferManager.h"

namespace vengine {

GlBufferManager::GlBufferManager()
{
}

GlBufferManager::~GlBufferManager()
{
	for (NameIndex::iterator i = _nameIndex.begin(); i != _nameIndex.end(); ++i) {
		_buffers.Release(i->second);
	}
}

GlBufferManager::HBuffer
GlBufferManager::GetBuffer(const std::string& name)
{
	NameIndexInsertRc rc = _nameIndex.insert(std::make_pair(name, HBuffer()));

	//If this element is new
	if (rc.second) {
		GlBuffer* buffer = _buffers.Acquire(rc.first->second);
		if (buffer->Init(name)) {
			DeleteBuffer(rc.first->second);
			return 0;
		}
	}

	return (rc.first->second);
}

void
GlBufferManager::DeleteBuffer(HBuffer hbuffer)
{
	GlBuffer* buffer = _buffers.GetItem(hbuffer);

	if (buffer != NULL) {
		_nameIndex.erase(_nameIndex.find(buffer->GetName()));

		_buffers.Release(hbuffer);
	}
	else {
		DEBUG_INFO("Wrong handle: %x, magic num: %d",
				   hbuffer.GetHandle(), hbuffer.GetMagicNumber());
	}
}

void
GlBufferManager::DeleteAllBuffers()
{
	for (NameIndex::iterator i = _nameIndex.begin(); i != _nameIndex.end(); ++i) {
		_buffers.Release(i->second);
	}
	_nameIndex.clear();
}



GLuint
GlBufferManager::GetGlHandle(HBuffer hbuffer)
{
	GlBuffer* buffer = _buffers.GetItem(hbuffer);

	if (buffer != NULL) {
		return *buffer;
	}
	else {
		DEBUG_INFO("Cannot get GL handle for buffer. Wrong handle: %x, magic num: %d",
				   hbuffer.GetHandle(), hbuffer.GetMagicNumber());
		return 0;
	}
}


int
GlBufferManager::BindBuffer(HBuffer hbuffer, GlBuffer::BindTarget target)
{
	GlBuffer* buffer = _buffers.GetItem(hbuffer);

	if (buffer != NULL) {
		buffer->Bind(target);
		return 0;
	}
	else {
		DEBUG_INFO("Cannot bind buffer. Wrong handle: %x, magic num: %d",
				   hbuffer.GetHandle(), hbuffer.GetMagicNumber());
		return VE_NORESOURCES;
	}
}

int
GlBufferManager::ReserveData(HBuffer hbuffer, const void* data, GLsizeiptr size, GLbitfield flags)
{
	GlBuffer* buffer = _buffers.GetItem(hbuffer);

	if (buffer != NULL) {
		buffer->ReserveData(data, size, flags);
		return 0;
	}
	else {
		DEBUG_INFO("Cannot reserve data in buffer. Wrong handle: %x, magic num: %d",
				   hbuffer.GetHandle(), hbuffer.GetMagicNumber());
		return VE_NORESOURCES;
	}
}

int 
GlBufferManager::ChangeData(HBuffer hbuffer, GLintptr start, const void* data, GLsizeiptr size)
{
	GlBuffer* buffer = _buffers.GetItem(hbuffer);

	if (buffer != NULL) {
		buffer->ChangeData(start, data, size);
		return 0;
	}
	else {
		DEBUG_INFO("Cannot change data in buffer. Wrong handle: %x, magic num: %d",
				   hbuffer.GetHandle(), hbuffer.GetMagicNumber());
		return VE_NORESOURCES;
	}
}

void*
GlBufferManager::MapBuffer(HBuffer hbuffer, GLintptr start, GLsizeiptr length, GLbitfield access)
{
	GlBuffer* buffer = _buffers.GetItem(hbuffer);

	if (buffer != NULL) {
		return buffer->Map(start, length, access);
	}
	else {
		DEBUG_INFO("Cannot mapa data from buffer. Wrong handle: %x, magic num: %d",
				   hbuffer.GetHandle(), hbuffer.GetMagicNumber());
		return NULL;
	}
}

int
GlBufferManager::FillBuffer(HBuffer hbuffer, GLintptr start, const void* data, GLsizeiptr size,
							GLenum dataType, GlBuffer::Channels channels)
{
	GlBuffer* buffer = _buffers.GetItem(hbuffer);

	if (buffer != NULL) {
		buffer->Fill(start, data, size, dataType, channels);
		return 0;
	}
	else {
		DEBUG_INFO("Cannot fill buffer. Wrong handle: %x, magic num: %d",
				   hbuffer.GetHandle(), hbuffer.GetMagicNumber());
		return NULL;
	}
}

int
GlBufferManager::CopyBuffers(HBuffer hsource, HBuffer hdestination,
							 GLintptr srcStart, GLintptr dstStart, GLsizeiptr size)
{
	GlBuffer* src = _buffers.GetItem(hsource);
	if (src == NULL) {
		DEBUG_INFO("Cannot copy buffer. Wrong source handle: %x, magic num: %d",
				   hsource.GetHandle(), hsource.GetMagicNumber());
		return VE_NORESOURCES;
	}

	GlBuffer* dst = _buffers.GetItem(hdestination);
	if (dst == NULL) {
		DEBUG_INFO("Cannot fill buffer. Wrong destination handle: %x, magic num: %d",
				   hdestination.GetHandle(), hdestination.GetMagicNumber());
		return VE_NORESOURCES;
	}

	GlBuffer::Copy(*src, *dst, srcStart, dstStart, size);
	return 0;
}

}