#include "GameObject.h"

namespace vengine {
#ifdef VE_DEBUG
const DebugConfig* GameObject::debugConfig;
#endif 

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
	
	/* Recursively clone all children of the branch */
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

	/* Run for all children */
	if (HasChild())
		((GameObject*)_child)->Init();

	/* And for all next objects */
	if(HasParent() && !IsLastChild())
		((GameObject*)_next)->Init();
}

void 
GameObject::Draw(Renderer* renderer)
{
	/* Update matrix before drawing, to make sure that all children have actual matrixes */
	_transform.UpdateMatrix();

	/* Only render object if it is visible */
	if(IsVisible())
		OnDraw(renderer);

	if (HasChild())
		((GameObject*)_child)->Draw(renderer);

	if (HasParent() && !IsLastChild())
		((GameObject*)_next)->Draw(renderer);

	/* Reset state at the end, because children are checking for the state */
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
GameObject::Destroy(GameObject* gameObject)
{
	/* If game is already destroyed, just skip this */
	if (gameObject->_destroyed)
		return;

	gameObject->OnDestroy();

	if (gameObject->HasChild())
		Destroy(((GameObject*)gameObject->_child));

	/* Detach object */
	gameObject->Detach();
	gameObject->_destroyed = true;
	/* And add it in structure containing destroyed object. Special routine will flush it */
	GameObject::_destroyedObjects.push_back(gameObject);
}

void
GameObject::HandleDestroyed()
{
	/* Delete all destroyed objects */
	for (GameObjects::iterator it = _destroyedObjects.begin(); it != _destroyedObjects.end(); ++it) {
		delete (*it);
	}

	_destroyedObjects.clear();
}

void 
GameObject::AttachTo(Node* parent)
{
	/* Must override Node function to update parent of the transform */
	_transform.SetParent(&(((GameObject *)parent)->_transform));
	Node::AttachTo(parent);
}
void 
GameObject::Attach(Node* child)
{
	/* Must override Node function to update parent of the transform */
	((GameObject *)child)->_transform.SetParent(&_transform);
	Node::Attach(child);
}

void 
GameObject::Detach()
{
	/* Must override Node function to update parent of the transform */
	_transform.SetParent(nullptr);
	Node::Detach();
}

GameObject*
GameObject::Instantiate(GameObject* gameObject)
{
	/* Clone object */
	GameObject* clone = gameObject->Clone();

	/* And clone all his children and attach it to the clone */
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

		/* Draw point representing center of the object */
		renderer->SetModelMatrix(Matrix4::identity);
		renderer->DepthTestDisable();
		centers.Draw(renderer);
		renderer->DepthTestEnable();
	}
#endif
}

}