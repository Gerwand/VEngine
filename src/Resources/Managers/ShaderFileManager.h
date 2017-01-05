#pragma once
#include "File.h"
#include "FileManager.h"
namespace vengine {

/**
*	ShaderFile will be using everthing that File class provides, as it does not to implement any special read or write operations.
*	It will simply read shader from normal text file.
*/
class ShaderFile : public File {};

/* Typedef for more elegant usage. */
typedef FileManager<ShaderFile> ShaderFileManager;

/* Define for easier access to manager, like global variable. */
#define shaderFileManager ShaderFileManager::GetSingleton()

}