#pragma once

#include "Handle.h"
#include "HandleManager.h"
#include "Singleton.h"
#include "GlBuffer.h"

#include <string>
#include <map>

namespace vengine {

/**
*	Class representating GL Buffers manager for managing GlBuffers.
*	It inheritances from class Singleton, so only one object of this class can exist at any time.
*	It it highly advisable to create instance of singleton ShaderManager before using this class.
*/
class GlBufferManager : public Singleton<GlBufferManager>
{
public:
	/* Tag type for GlBuffer handle. */
	struct BufferTag {};
	/* Handle for GlBuffer. */
	typedef Handle<BufferTag> HBuffer;
private:
	/* HandleManager for GlBuffer. */
	typedef HandleManager<GlBuffer, HBuffer> HBufferManager;

	/**
	*	Map associating handle with the name of the resource.
	*	As third template parameter, class for string comparison is passed.
	*/
	typedef std::map<std::string, HBuffer, istring_less> NameIndex;
	/* Typedef for inserting item into the map */
	typedef std::pair<typename NameIndex::iterator, bool> NameIndexInsertRc;

public:
	/**
	*	Default constructor doing nothing.
	*/
	GlBufferManager();
	/**
	*	Destructor frees all earlier allocated resources.
	*/
	~GlBufferManager();

	/**
	*	Get handle to the GlPipeline with given name. If it is opened for first time, resource is allocated, initialized and
	*	associated with the handle. Default program is separable.
	*
	*	@param name unique name of the GlBuffer.
	*	@return Handle to the GlBuffer resource.
	*/
	HBuffer GetBuffer(const std::string& name);

	/**
	*	Delete given GlBuffer.
	*
	*	@param hbuffer handle to the GlBuffer.
	*/
	void DeleteBuffer(HBuffer hbuffer);
	/**
	*	Delete all GlBuffers in manager.
	*/
	void DeleteAllBuffers();

	/**
	*	
	*
	*	@param hbuffer handle to the GlBuffer.
	*	@return Error value, 0 if succeed.
	*/
	int BindBuffer(HBuffer hbuffer, GlBuffer::BindTarget target);

	int ReserveData(HBuffer hbuffer, const void* data, GLsizeiptr size, GLbitfield flags);

	int ChangeData(HBuffer hbuffer, GLintptr start, const void* data, GLsizeiptr size);

	void* MapBuffer(HBuffer hbuffer, GLintptr start, GLsizeiptr length, GLbitfield access);

	int FillBuffer(HBuffer hbuffer, GLintptr start, const void* data, GLsizeiptr size,
				   GLenum dataType, GlBuffer::Channels channels);

	int CopyBuffers(HBuffer hsource, HBuffer hdestination,
					GLintptr srcStart, GLintptr dstStart, GLsizeiptr size);

	/**
	*	Returns gl handle of the GlBuffer.
	*
	*	@param hpipeline handle to the GlBuffer.
	*	@return GL handle, 0 in case of empty handle/error.
	*/
	GLuint GetGlHandle(HBuffer hbuffer);

private:
	HBufferManager _buffers;	/* Manager for handles.					*/
	NameIndex _nameIndex;		/* Map associating names with handles.	*/
};

/* Define for easier access to manager, like global variable. */
#define bufferManager GlBufferManager::GetSingleton()

}