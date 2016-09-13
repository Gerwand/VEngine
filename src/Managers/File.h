#pragma once

#include "Errors.h"
#include "Assert.h"

#include <stdio.h>
#include <string>

namespace vengine
{

class File
{
public:
	/**
	*	Available modes for opening the file.
	*/
	enum FileMode
	{
		NONE = 0x0,		/* None. File cannot be opened with that mode.								*/
		APPEND = 0x1,	/* Append is opening file for reading and is inserting new data after previously written.	*/
		READ = 0x2,		/* Read is opening file for reading.										*/
		WRITE = 0x4,	/* Write opens file for writting, if file was existing, it overwrites it.	*/
	};

	/**
	*	Default constructor sets file data as empty. 
	*/
	File();
	/**
	*	Releases all existing resources.
	*/
	~File();

	/**
	*	Initializes empty File resource. If resource is not empty, assertion will fail.
	*	
	*	@param fileName path to the file.
	*	@param mode mode of the file for opening. Can be NONE at first, but must be changed before opening.
	*	@return Error code, 0 if succeed.
	*/
	int Init(const std::string& fileName, unsigned int mode = NONE);

	/**
	*	Open file. File must be properly initialized before using it, and mode must be other than NONE.
	*
	*	@return Error code, 0 if succeed.
	*/
	int Open();
	/**
	*	Open file with initialization.
	*
	*	@param fileName path to the file.
	*	@param mode mode of the file for opening. Must be other than NONE.
	*	@return Error code, 0 if succeed.
	*/
	int Open(const std::string& fileName, unsigned int mode);

	/**
	*	Check if file is already opened.
	*
	*	@return True if file is opened, false if not.
	*/
	bool IsOpened() const;
	
	/**
	*	Close file. If file was not opened, nothing happens.
	*/
	void Close();
	
	/**
	*	Changes file mode. If file was already opened, it will be reopened with new mode.
	*
	*	@param mode new mode for file opening.
	*	@return True if file is opened, false if not.
	*/
	int SetMode(unsigned int mode);

	/**
	*	Get file mode.
	*
	*	@return File mode.
	*/
	FileMode GetMode() const;
	/**
	*	Get file handle.
	*
	*	@return File handle.
	*/
	std::FILE* GetHandle() const;
	/**
	*	Get file name.
	*
	*	@return File name.
	*/
	const std::string& GetName() const;

	/**
	*	Write data to file. This function should be overriden by inheritating class functions.
	*
	*	@data pointer to the structure containing data for writting.
	*	@size size of the data to be written.
	*	@return Error code, 0 if succeed.
	*/
	virtual int Write(const void* data, size_t size) const;
	/**
	*	Read all data from file and put it into desired structure byte after byte.
	*	Class File will allocate resources for read
	*	data, so remember to delete it.
	*	This function should be overriden by inheritating class functions.
	*
	*	@data pointer to the structure containing data for writting.
	*	@size size of the data to be written. Remember to delete this data after getting rid of it.
	*	@return Error code, 0 if succeed.
	*/
	virtual int Read(void** destination) const;

protected:
	/**
	*	Rewind file to read it from the beginnig.
	*/
	void Rewind() const;
	/**
	*	Get size of the file.
	*
	*	@return size of the file in bytes.
	*/
	long GetFileSize() const;

private:
	/**
	*	Function that converts file mode from number to string.
	*
	*	@param mode mode for the opening.
	*	@return string contaitning values needed in fopen function.
	*/
	std::string	GetStrMode(unsigned int mode);

	FILE* _file;			/* Handle for the file.								*/
	std::string _fileName;	/* File name.										*/
	FileMode _mode;			/* Mode of the file for opening for easier check.	*/
	std::string _strMode;	/* Mode of the file for opening for function fopen.	*/
};

}