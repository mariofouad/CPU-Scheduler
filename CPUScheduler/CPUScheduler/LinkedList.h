#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#pragma once
//====================================================================================================================//
#include "Node.h"
#include<iostream>
using namespace std;
//Linked List class one of the used data structures in our simulators<T>
//====================================================================================================================//
//======================================================== CLASS FUNCTIONS ===========================================//
//====================================================================================================================//
template <typename T>
class LinkedList
{
private:
	Node<T>* Head;	                                        //Pointer to the head of the list
	int count;
public:

	LinkedList();                                           //Default Constructor

	LinkedList(LinkedList<T>& COPY);						//copy constructor

	void PrintList()	const;                              //Prints all data in linked list

	void InsertBeg(T& data);								//Insert a node at the begining

	void DeleteAll();                                       //Delete all nodes

	void InsertEnd(T& data);								//Insert a node at the end 

	bool Find(int Key);                                     //search for a data in the nodes of the linked list

	int CountOccurance(T value);							//no of repertition of a number

	bool DeleteFirst(T& value);                             //Delete a node at the begining

	bool DeleteNode(const T& value);								//Delete a specific node with a value passed by user

	bool DeleteNodes(T value);								//Delete all nodes

	void Merge(LinkedList& L);                              //Merge two linked lists

	void Reverse();                                         //Reverse a linked list

	bool IsEmpty()const;									//Check if the list is empty

	bool Traversal(T& value, int& key);						//return the value at given index

	bool DeleteLast(T& frntEntry);                          //Delete a node at the end

	bool peek(T& value);

	~LinkedList();

	//Destructor calls delete all function to delete all nodes
};
//====================================================================================================================//
//======================================================== CLASS IMPLIMENTATION ======================================//
//====================================================================================================================//
template <typename T>
LinkedList<T>::LinkedList()                                 //Default Constructor
{
	Head = nullptr;
	count = 0;
}

template<typename T>
LinkedList<T>::LinkedList(LinkedList<T>& copy) {
	if (copy.Head == nullptr) {
		Head = nullptr;
	}
	else {
		Head = new Node<T>(copy.Head->getValue());
		Node<T>* current = Head;
		Node<T>* copyCurrent = copy.Head->getNext();

		while (copyCurrent != nullptr) {
			current->setNext(new Node<T>(copyCurrent->getValue()));
			current = current->getNext();
			copyCurrent = copyCurrent->getNext();
		}
	}
}

template <typename T>
bool LinkedList<T>::IsEmpty() const
{
	return (Head == nullptr);
}

template <typename T>
void LinkedList<T>::PrintList()	const {                    //Prints all data in linked list
	if (IsEmpty()) {
		return;
	}
	Node<T>* currentNode = Head;
	cout << currentNode->getValue();
	currentNode = currentNode->getNext();
	while (currentNode != nullptr) {
		cout << ", " << currentNode->getValue();
		currentNode = currentNode->getNext();
	}
}

template <typename T>
void LinkedList<T>::InsertBeg(T& data)                //Insert a node at the begining
{
	Node<T>* R = new Node<T>(data);
	R->setNext(Head);
	Head = R;
	count++;
}

template <typename T>
void LinkedList<T>::DeleteAll()                             //Delete all nodes
{
	Node<T>* P = Head;
	while (Head)
	{
		P = Head->getNext();
		delete Head;
		Head = P;
		count--;
	}
}

template <typename T>
void LinkedList<T>::InsertEnd(T& data)                //Insert a node at the end 
{
	Node<T>* loop = Head;
	Node<T>* p = new Node<T>(data);
	if (Head == nullptr)
	{
		Head = p;
		p->setNext(nullptr);
		return;
		count++;
	}
	while (loop->getNext())
	{
		loop = loop->getNext();
	}
	loop->setNext(p);
	p->setNext(nullptr);
	count++;
}

template <typename T>
bool LinkedList<T>::Find(int Key)                           //search for a data in the nodes of the linked list
{
	Node<T>* loop = Head;
	if (Head == nullptr)
		return false;
	while (loop)
	{
		if (loop->getItem() == Key)
		{
			return true;
		}
	}
	return false;
}

template<typename T>
bool LinkedList<T>::Traversal(T& value, int& Key)                           //search for a data in the nodes of the linked list
{
	int CountN = 0;
	Node<T>* loop = Head;
	if (Head == nullptr)
		return false;
	while (loop)
	{
		if (CountN == Key)
		{
			value = loop->getValue();
			return true;
		}
		loop = loop->getNext();
		CountN++;
	}
	return false;
}

template <typename T>
int LinkedList<T>::CountOccurance(T value)           //no of repertition of a number	
{
	int countN = 0;
	Node<T>* loop = Head;
	if (Head == nullptr)
		return countN;
	while (loop)
	{
		if (loop->getItem() == value)
		{
			countN++;
		}
		loop = loop->getNext();
	}
	return countN;
}

template <typename T>
bool LinkedList<T>::DeleteFirst(T& value)                           //Delete a node at the begining
{
	if (IsEmpty())
	{
		return false;
	}
	Node<T>* delptr = Head;
	value = Head->getValue();
	Head = Head->getNext();
	count--;
	delete delptr;
	return true;
}

template <typename T>
bool LinkedList<T>::DeleteLast(T& frntEntry)                            //Delete a node at the end
{
	Node<T>* prev = nullptr;
	Node<T>* delptr = Head;
	if (Head == nullptr)
		return false;
	if (Head->getNext() == nullptr)
	{
		frntEntry = delptr->getValue();
		delete delptr;
		Head = nullptr;
		return true;
	}
	while (delptr->getNext())
	{
		prev = delptr;
		delptr = delptr->getNext();
	}
	prev->setNext(nullptr);
	delete delptr;
	frntEntry = delptr->getValue();
	return true;
}

template<typename T>
bool LinkedList<T>::peek(T&value) {
	if (IsEmpty()) {
		// handle empty list
		return false;
	}
	value = Head->getValue();
	return true;
}

template <typename T>
bool LinkedList<T>::DeleteNode(const T& value)
{
	if (!Head) {
		// handle empty list
		return false;
	}

	if (Head->getValue() == value) {
		// handle deleting the head node
		Node<T>* delptr = Head;
		Head = Head->getNext();
		delete delptr;
		return true;
	}

	Node<T>* prev = Head;
	Node<T>* curr = Head->getNext();
	while (curr) {
		if (curr->getValue() == value) {
			// handle deleting a non-head node
			prev->setNext(curr->getNext());
			delete curr;
			return true;
		}
		prev = curr;
		curr = curr->getNext();
	}

	// handle value not found
	return false;
}


template <typename T>
bool LinkedList<T>::DeleteNodes(T value)             //Delete all nodes
{

}

template <typename T>
void LinkedList<T>::Merge(LinkedList& L)                    //Merge two linked lists
{
	Node<T>* loop = Head;
	if (L.Head == nullptr)
		return;
	if (Head == nullptr)
	{
		Head->setNext(L.Head);
	}
	else
	{
		while (loop->getNext())
		{
			loop = loop->getNext();
		}
		loop->setNext(L.Head);
		L.Head = nullptr;
	}
}

template <typename T>
void LinkedList<T>::Reverse() {}                            //Reverse a linked list

template <typename T>
LinkedList<T>::~LinkedList()                                //Destructor calls delete all function to delete all nodes
{
	DeleteAll();
}
#endif