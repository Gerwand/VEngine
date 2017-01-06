#pragma once
#include "Assert.h"
#include "Resources/Handle.h"
#include "Strcmp.h"

#include <vector>
#include <map>


/**
*	Class HandleManagers takes care of allocating and destroying resources, associating data with handles,
*	and validating handles.
*/
namespace vengine {
template <typename DATA, typename HANDLE>
class HandleManager
{
public:
	/**
	*	Destructor frees all allocated resources.
	*/
	~HandleManager();

	/**
	*	Allocate resource and associate handle with data.
	*
	*	@param handle reference to the handle which is going to be associated with data.
	*	@return pointer to associated resource.
	*/
	DATA* Acquire(HANDLE& handle);


	HANDLE AddItem(DATA* data);
	/**
	*	Reselase data associated with given handle.
	*
	*	@param handle handle associated with given resource.
	*/
	void Release(HANDLE handle);

	/**
	*	Get pointer to the item assocaited with given handle.
	*
	*	@param handle handle associated with given resource.
	*	@return pointer to the associated resource.
	*/
	DATA* GetItem(HANDLE handle);

	/**
	*	Get number of used handles.
	*
	*	@return Number of used handles.
	*/
	unsigned int GetUsedHandleCount() const;
	/**
	*	Check if any handles are used.
	*
	*	@return True if any handle is used, false if not.
	*/
	bool HasUsedHandles() const;

private:
	/* Typedefs for easier usage of vectors. */
	typedef std::vector<DATA*> DataVector;
	typedef std::vector<unsigned int> MagicVector;
	typedef std::vector<unsigned int> FreeVector;

	/* Vector storing pointers to the data. */
	DataVector _dataVec;
	/* Vector with magic value for data verification. */
	MagicVector _magicVec;
	/* Vector containing indexes of empty spaces in vectors to prevent from often resizes of the vector. */
	FreeVector _freeVec;
};



template <typename DATA, typename HANDLE>
HandleManager<DATA, HANDLE>::~HandleManager()
{
	for (DataVector::iterator i = _dataVec.begin(); i != _dataVec.end(); ++i) {
		DATA* item = *i;
		if (item != NULL)
			delete item;
	}
}


/**************************************************************************
*
*								Public methods
*
***************************************************************************/

/**************************************************************************
*							Allocation and free
***************************************************************************/

template <typename DATA, typename HANDLE>
HANDLE
HandleManager<DATA, HANDLE>::AddItem(DATA* data)
{
	HANDLE handle;
	unsigned int index;
	//Check for any earlier freed handles
	if (_freeVec.empty()) {
		index = _magicVec.size();

		handle.Init(index);

		_dataVec.push_back(data);
		_magicVec.push_back(handle.GetMagicNumber());
	}
	else {
		index = _freeVec.back();

		handle.Init(index);
		_freeVec.pop_back();

		_dataVec[index] = data;
		_magicVec[index] = handle.GetMagicNumber();
	}

	return handle;
}

template <typename DATA, typename HANDLE>
DATA*
HandleManager<DATA, HANDLE>::Acquire(HANDLE& handle)
{
	unsigned int index;
	//Check for any earlier freed handles
	if (_freeVec.empty()) {
		index = _magicVec.size();

		handle.Init(index);

		_dataVec.push_back(new DATA);
		_magicVec.push_back(handle.GetMagicNumber());
	}
	else {
		index = _freeVec.back();

		handle.Init(index);
		_freeVec.pop_back();

		_dataVec[index] = new DATA;
		_magicVec[index] = handle.GetMagicNumber();
	}
	return (_dataVec[index]);
}

template <typename DATA, typename HANDLE>
void
HandleManager<DATA, HANDLE>::Release(HANDLE handle)
{
	unsigned int index = handle.GetIndex();

	assert(index < _dataVec.size(), "Index must not exceed max value");
	assert(_magicVec[index] == handle.GetMagicNumber(), "Magix numbers are not equal!");

	// ok remove it - tag as unused and add to free list
	delete _dataVec[index];
	_dataVec[index] = NULL;
	_magicVec[index] = HANDLE::EMPTY_VALUE;
	_freeVec.push_back(index);
}

/**************************************************************************
*								Get item
***************************************************************************/

template <typename DATA, typename HANDLE>
DATA*
HandleManager<DATA, HANDLE>::GetItem(HANDLE handle)
{
	if (handle.IsEmpty())
		return NULL;

	unsigned int index = handle.GetIndex();
	assert((index < _dataVec.size()) && (_magicVec[index] == handle.GetMagicNumber()),
		   "Invalid handle!");

	return (_dataVec[index]);
}

/**************************************************************************
*							Get additional data
***************************************************************************/

template <typename DATA, typename HANDLE>
unsigned int
HandleManager<DATA, HANDLE>::GetUsedHandleCount() const
{
	return _magicVec.size() - freeVec.size();
}

template <typename DATA, typename HANDLE>
bool
HandleManager<DATA, HANDLE>::HasUsedHandles() const
{
	return GetUsedHandleCount() != 0;
}

}