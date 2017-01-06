#include "GameObjectManager.h"

namespace vengine {

GameObjectManager::~GameObjectManager()
{
	for (NameIndex::iterator i = _nameIndex.begin(); i != _nameIndex.end(); ++i) {
		_objects.Release(i->second);
	}
}

GameObjectManager::HObject 
GameObjectManager::AddGameObject(GameObject* source)
{
	NameIndexInsertRc rc = _nameIndex.insert(std::make_pair(source->GetRawName(), HObject()));

	//If this element is new
	assert(rc.second, "Failed to initialize %s object - already existing", source->GetName().c_str());
	rc.first->second = _objects.AddItem(source);

	return (rc.first->second);
}

void 
GameObjectManager::DeleteGameObject(HObject hobject)
{
	GameObject* object = _objects.GetItem(hobject);
	assert(object != NULL, "Invalid handle.");

	_nameIndex.erase(object->GetRawName());
	_objects.Release(hobject);
}

void 
GameObjectManager::DeleteAllGameObjects()
{
	for (NameIndex::iterator i = _nameIndex.begin(); i != _nameIndex.end(); ++i) {
		_objects.Release(i->second);
	}

	_nameIndex.clear();
}

GameObject* 
GameObjectManager::Instantiate(HObject hobject)
{
	GameObject* object = _objects.GetItem(hobject);
	assert(object != NULL, "Invalid handle.");

	return GameObject::Instantiate(object);
}

GameObject* 
GameObjectManager::Instantiate(const std::string& name)
{
	GameObject* object = _objects.GetItem(_nameIndex[name]);
	assert(object != NULL, "Invalid handle.");

	return GameObject::Instantiate(object);
}

}