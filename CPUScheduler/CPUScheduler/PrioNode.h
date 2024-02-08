#ifndef PRIO_NODE_H
#define PRIO_NODE_H
#pragma once

//Priority Node class for DataStructure: SL_PRIORITYQUEUE
//====================================================================================================================//
//======================================================== CLASS FUNCTIONS ===========================================//
//====================================================================================================================//

template <class ItemType>
class PrioNode
{
private:
    ItemType value; // Entry in list
    PrioNode<ItemType>* next; // Pointer to next node
    int priority; // Priority of the node
public:
    PrioNode();
    PrioNode(const ItemType& anItem);
    PrioNode(const ItemType& anItem, PrioNode<ItemType>* nextNodePtr);
    void setValue(const ItemType& anItem);
    void setNext(PrioNode<ItemType>* nextNodePtr);
    void setPriority(int prio);
    ItemType getValue() const;
    PrioNode<ItemType>* getNext() const;
    int getPriority() const;
    ~PrioNode();
}; // end Node

//====================================================================================================================//
//======================================================== CLASS IMPLIMENTATION ======================================//
//====================================================================================================================//

template<class ItemType>
PrioNode<ItemType>::PrioNode()					       //Default Constructor
{
    next = nullptr;
}

template<class ItemType>
PrioNode<ItemType>::PrioNode(const ItemType& anItem)			       //Non-Default Constructor: Takes only the Value of the Node
{
    value = anItem;
    next = nullptr;

}

template<class ItemType>
PrioNode<ItemType>::PrioNode(const ItemType& anItem, PrioNode<ItemType>* nxt)     //Non-Default Constructor: Takes the Value and the pointer to Next of the Node
{
    value = anItem;
    next = nxt;
}

template<class ItemType>
void PrioNode<ItemType>::setNext(PrioNode* a)		   //Sets the Next of the Node
{
    next = a;
}

template<class ItemType>
PrioNode<ItemType>* PrioNode<ItemType>::getNext()const		   //Returns pointer of class Node to the Next
{
    return next;
}

template<class ItemType>
void PrioNode<ItemType>::setValue(const ItemType& anItem)			   //Sets the Value of the Node
{
    value = anItem;
}

template<class ItemType>
ItemType PrioNode<ItemType>::getValue()	const			   //Returns value of the Node
{
    return value;
}
template<class ItemType>
int PrioNode<ItemType>::getPriority()const		   //Returns priority of the node
{
    return priority;
}
template<class ItemType>
void PrioNode<ItemType>::setPriority(int prio)			   //Sets the priority of the Node
{
    priority = prio;
}

template<class ItemType>
PrioNode<ItemType>::~PrioNode() {}					   //Default destuctor

#endif