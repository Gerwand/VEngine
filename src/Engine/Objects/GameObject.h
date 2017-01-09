#pragma once

#include "Engine/Objects/Node.h"
#include "Transform.h"
#include "Engine/Renderer.h"
#include "Resources/Renderables/Points.h"
#include "Resources/Renderables/Lines.h"
#include "Engine/IO/Input.h"
#include "Engine/Time.h"
#include "Engine/DebugConfig.h"

#include <string>

namespace vengine {

/*
* Class representing all objects used in program.
*/
class GameObject : public Node
{
public:
	typedef std::vector<GameObject *> GameObjects;
	typedef std::vector<const Vector3&> VectorRefs;

	/* 
	* Basic constructor, setting default values for all variables. Default tag is All.
	*
	* @param name first part of the name of the object
	*/
	GameObject(const std::string& name = "Object");
	/*
	* Copy constructor, creates exact copy of the object.
	*
	* @param source source object
	*/
	GameObject(const GameObject& source);

	/*
	* Replaces objects Transform with the new one. Transform parent is set to current node's parent.
	*
	* @param transform new transform for the object
	*/
	void SetTransform(const Transform& transform);
	/*
	* Get transform of the object
	*
	* @return reference to object's transform
	*/
	const Transform& GetTransform();

	/*
	* Renames first part of the name of the object. The second one will be object's ID.
	*
	* @param name first part of the name of the object
	*/
	void Rename(const std::string& name);
	/*
	* Sets tag for the objects to categorize it.
	*
	* @param tag new tag of the object.
	*/
	void SetTag(const std::string& tag);

	/*
	* Get full name of the object - name + id.
	*
	* @return Full name of the object.
	*/
	std::string GetName() const;
	/*
	* Get only first part of the name only, without id.
	*
	* @return First part of the name
	*/
	const std::string& GetRawName() const;
	/*
	* Get tag of the object.
	*
	* @return Tag of the object
	*/
	const std::string& GetTag() const;

	/*
	* Compare tag with string.
	*
	* @param tag string to compare
	* @return True if tags are equal, else false
	*/
	bool CompareTag(const std::string& tag) const;
	/*
	* Compare objects tags.
	*
	* @param other other object to compare
	* @return True if tags are equal, else false
	*/
	bool CompareTag(const GameObject& other) const;

	/*
	* Set is object is visible.
	*
	* @param visible true if visible, false if not
	*/
	void SetVisible(bool visible);
	/*
	* Checks if object is visible.
	*
	* @return True if visible, else false
	*/
	bool IsVisible();

	/* Calls OnInit on this object and calls Init() in all sisters and children nodes */
	void Init();
	/* Calls OnUpdate on this object and calls Update() in all sisters and children nodes */
	void Update();
	/* Calls OnPhysic on this object and calls Physic() in all sisters and children nodes */
	void Physic();
	/* Calls OnLateUpdate on this object and calls LateUpdate() in all sisters and children nodes */
	void LateUpdate();
	/* Calls OnDraw on this object and calls Draw(renderer) in all sisters and children nodes */
	void Draw(Renderer* renderer);
	/* Calls OnLateDraw on this object and calls LateDraw(renderer) in all sisters and children nodes */
	void LateDraw(Renderer* renderer);

	/* Checks if object was checked to be destroyed at the end of the frame */
	bool IsDestroyed();

	/* Clones this object. If inheritated, should return allocated object through new operator with copied data */
	virtual GameObject* Clone() = 0;

	/* Overloaded Node function for updating Transform parent */
	void AttachTo(Node* parent);
	/* Overloaded Node function for updating Transform parent */
	void Attach(Node* child);
	/* Overloaded Node function for updating Transform parent */
	void Detach();

	/*
	* Destroys given object and detaches it from hierarchy. Then it is put into _destroyedObjects structure.
	* Object is really deleted after calling HandleDestroyed. 
	*/
	static void Destroy(GameObject* gameObject);
	/* Deletes all destroyed objects since last call and frees resources stored by them */
	static void HandleDestroyed();

	/*
	* Clones desired object and all its children, then returns pointer to the new object, which is not attached
	* to any object in the hierarchy.
	*/
	static GameObject* Instantiate(GameObject* gameObject);

#ifdef VE_DEBUG
	/* Static pointer to debug config structure, which is storing information if additional should be drawn */
	static const DebugConfig* debugConfig;
#endif
protected:
	unsigned int _id;				/* Unique ID of the object - will be used in full object's name */
	static unsigned int _nextID;	/* Next used ID of the object */
	std::string _tag;				/* Tag of the obejct, used for categorization */
	std::string _name;				/* First part of the name of the object - without ID number */
	Transform _transform;			/* Local Transform of the object */

	bool _visible;					/* Is object is not visible, will not be drawn. */
	bool _destroyed;				/* Indicates that object has been destroyed, used for synchronization with Octree */
	
	/* It will be called only once, during world initialization */
	virtual void OnInit() {}
	/* Used for calcualting physic after all update calls */
	virtual void OnPhysic() {}
	/* Used for all things that should be done every frame */
	virtual void OnUpdate() {}
	/* Whenever something must be done after physic and collider cheks, put it there (i.e. camera position update to prevent shaking) */
	virtual void OnLateUpdate() {}
	/* Used for drawing */
	virtual void OnDraw(Renderer* renderer) {}
	/* Used for at the end. GameObject has implemented drawing objects global position as point if proper debug flag is turned on */
	virtual void OnLateDraw(Renderer* renderer);
	/* Whenever object is destroyed, it will be called */
	virtual void OnDestroy() {};

private:
	static GameObjects _destroyedObjects;	/* List containing all objects that had been destroyed since calling HandleDestroyed */

	/* Private function, that clones all children of the parent object. */
	void CloneBranchChildren(GameObject* parent);
};

inline void
GameObject::Rename(const std::string& name)
{
	_name = name;
}

inline void
GameObject::SetTag(const std::string& tag)
{
	_tag = tag;
}

inline std::string
GameObject::GetName() const
{
	return _name + std::to_string(_id);
}

inline const std::string&
GameObject::GetTag() const
{
	return _tag;
}

inline bool
GameObject::CompareTag(const std::string& tag) const
{
	return _tag == tag;
}
inline bool 
GameObject::CompareTag(const GameObject& other) const
{
	return _tag == other._tag;
}

inline void 
GameObject::SetVisible(bool visible)
{
	_visible = visible;
}

inline bool
GameObject::IsVisible()
{
	return _visible;
}

inline bool
GameObject::IsDestroyed()
{
	return _destroyed;
}

inline void 
GameObject::SetTransform(const Transform& transform)
{
	Transform* parent = _transform._parent;
	_transform.Set(transform);
	_transform._parent = parent;
}

inline const Transform& 
GameObject::GetTransform()
{
	return _transform;
}
inline 	const std::string&
GameObject::GetRawName() const
{
	return _name;
}

}