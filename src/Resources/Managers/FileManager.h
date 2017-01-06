#pragma once
#include "Resources/Handle.h"
#include "HandleManager.h"
#include "Singleton.h"

#include <string>
#include <map>

namespace vengine {

/**
*	Class represanting FileManagers. 
*	Template parameter should be class inheritating from class File, or another one that can cover most of the File class methods.
*	It is able to create managers for different type of files, which have to be read/write in different ways.
*	This class has been developed basing on "Pere³ki programowania gier, Tom 1", Helion
*	chapter 1.6, class TextureManager.
**/
template <typename FILETYPE>
class FileManager : public Singleton<FileManager<FILETYPE>>
{
public:
	/* Tag type for handle. */
	struct FileTag {};
	/* Handle for File type. Note that for each FILETYPE handle will be different types. */
	typedef Handle<FileTag> HFile;
private:	
	/* HandleManager for FILETYPE. */
	typedef HandleManager<FILETYPE, HFile> HFileManager;

	/**
	*	Map associating handle with the name of the resource.
	*	As third template parameter, class for string comparison is passed.
	*/
	typedef std::map<std::string, HFile, istring_less> NameIndex;
	/* Typedef for inserting item into the map */
	typedef std::pair<typename NameIndex::iterator, bool> NameIndexInsertRc;

public:


	/**
	*	Default constructor doing nothing.
	*/
	FileManager();
	/**
	*	Destructor frees all earlier allocated resources.
	*/
	~FileManager();

	/**
	*	Get handle to the file with given name. If it is opened for first time, resource is allocated, initialized and
	*	associated with the handle. If mode will be different than the previous, new one will override older one.
	*
	*	@param name path to the given file
	*	@param mode IO mode for opening, should be one of the value from File::FileMode.
	*	@return Handle to the File resource.
	*/
	HFile GetFile(const std::string& name, unsigned int mode);
	/**
	*	Get handle to the file with given name for reading. If it is opened for first time, resource is allocated, initialized and
	*	associated with the handle. If mode will be different than the previous, new one will override older one.
	*
	*	@param name path to the given file
	*	@return Handle to the File resource.
	*/
	HFile GetFileRead(const std::string& name);
	/**
	*	Get handle to the file with given name for writing. If it is opened for first time, resource is allocated, initialized and
	*	associated with the handle. If mode will be different than the previous, new one will override older one.
	*
	*	@param name path to the given file
	*	@return Handle to the File resource.
	*/
	HFile GetFileWrite(const std::string& name);
	/**
	*	Delete File resource (not file on file system).
	*	@param hfile handle to the file.
	*/
	void DestroyFile(HFile hfile);

	/**
	*	Writes data to file using Write function in given FILETYPE class.  File must be opened for write or assertion will fail.
	*	Second parameter can be ignored if FILETYPE class is more inteligent when writing file, and this parameter is class dependent.
	*
	*	@param hfile handle to the file.
	*	@param data pointer to the structure containing data for writing.
	*	@param size parameter passed to the write function, can be ignored if implementation of FILETYPE class is not using it.
	*	@return Error code, 0 if succeed.
	*/
	int WriteData(HFile hfile, void* data, int size = 0);
	/**
	*	Reads data to file using Write function in given FILETYPE class.  File must be opened for write or assertion will fail.
	*	Second parameter can be ignored if FILETYPE class is more inteligent when writing file, and this parameter is class dependent.
	*
	*	@param hfile handle to the file.
	*	@param destination pointer to the structure, where read data will be put.
	*	@return Error code, 0 if succeed.
	*/
	int ReadData(HFile hfile, void** destination);
	/**
	*	Changes IO mode of the file.
	*
	*	@param hfile handle to the file.
	*	@param mode IO mode for opening, should be one of the value from File::FileMode.
	*	@return Error code, 0 if succeed.
	*/
	int ChangeMode(HFile hfile, unsigned int mode);

private:
	HFileManager _files;	/* Manager for handles.					*/
	NameIndex _nameIndex;	/* Map associating names with handles.	*/
};


template <typename FILETYPE>
FileManager<FILETYPE>::FileManager()
{
}

template <typename FILETYPE>
FileManager<FILETYPE>::~FileManager()
{
	for (NameIndex::iterator i = _nameIndex.begin(); i != _nameIndex.end(); ++i) {
		_files.Release(i->second);
	}
}

template <typename FILETYPE>
typename FileManager<FILETYPE>::HFile
FileManager<FILETYPE>::GetFile(const std::string& name, unsigned int mode)
{
	//Initializing with empty handle. If it fails, we will return existing file.
	NameIndexInsertRc rc = _nameIndex.insert(std::make_pair(name, HFile()));

	if (rc.second) {
		File* file = _files.Acquire(rc.first->second);
		if (file->Init(name, mode)) {
			//If init fails, then delete and assign return handle
			DestroyFile(rc.first->second);
			rc.first->second = HFile();
		}
	}
	else {
		File* file = _files.GetItem(rc.first->second);
		if (mode != file->GetMode()) {
			DEBUG_INFO("Changing file mode. Was %d, now is %d.", file->GetMode(), mode);
			file->SetMode(mode);
		}
	}

	return (rc.first->second);
}

template <typename FILETYPE>
typename FileManager<FILETYPE>::HFile
FileManager<FILETYPE>::GetFileRead(const std::string& name)
{
	return GetFile(name, FILETYPE::READ);
}

template <typename FILETYPE>
typename FileManager<FILETYPE>::HFile
FileManager<FILETYPE>::GetFileWrite(const std::string& name)
{
	return GetFile(name, FILETYPE::WRITE);
}

template <typename FILETYPE>
void
FileManager<FILETYPE>::DestroyFile(HFile hfile)
{
	File* file = _files.GetItem(hfile);

	if (file != NULL) {
		_nameIndex.erase(_nameIndex.find(file->GetName()));

		_files.Release(hfile);
	}
}

template <typename FILETYPE>
int
FileManager<FILETYPE>::WriteData(HFile hfile, void* data, int size)
{
	File* file = _files.GetItem(hfile);
	int error;

	if (file == NULL)
		return VE_NORESOURCES;

	if (!file->isOpened()) {
		if (error = file->Open()) {
			NORMAL_INFO("File cannot be opened! error %d, name %s, mode %d",
						error, file->GetName(), file->GetMode());
			return error;
		}
	}

	error = file->Write(data, size);

	return error;
}

template <typename FILETYPE>
int
FileManager<FILETYPE>::ReadData(HFile hfile, void** destination)
{
	File* file = _files.GetItem(hfile);
	assert(file != NULL, "Invalid handle.");

	int error;
	if (!file->IsOpened()) {
		if (error = file->Open()) {
			NORMAL_INFO("File cannot be opened! error %d, name %s, mode %d",
						error, file->GetName().c_str(), file->GetMode());
			return error;
		}
	}

	error = file->Read(destination);

	return error;
}

template <typename FILETYPE>
int
FileManager<FILETYPE>::ChangeMode(HFile hfile, unsigned int mode)
{
	File* file = _files.GetItem(hfile);
	assert(file != NULL, "Invalid handle.");

	int error;

	if (file->isOpened())
		file->Close();

	error = file->SetMode(mode);

	return error;
}


}