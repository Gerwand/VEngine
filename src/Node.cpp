#include "Node.h"

namespace vengine {

Node::Node()
{
	_parent = nullptr;
	_child = nullptr;
	_next = this;
	_previous = this;
}

Node::Node(Node* parent)
{
	_parent = nullptr;
	_child = nullptr;
	_next = this;
	_previous = this;
	AttachTo(parent);
}

Node::~Node()
{
	Detach();
	
	if (_child)
		delete _child;
}

bool
Node::HasParent()
{
	return _parent != 0;
}

bool 
Node::HasChild()
{
	return _child != 0;
}

bool 
Node::IsLastChild()
{
	return HasParent() && _parent->_child->_previous == this;
}

bool 
Node::IsFirstChild()
{
	return HasParent() && _parent->_child == this;
}

void 
Node::AttachTo(Node* parent)
{
	assert(parent != this, "Trying to attach object to itself - it can create infinite loop");
	if (HasParent())
		Detach();

	_parent = parent;

	Node *tmp = parent->_child;
	if (tmp != nullptr) {
		_next = tmp;
		_previous = tmp->_previous;
		tmp->_previous->_next = this;
		tmp->_previous = this;
	}
	else {
		parent->_child = this;
	}

}
void
Node::Attach(Node* child)
{
	if (child->HasParent())
		Detach();

	child->_parent = this;

	if (HasChild()) {
		child->_next = _child;
		child->_previous = _child->_previous;
		_child->_previous->_next = child;
		_child->_previous = child;
	}
	else {
		_child = child;
	}
}
void 
Node::Detach()
{
	if (HasParent() && _parent->_child == this) {
		if (IsLastChild())
			_parent->_child = nullptr;
		else
			_parent->_child = _next;
	}

	_next->_previous = _previous;
	_previous->_next = _next;
	_next = this;
	_previous = this;
}

int 
Node::NumberOfNodes()
{
	int i = 0;
	Node *n = this;
	while ((n = n->_child) != nullptr)
		++i;

	return i;
}


Node*
Node::GetRoot()
{
	Node *n = this;
	while (n->HasParent())
		n = n->_parent;

	return n;
}

}