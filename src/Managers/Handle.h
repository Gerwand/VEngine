#pragma once
#include "Assert.h"

namespace vengine {


/**
*	Class represanting handles used for managing resources.
*	Template parameter be tag unique for each resource. I.e. empty struct can be used: struct MyTag{};.
*	Each resource is described as pair of index and magic number. Index is simply index in vector/array and magic number can describe abstract values,
*	like empty handle, without touch index part of the handle.
*	This class has been developed basing on "Pere³ki programowania gier, Tom 1", Helion
*	chapter 1.6.
**/
template <typename TAG>
class Handle
{
public:
	enum
	{
		EMPTY_VALUE = 0U, /* Magic number which describes not used handle (empty). */
	};

	/**
	*	Default constructor initializes handle with EMPTY_VALUE.
	*/
	Handle();
	Handle(unsigned int handle);

	/**
	*	Initializes not empty handle with magic value calculated and given index.
	*	We are assuming that pair of index and magic number is unique for each resource.
	*
	*	@param index handle will store this index inside. Must not exceed 65 535(2^16).
	*/
	void Init(unsigned int index);

	/**
	*	Get whole handle value.
	*
	*	@return Handle value.
	*/
	unsigned int GetHandle() const;
	/**
	*	Get index value stored in handle.
	*
	*	@return Index.
	*/
	unsigned int GetIndex() const;
	/**
	*	Get magic number of the handle.
	*
	*	@return Magic number.
	*/
	unsigned int GetMagicNumber() const;
	/**
	*	Check if handle is not describing any existing resource.
	*
	*	@return True is handle is empty, false if not.
	*/
	bool IsEmpty() const;

	/**
	*	Convert object to unsigned int, returning whole handle value.
	*
	*	@return Handle value.
	*/
	operator unsigned int() const;
private:
	/* Constants, in case of tuning and to keep code consistency */
	enum
	{
		/*Our unsigned value will keep index on one half and magic number on the other one		*/
		INDEX_BITS = sizeof(unsigned int) * 8U / 2U,	/* Bits used for storing index.			*/
		MAGIC_BITS = sizeof(unsigned int) * 8U / 2U,	/* Bits used for storing magic value.	*/

		MAX_INDEX = (1U << INDEX_BITS) - 1U,			/* Maximum possible index value.		*/
		MAX_MAGIC = (1U << MAGIC_BITS) - 1U,			/* Maximum possible magic number.		*/
		FIRST_MAGIC = 1U								/* First, not special magic number.		*/
	};

	/* Describes handle as one usigned int handle or struct with unsigned int divided into index and magic number. */
	union
	{
		/* Pair of both index and magic number will identify our resource */
		struct
		{
			unsigned int _index : INDEX_BITS;		/* Index describes element of vector/array of the resource.					*/
			unsigned int _magicNumber : MAGIC_BITS; /* Magic number can store special values or just stand for data validation. */
		};
		/* Handle containing information about index and special magic number. */
		unsigned int _handle;
	};
};


template <typename TAG>
Handle<TAG>::Handle() : _handle(EMPTY_VALUE) { }

template <typename TAG>
Handle<TAG>::Handle(unsigned int handle) : _handle(handle) { }

template <typename TAG>
void
Handle<TAG>::Init(unsigned int index)
{
	assert(IsEmpty(), "Cannot init Handle - not empty!");
	assert(index < MAX_INDEX, "Cannot init Handle - input index exceeds max value");

	_index = index;

	//Handle for specified TAG will have its own set of Magic Numbers
	static unsigned int magicNumber = 0;
	magicNumber = magicNumber == MAX_MAGIC ? FIRST_MAGIC : magicNumber + 1;

	_magicNumber = magicNumber;
}

template <typename TAG>
unsigned int
Handle<TAG>::GetHandle() const
{
	return _handle;
}

template <typename TAG>
unsigned int
Handle<TAG>::GetIndex() const
{
	return _index;
}


template <typename TAG>
unsigned int
Handle<TAG>::GetMagicNumber() const
{
	return _magicNumber;
}


template <typename TAG>
bool
Handle<TAG>::IsEmpty() const
{
	return _magicNumber == EMPTY_VALUE;
}


template <typename TAG>
Handle<TAG>::operator unsigned int() const
{
	return _handle;
}

}