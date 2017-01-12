#pragma once

#include "HandleManager.h"
#include "Singleton.h"
#include "Engine/Objects/GameObject.h"

#include <string>
#include <map>

namespace vengine {

/* Class similar to other obejct managers, containing prototypes of the objects. */
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

	/* Add given object to the gameObjectManager. Returns handle for the object */
	HObject AddGameObject(GameObject* source);

	void DeleteGameObject(HObject hobject);
	void DeleteAllGameObjects();

	/* Instantiate object with given handle */
	GameObject* Instantiate(HObject hobject);
	/* Instantiate object with given name */
	GameObject* Instantiate(const std::string& name);

private:
	HObjectManager _objects;	/* Manager for handles.					*/
	NameIndex _nameIndex;		/* Map associating names with handles.	*/
};

/* Define for easier access to manager, like global variable. */
#define gameObjectManager GameObjectManager::GetSingleton()

}