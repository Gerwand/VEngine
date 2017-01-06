#pragma once

#include "HandleManager.h"
#include "Singleton.h"
#include "Engine/Objects/GameObject.h"

#include <string>
#include <map>

namespace vengine {

class GameObjectManager : public Singleton<GameObjectManager>
{
public:

	struct ObjectTag {};

	typedef Handle<ObjectTag> HObject;
private:

	typedef HandleManager<GameObject, HObject> HObjectManager;

	typedef std::map<std::string, HObject, istring_less> NameIndex;

	typedef std::pair<typename NameIndex::iterator, bool> NameIndexInsertRc;


public:
	~GameObjectManager();

	HObject AddGameObject(GameObject* source);

	void DeleteGameObject(HObject hmesh);

	void DeleteAllGameObjects();

	GameObject* Instantiate(HObject object);
	GameObject* Instantiate(const std::string& name);

private:
	HObjectManager _objects;	/* Manager for handles.					*/
	NameIndex _nameIndex;				/* Map associating names with handles.	*/
};

/* Define for easier access to manager, like global variable. */
#define gameObjectManager GameObjectManager::GetSingleton()

}