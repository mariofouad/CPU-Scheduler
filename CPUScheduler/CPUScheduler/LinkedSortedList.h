/** ADT sorted list: Link-based implementation.
@file LinkedSortedList.h */
#ifndef _LINKED_SORTED_LIST
#define _LINKED_SORTED_LIST

#include "PrioNode.h"

template < class ItemType>
class LinkedSortedList
{
private:
	PrioNode<ItemType>* headPtr; // Pointer to first node in the chain
	int itemCount; // Current count of list items
	// Locates the node that is before the node that should or does
	// contain the given entry.
	// @param anEntry The entry to find.
	// @return Either a pointer to the node before the node that contains
	// or should contain the given entry, or nullptr if no prior node exists.
	PrioNode<ItemType>* getNodeBefore(const ItemType& anEntry, int prio) const;
	// Locates the node at a given position within the chain.
	PrioNode<ItemType>* getNodeAt(int position) const;
	// Returns a pointer to a copy of the chain to which origChainPtr points.
	PrioNode<ItemType>* copyChain(const PrioNode<ItemType>* origChainPtr);
public:
	LinkedSortedList();
	LinkedSortedList(const LinkedSortedList<ItemType>& aList);
	virtual ~LinkedSortedList();
	void insertSorted(const ItemType& newEntry, int prio);

	bool remove(const ItemType& anEntry);
	void RemoveSorted(ItemType& anEntry);
	int getPosition(const ItemType& newEntry) const;
	// The following methods are the same as given in ListInterface:
	bool isEmpty() const;
	int getLength() const;
	bool remove(int position);
	void clear();
	ItemType getEntry(int position) const;


}; // end LinkedSortedList

template < class ItemType>LinkedSortedList<ItemType>::LinkedSortedList() : headPtr(nullptr), itemCount(0)
{
} // end default constructor
template < class ItemType>
LinkedSortedList<ItemType>::LinkedSortedList(const LinkedSortedList<ItemType>& aList)
{
	headPtr = copyChain(aList.headPtr);
} // end copy constructor
#endif

template < class ItemType>
LinkedSortedList<ItemType>::~LinkedSortedList()
{
	clear();
} // end destructor




template<class ItemType>
PrioNode<ItemType>* LinkedSortedList<ItemType>::getNodeAt(int position) const
{
	if (position < 1 || position > itemCount)
	{
		return nullptr;
	}
	else
	{
		PrioNode<ItemType>* curPtr = headPtr;
		for (int i = 1; i < position; i++)
		{
			curPtr = curPtr->getNext();
		}
		return curPtr;
	}
}

template < class ItemType>
PrioNode<ItemType>* LinkedSortedList<ItemType>::copyChain(const PrioNode<ItemType>* origChainPtr)
{
	PrioNode<ItemType>* copiedChainPtr;
	if (origChainPtr == nullptr)
	{
		copiedChainPtr = nullptr;
		itemCount = 0;
	}
	else
	{
		// Build new chain from given one
		PrioNode<ItemType>* copiedChainPtr = new PrioNode<ItemType>(origChainPtr->getValue());
		copiedChainPtr->setNext(copyChain(origChainPtr->getNext()));
		itemCount++;
	} // end if
	return copiedChainPtr;
} // end copyChain



template < class ItemType>
void LinkedSortedList<ItemType>::insertSorted(const ItemType& newEntry, int prio)
{
	PrioNode<ItemType>* newNodePtr = new PrioNode<ItemType>(newEntry);
	newNodePtr->setPriority(prio);
	PrioNode<ItemType>* prevPtr = getNodeBefore(newEntry, prio);
	if (isEmpty() || (prevPtr == nullptr)) // Add at beginning
	{
		newNodePtr->setNext(headPtr);
		headPtr = newNodePtr;
	}
	else // Add after node before
	{
		PrioNode<ItemType>* aftPtr = prevPtr->getNext();
		newNodePtr->setNext(aftPtr);
		prevPtr->setNext(newNodePtr);
	} // end if
	itemCount++;
} // end insertSorted


