#pragma once
#include <Assert.h>

namespace vengine {
/**
*	Template for class describing singleton object.
*	Only one copy of object of class inheritating from this class can exists at any time.
*	As a template parameter pass class which is inheritating Singleton class and it's properties.
*	Address of created singleton is stored in global variable. If singleton is created by new operator, rememebr to delete it.
*/
template <typename T>
class Singleton
{
public:
	/*
	*	Default constructor. It is calculating address of singleton object (skipping singleton data),
	*	to store only pointer to pure class T. If any other singleton was already created, assertion will fail.
	*/
	Singleton();
	/**
	*	When singleton is destroyed, it is assigning NULL to pointer storing pointer to object and another
	*	singleton can be created. It is not freeing any resources, if they were allocated, user must take care of that.
	*/
	~Singleton();

	/*
	*	Get reference to the stored singleton object. For convinience, this function can be
	*	wrapped in define, to make code shorter and to be able to call the function as global variable.
	*/
	static T& GetSingleton();
	/*
	*	Get pointer to the singleton object. Can be used to delete singleton if it was earlier created by "new".
	*/
	static T* GetSingletonPointer();
private:
	static T* _singleton; /* Private variable, containing address of the singleton object. */
};

template <typename T> T* Singleton<T>::_singleton = NULL;

template <typename T>
Singleton<T>::Singleton()
{
	assert(_singleton == NULL, "Singleton is not NULL, cannot create two singleton objects.");
	//Calculate offset between inheritating class T and our singleton, we must know how much bytes to skip
	int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
	_singleton = (T*)((int)this + offset);
}

template <typename T>
Singleton<T>::~Singleton()
{
	//This item should not be accessible after destroying
	_singleton = NULL;
}

template <typename T>
T&
Singleton<T>::GetSingleton()
{
	assert(_singleton != NULL, "Singleton is NULL!");
	return *_singleton;
}

template <typename T>
T*
Singleton<T>::GetSingletonPointer()
{
	assert(_singleton != NULL, "Singleton is NULL!");
	return _singleton;
}

}
