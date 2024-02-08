#ifndef NODEBT_H
#define NODEBT_H
#pragma once

//ONLY FOR TREE
//====================================================================================================================//
//======================================================== CLASS FUNCTIONS ===========================================//
//====================================================================================================================//
template<typename T>
class NodeBT
{
private:
	T data;

	NodeBT* left;

	NodeBT* right;

public:
	NodeBT(T val);

	void setdata(T d);

	T getdata();

	void setright(NodeBT<T>* p);

	NodeBT<T>*& getright();

	void setleft(NodeBT<T>* p);

	NodeBT<T>*& getleft();
};
//====================================================================================================================//
//======================================================== CLASS IMPLIMENTATION ======================================//
//====================================================================================================================//
template<typename T>
NodeBT<T>::NodeBT(T val)
{
	data = val;
	left = right = nullptr;
}

template<typename T>
void NodeBT<T>::setdata(T d)
{
	data = d;
}

template<typename T>
T NodeBT<T>::getdata()
{
	return data;
}

template<typename T>
void NodeBT<T>::setleft(NodeBT<T>* p)
{
	left = p;
}

template<typename T>
NodeBT<T>*& NodeBT<T>::getleft()
{
	return left;
}

template<typename T>
void NodeBT<T>::setright(NodeBT<T>* p)
{
	right = p;
}

template<typename T>
NodeBT<T>*& NodeBT<T>::getright()
{
	return right;
}
#endif