template < class ItemType>
PrioNode<ItemType>* LinkedSortedList<ItemType>::getNodeBefore(const ItemType& anEntry, int prio) const
{
	PrioNode<ItemType>* curPtr = headPtr;
	PrioNode<ItemType>* prevPtr = nullptr;
	while ((curPtr != nullptr) && (prio > curPtr->getPriority()))
	{
		prevPtr = curPtr;
		curPtr = curPtr->getNext();
	} // end while
	return prevPtr;
} // end getNodeBefore

template <class ItemType>
bool LinkedSortedList<ItemType>::remove(const ItemType& anEntry)
{
	PrioNode<ItemType>* nodeToRemovePtr = headPtr;
	PrioNode<ItemType>* prevPtr = nullptr;
	bool found = false;
	while (!found && (nodeToRemovePtr != nullptr))
	{
		if (anEntry == nodeToRemovePtr->getValue())
		{
			found = true;
		}
		else
		{
			prevPtr = nodeToRemovePtr;
			nodeToRemovePtr = nodeToRemovePtr->getNext();
		}
	}

	bool canRemove = found;
	if (canRemove)
	{
		if (nodeToRemovePtr == headPtr)
		{
			headPtr = headPtr->getNext();
		}
		else
		{
			prevPtr->setNext(nodeToRemovePtr->getNext());
		}

		nodeToRemovePtr->setNext(nullptr);
		delete nodeToRemovePtr;
		nodeToRemovePtr = nullptr;
		itemCount--;
	}

	return canRemove;
}
template<class ItemType>
void LinkedSortedList<ItemType>::RemoveSorted(ItemType& anEntry) {

	if (isEmpty())
	{
		return;
	}
	PrioNode<ItemType>* delptr = headPtr;
	anEntry = headPtr->getValue();
	headPtr = headPtr->getNext();
	itemCount--;
	delete delptr;
}
template <class ItemType>
bool LinkedSortedList<ItemType>::isEmpty() const
{
	return itemCount == 0;
} // end isEmpty

template < class ItemType>
int LinkedSortedList<ItemType>::getLength() const
{
	return itemCount;
} // end getLength

template<class ItemType>
bool LinkedSortedList<ItemType>::remove(int position)
{
	bool ableToRemove = (position >= 1) && (position <= itemCount);
	if (ableToRemove)
	{
		PrioNode<ItemType>* curPtr = nullptr;
		if (position == 1)
		{
			curPtr = headPtr;
			headPtr = headPtr->getNext();
		}
		else
		{
			PrioNode<ItemType>* prevPtr = getNodeAt(position - 1);
			curPtr = prevPtr->getNext();
			prevPtr->setNext(curPtr->getNext());
		}
		curPtr->setNext(nullptr);
		delete curPtr;
		curPtr = nullptr;
		itemCount--;
	}
	return ableToRemove;
}  // end remove

template < class ItemType>
void LinkedSortedList<ItemType>::clear()
{
	while (!isEmpty())
		remove(1);
}  //end clear

template < class ItemType>
int LinkedSortedList<ItemType>::getPosition(const ItemType& newEntry) const
{
	int position = 1;
	PrioNode<ItemType>* curPtr = headPtr;
	while ((curPtr != nullptr) && (newEntry > curPtr->getValue()))
	{
		curPtr = curPtr->getNext();
		position++;
	}
	if (curPtr == nullptr || curPtr->getValue() != newEntry)
	{
		position = -1;
	}
	return position;
} // end getPosition



template < class ItemType>
ItemType LinkedSortedList<ItemType>::getEntry(int position) const
{
	bool ableToGet = (position >= 1) && (position <= itemCount);
	if (ableToGet)
	{
		PrioNode<ItemType>* nodePtr = getNodeAt(position);
		return nodePtr->getValue();
	}
	else
	{
		return ItemType();
	}  // end if
}  // end getEntry
