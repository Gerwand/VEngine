#pragma once

#include "Assert.h"

namespace vengine {

class Node {
public:
	Node();
	Node(Node* parent);

	virtual ~Node();

	bool HasParent();
	bool HasChild();
	bool IsLastChild();
	bool IsFirstChild();

	virtual void AttachTo(Node* parent);
	virtual void Attach(Node* child);
	virtual void Detach();

	Node* GetParent();
	Node* GetChild();
	Node* GetNext();
	Node* GetPrevious();

	int NumberOfNodes();

	Node* GetRoot();
protected:
	Node* _parent;
	Node* _child;
	Node* _next;
	Node* _previous;
};

inline Node*
Node::GetParent()
{
	return _parent;
}

inline Node*
Node::GetChild()
{
	return _child;
}

inline Node*
Node::GetNext()
{
	return _next;
}

inline Node*
Node::GetPrevious()
{
	return _previous;
}

}