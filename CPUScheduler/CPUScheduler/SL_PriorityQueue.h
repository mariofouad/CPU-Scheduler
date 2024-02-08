
#ifndef _PRIORITY_QUEUE
#define _PRIORITY_QUEUE
using namespace std;
#include <iostream>
#include "LinkedSortedList.h"

template < class ItemType>
class SL_PriorityQueue
{
private:
	LinkedSortedList<ItemType>* slistPtr; // Pointer to sorted list of
	// items in the priority queue
public:
	SL_PriorityQueue();
	SL_PriorityQueue(const SL_PriorityQueue& pq);
	~SL_PriorityQueue();
	bool isEmpty() const;
	bool add(const ItemType& newEntry, int prio);
	void print() const;
	bool peek(ItemType& entry);
	void Traversal(ItemType& entry, int position);
	bool remove(ItemType& entry);
	void RemoveSorted(ItemType& entry);

}; // end SL_PriorityQueue

template<class ItemType>
void SL_PriorityQueue<ItemType>::Traversal(ItemType& entry, int position) {
	if (isEmpty()) {
		// handle empty queue
		return;
	}
	else {
		if (position < 0 || position >= slistPtr->getLength()) {
			// handle position out of bounds
			return;
		}
		else {
			entry = slistPtr->getEntry(position + 1);
		}
	}
}

template <class ItemType>
SL_PriorityQueue<ItemType>::SL_PriorityQueue() {
	slistPtr = new LinkedSortedList<ItemType>();
} // end default constructor

template <class ItemType>
SL_PriorityQueue<ItemType>::SL_PriorityQueue(const SL_PriorityQueue& pq)
{
	// Create a new linked sorted list and copy items from the original list
	slistPtr = new LinkedSortedList<ItemType>(*pq.slistPtr);
}

template <class ItemType>
SL_PriorityQueue<ItemType>::~SL_PriorityQueue()
{
	//delete slistPtr;
} // end destructor

template < class ItemType>
bool SL_PriorityQueue<ItemType>::add(const ItemType& newEntry, int prio)
{
	slistPtr->insertSorted(newEntry, prio);
	return true;
} // end add

template <class ItemType>
bool SL_PriorityQueue<ItemType>::remove(ItemType& anEntry)
{
	// The highest-priority item is at the beginning of the sorted list (now)
	ItemType removedItem = slistPtr->getEntry(1);
	bool result = slistPtr->remove(1);
	if (result) {
		anEntry = removedItem;
		return true;
	}
	else {
		// If the list is empty or removal fails, return a default-constructed item
		return false;
	}
}
template <class ItemType>
bool SL_PriorityQueue<ItemType>::isEmpty() const
{
	return slistPtr->isEmpty();
} //end isEmpty

template <class ItemType>
void SL_PriorityQueue<ItemType>::print() const {
	if (isEmpty()) {
		return;
	}
	else {
		LinkedSortedList<ItemType>* list = slistPtr;
		cout << list->getEntry(1);
		for (int i = 2; i <= list->getLength(); i++) {
			cout << ", " << list->getEntry(i);
		}

	}
}
template<class ItemType>
void SL_PriorityQueue<ItemType>::RemoveSorted(ItemType& anEntry)                           //Delete a node at the begining
{
	if (isEmpty())
	{
		return;
	}
	else {
		slistPtr->RemoveSorted(anEntry);
	}
}
template<class ItemType>
bool SL_PriorityQueue<ItemType>::peek(ItemType& entry) {
	if (isEmpty()) {
		// handle empty queue
		return false;
	}
	entry = slistPtr->getEntry(1);
	return true;
}
#endif
