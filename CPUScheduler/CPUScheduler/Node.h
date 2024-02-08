#ifndef NODE_H
#define NODE_H
#pragma once

//Node class for DataStructures; LinkedList and LinkedQueue
//====================================================================================================================//
//======================================================== CLASS FUNCTIONS ===========================================//
//====================================================================================================================//

template<typename T>
class Node {
	T value;
	Node* next;
public:

	Node();                            //Default Constructor

	Node(T vlu);                       //Non-Default Constructor: Takes only the Value of the Node 

	Node(T vlu, Node<T>* nxt);         //Non-Default Constructor: Takes the Value and the pointer to Next of the Node

	void setNext(Node* a);		       //Sets the Next of the Node

	Node* getNext();			       //Returns pointer of class Node to the Next

	void setValue(T s);                //Sets the Value of the Node

	T getValue();                      //Returns value of the Node

	~Node();					       //Default destuctor
};
//====================================================================================================================//
//======================================================== CLASS IMPLIMENTATION ======================================//
//====================================================================================================================//

template<typename T>
Node<T>::Node()					       //Default Constructor
{
	next = nullptr;
}

template<typename T>
Node<T>::Node(T vlu)			       //Non-Default Constructor: Takes only the Value of the Node
{
	value = vlu;
	next = nullptr;
}

template<typename T>
Node<T>::Node(T vlu, Node<T>* nxt)     //Non-Default Constructor: Takes the Value and the pointer to Next of the Node
{
	value = vlu;
	next = nxt;
}

template<typename T>
void Node<T>::setNext(Node* a)		   //Sets the Next of the Node
{
	next = a;
}

template<typename T>
Node<T>* Node<T>::getNext()			   //Returns pointer of class Node to the Next
{
	return next;
}

template<typename T>
void Node<T>::setValue(T s)			   //Sets the Value of the Node
{
	value = s;
}

template<typename T>
T Node<T>::getValue()				   //Returns value of the Node
{
	return value;
}

template<typename T>
Node<T>::~Node() {}					   //Default destuctor

#endif