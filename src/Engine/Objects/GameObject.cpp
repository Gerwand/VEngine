#include "GameObject.h"

namespace vengine {
const DebugConfig* GameObject::debugConfig;
unsigned int GameObject::_nextID;
GameObject::GameObjects GameObject::_destroyedObjects;

GameObject::GameObject(const std::string& name)
{
	_id = _nextID++;
	_tag = "All";
	_name = name;

	_visible = true;
	_destroyed = false;
}

GameObject::GameObject(const GameObject& source)
{
	_id = _nextID++;
	_tag = source._tag;
	_name = source._name;

	_visible = true;
	_destroyed = false;

	_transform = source._transform;
}

void 
GameObject::CloneBranchChildren(GameObject* parent)
{
	if (!HasChild())
		return;
	
	GameObject* child = (GameObject *)_child;
	do {
		GameObject* clone = Instantiate(child);
		clone->AttachTo(parent);
		child = (GameObject *)child->GetNext();
	} while (!child->IsLastChild());

}

void
GameObject::Init()
{
	OnInit();

	if (HasChild())
		((GameObject*)_child)->Init();

	if(HasParent() && !IsLastChild())
		((GameObject*)_next)->Init();
}

void 
GameObject::Draw(Renderer* renderer)
{
	_transform.UpdateMatrix();

	if(IsVisible())
		OnDraw(renderer);

	if (HasChild())
		((GameObject*)_child)->Draw(renderer);

	if (HasParent() && !IsLastChild())
		((GameObject*)_next)->Draw(renderer);

	_transform.ResetState();
}

void
GameObject::Update()
{
	OnUpdate();

	if (HasChild())
		((GameObject*)_child)->Update();

	if (HasParent() && !IsLastChild())
		((GameObject*)_next)->Update();
}

void 
GameObject::Physic()
{
	OnPhysic();

	if (HasChild())
		((GameObject*)_child)->Physic();

	if (HasParent() && !IsLastChild())
		((GameObject*)_next)->Physic();
}

void
GameObject::LateUpdate()
{
	OnLateUpdate();

	if (HasChild())
		((GameObject*)_child)->LateUpdate();

	if (HasParent() && !IsLastChild())
		((GameObject*)_next)->LateUpdate();
}

void
GameObject::Destroy(GameObject* gameObject) {
	if (gameObject->_destroyed)
		return;

	gameObject->OnDestroy();

	if (gameObject->HasChild())
		((GameObject*)gameObject->_child)->Update();

	gameObject->Detach();
	gameObject->_destroyed = true;
	GameObject::_destroyedObjects.push_back(gameObject);
}

void
GameObject::HandleDestroyed()
{
	for (GameObjects::iterator it = _destroyedObjects.begin(); it != _destroyedObjects.end(); ++it) {
		delete (*it);
	}

	_destroyedObjects.clear();
}

void 
GameObject::AttachTo(Node* parent)
{
	_transform.SetParent(&(((GameObject *)parent)->_transform));
	Node::AttachTo(parent);
}
void 
GameObject::Attach(Node* child)
{
	((GameObject *)child)->_transform.SetParent(&_transform);
	Node::Attach(child);
}

void 
GameObject::Detach()
{
	_transform.SetParent(nullptr);
	Node::Detach();
}

GameObject*
GameObject::Instantiate(GameObject* gameObject)
{
	GameObject* clone = gameObject->Clone();

	gameObject->CloneBranchChildren(clone);

	return clone;
}

void 
GameObject::LateDraw(Renderer* renderer)
{
	if (IsVisible())
		OnLateDraw(renderer);
	

	if (HasChild())
		((GameObject*)_child)->LateDraw(renderer);

	if (HasParent() && !IsLastChild())
		((GameObject*)_next)->LateDraw(renderer);
}

void 
GameObject::OnLateDraw(Renderer* renderer) {
#ifdef VE_DEBUG
	if(debugConfig->drawPositions) {
		Points centers;
		centers.Init();

		centers.SetColor(Vector3(1.0f, 0.0f, 0.0f));
		centers.AddPoint(_transform.GetWorldPosition());

		renderer->SetModelMatrix(Matrix4::identity);
		renderer->DepthTestDisable();
		centers.Draw(renderer);
		renderer->DepthTestEnable();
	}
#endif
}

}