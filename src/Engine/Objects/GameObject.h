#pragma once

#include "Node.h"
#include "Transform.h"
#include "Renderer.h"
#include "Points.h"
#include "Input.h"
#include "Time.h"
#include <string>

namespace vengine {


class GameObject : public Node
{
public:
	typedef std::vector<GameObject *> GameObjects;
	typedef std::vector<const Vector3&> VectorRefs;

	GameObject(const std::string& name = "Object");
	GameObject(const GameObject& source);

	void SetTransform(const Transform& transform);
	const Transform& GetTransform();

	void Rename(const std::string& name);
	void SetTag(const std::string& tag);
	std::string GetName() const;
	const std::string& GetTag() const;

	bool CompareTag(const std::string& tag) const;
	bool CompareTag(const GameObject& other) const;

	void SetVisible(bool visible);
	bool IsVisible();

	void Init();
	void Update();
	void LateUpdate();
	void Draw(Renderer* renderer);
	void LateDraw(Renderer* renderer);

	bool IsDestroyed();

	virtual GameObject* Clone() = 0;

	virtual void AttachTo(Node* parent);
	virtual void Attach(Node* child);
	virtual void Detach();

	static void Destroy(GameObject* gameObject);
	static GameObject* Instantiate(GameObject* gameObject);

protected:
	unsigned int _id;
	static unsigned int _nextID;
	std::string _tag;
	std::string _name;
	Transform _transform;

	bool _visible;
	bool _destroyed;
	
	virtual void OnInit() {}
	virtual void OnUpdate() {}
	virtual void OnLateUpdate() {}
	virtual void OnDraw(Renderer* renderer) {}
	virtual void OnLateDraw(Renderer* renderer);

	virtual void OnDestroy() {};

	void CloneBranchChildren(GameObject* parent);

	static GameObjects _destroyedObjects;
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
	_transform.Set(transform);
}

inline const Transform& 
GameObject::GetTransform()
{
	return _transform;
}

}