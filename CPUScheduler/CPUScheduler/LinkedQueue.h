#ifndef LINKEDQUEUE
#define	LINKEDQUEUE

#include"Node.h"
#include"LinkedQueueADT.h"
#include<iostream>

using namespace std;

template <typename T>
class LinkedQueue : public LinkedQueueADT<T> {
private:

	Node<T>* Front;
	Node<T>* Rear;
	int size = 0;

public:

	LinkedQueue();
	bool IsEmpty() const;
	bool Enqueue(T newEntry);
	bool Dequeue(T& frntEntry);
	bool peek(T& frntEntry)  const;
	void print() const;
	LinkedQueue(LinkedQueue<T>& LQ);   //copy constructor
	~LinkedQueue();
};

//Linked Queue constructor
template <typename T>
LinkedQueue<T>::LinkedQueue() {
	Front = nullptr;
	Rear = nullptr;
}

//A function to check if Queue is empty
template <typename T>
bool LinkedQueue<T>::IsEmpty() const {
	return (Front == nullptr);
}


//Function that adds a new element to our queue
template <typename T>
bool LinkedQueue<T>::Enqueue(T newEntry) {
	Node<T>* newNodePtr = new Node<T>(newEntry);
	if (IsEmpty())
		Front = newNodePtr;
	else
		Rear->setNext(newNodePtr);
	Rear = newNodePtr;
	return true;
}


//Function that removes the first Inserted element and saves its value in a passed by reference variable 
template <typename T>
bool LinkedQueue<T>::Dequeue(T& frntEntry) {
	if (IsEmpty())
		return false;

	Node<T>* nodeToDeletePtr = Front;
	frntEntry = Front->getValue();
	Front = Front->getNext();
	if (nodeToDeletePtr == Rear)
		Rear = nullptr;

	delete nodeToDeletePtr;
	return true;

}


//This Shows us the element of the first Inserted element, if queue is empty will return false
template <typename T>
bool LinkedQueue<T>::peek(T& frntEntry) const
{
	if (IsEmpty())
		return false;

	frntEntry = Front->getValue();
	return true;

}


//Queue destructor will delete all delete all nodes using dequeue function
template <typename T>
LinkedQueue<T>::~LinkedQueue() {
	T temp;
	while (Dequeue(temp));
}


//Copy Constructor function
template <typename T>
LinkedQueue<T>::LinkedQueue(LinkedQueue<T>& LQ) {
	Node<T>* NodePtr = LQ.Front;
	if (!NodePtr)
	{
		Front = Rear = nullptr;
		return;
	}

	//insert the first node
	Node<T>* ptr = new Node<T>(NodePtr->getValue());
	Front = Rear = ptr;
	NodePtr = NodePtr->getNext();

	//insert remaining nodes
	while (NodePtr)
	{
		Node<T>* ptr = new Node<T>(NodePtr->getValue());
		Rear->setNext(ptr);
		Rear = ptr;
		NodePtr = NodePtr->getNext();
	}
}

//print function 
template <class ItemType>
void LinkedQueue<ItemType>::print() const {
	if (IsEmpty()) {
		return;
	}
	Node<ItemType>* currentNode = Front;
	cout << currentNode->getValue();
	currentNode = currentNode->getNext();
	while (currentNode != nullptr) {
		cout << ", " << currentNode->getValue();
		currentNode = currentNode->getNext();
	}
}

#endif