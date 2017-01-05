#include "File.h"

namespace vengine {

/**************************************************************************
*
*						Constructors and Destructors
*
***************************************************************************/

File::File() : _file(NULL), _mode(NONE)
{
}

File::~File()
{
	Close();
}
/**************************************************************************
*
*								Public methods
*
***************************************************************************/

/**************************************************************************
*								Set functions
***************************************************************************/

int
File::Init(const std::string& fileName, unsigned int mode)
{
	assert(!IsOpened(), "Cannot initialize opened file");

	std::string fType = GetStrMode(mode);

	if (fType.empty()) {
		return VE_EVAL;
	}

	_fileName = fileName;
	_mode = (FileMode)mode;
	_strMode = fType;

	return 0;
}

int
File::SetMode(unsigned int mode)
{
	int error = 0;

	if (IsOpened()) {
		Close();
		error = Open(_fileName, mode);
	} else {
		std::string newMode = GetStrMode(mode);
		if (newMode.empty()) {
			DEBUG_INFO("Invalid mode inserted: %d", mode);
			return VE_EVAL;
		}
		_mode = (FileMode)mode;
		_strMode = newMode;
	}
	return 0;
}

/**************************************************************************
*								Get functions
***************************************************************************/

File::FileMode
File::GetMode() const
{
	return _mode;
}

std::FILE*
File::GetHandle() const
{
	return _file;
}

const std::string&
File::GetName() const
{
	return _fileName;
}

/**************************************************************************
*								File operations
***************************************************************************/

int
File::Open(const std::string& fileName, unsigned int mode)
{
	assert(mode != NONE, "Cannot open file for NONE mode.");
	int error;
	if (error = Init(fileName, mode))
		return error;

	return Open();
}

int
File::Open()
{
	assert(_mode != NONE, "Cannot open file with NONE mode.");
	assert(!_fileName.empty(), "File name cannot be empty.");

	if (IsOpened())
		return VE_CTXBUSY;
	return fopen_s(&_file, _fileName.c_str(), _strMode.c_str());
}

void
File::Close()
{	
	if (IsOpened()) {
		fclose(_file);
		_file = NULL;
	}
}

bool
File::IsOpened() const
{
	return _file != NULL;
}

/**************************************************************************
*								IO operations
***************************************************************************/

int
File::Write(const void* data, size_t size) const
{
	if (_file == NULL) {
		NORMAL_INFO("Cannot write to file %s, file is not opened properly!", _fileName.c_str());
		return VE_EIO;
	}

	if (!(_mode & (WRITE | APPEND))) {
		NORMAL_INFO("File %s was not opened for write.", _fileName.c_str());
		return VE_EMODE;
	}

	if (data == NULL) {
		NORMAL_INFO("Data is NULL!");
		return VE_EVAL;
	}

	fwrite(data, size, 1U, _file);

	return 0;
}

int
File::Read(void** destination) const
{
	if (_file == NULL) {
		NORMAL_INFO("Cannot write to file %s, file is not opened properly!", _fileName.c_str());
		return VE_EIO;
	}

	if (!(_mode & READ)) {
		NORMAL_INFO("File %s was not opened for read.", _fileName.c_str());
		return VE_EMODE;
	}
	
	long size = GetFileSize();
	*destination = new char[size + 1];

	fread(*destination, size, 1U, _file);
	((char *)*destination)[size] = '\0';

	return 0;
}

/**************************************************************************
*
*								Private methods
*
***************************************************************************/

long
File::GetFileSize() const
{
	fseek(_file, 0, SEEK_END);
	long size = ftell(_file);
	Rewind();

	return size;
}

std::string
File::GetStrMode(unsigned int mode)
{
	//In case number of argument will extend, we can keep mask argument
	//const int mask = APPEND | READ | WRITE;
	std::string result;


	switch (mode) {
	case APPEND:
		result += "a";
		break;
	case READ:
		result += "r";
		break;
	case WRITE:
		result += "r";
		break;
	//Return NULL when multiple options or none of above were passed
	default:
		DEBUG_INFO("Wrong mode for file: %x", mode);
		return result;
	}

	result += "b";

	return result;
}

void
File::Rewind() const {
	rewind(_file);
}


}