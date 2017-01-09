#pragma once

#include "Assert.h"

namespace vengine {

/*
* Node class is representing node from connected list tree. Based on book OpenGL - Programowanie Gier.
*/
class Node {
public:
	/* Constructor setting parent, child to nullptr and next, previous to this */
	Node();
	/* Creates object as a child of the given parent */
	Node(Node* parent);

	/* Detaches object and deletes all children */
	virtual ~Node();

	/* Checks if object has parent */
	bool HasParent();
	/* Checks if object has child */
	bool HasChild();
	/* Checks if object is last child - next object is pointed on by parent */
	bool IsLastChild();
	/* Checks if object is first child - parent is pointing on this object*/
	bool IsFirstChild();

	/* Attaches this node to the new parent */
	void AttachTo(Node* parent);
	/* Attaches child to this node */
	void Attach(Node* child);
	/* Detaches node from the hierarchy */
	void Detach();

	/* Returns parent of the node */
	Node* GetParent();
	/* Returns parent of the node */
	Node* GetChild();
	/* Returns parent of the node */
	Node* GetNext();
	/* Returns parent of the node */
	Node* GetPrevious();

	/* Get number of all children nodes */
	int NumberOfNodes();

	/* Returns root of the tree */
	Node* GetRoot();
protected:
	Node* _parent;   /* Parent of the node */
	Node* _child;    /* Child of the node */
	Node* _next;     /* Next node */
	Node* _previous; /* Previous node */
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