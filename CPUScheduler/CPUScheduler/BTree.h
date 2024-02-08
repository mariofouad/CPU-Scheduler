#ifndef BTREE_H
#define BTREE_H
//====================================================================================================================//
#pragma once
#include "NodeBT.h"
#include <string>
#include <iostream>
using namespace std;
//====================================================================================================================//
//======================================================== CLASS FUNCTIONS ===========================================//
//====================================================================================================================//
template<typename T>
class BTree
{
private:
	NodeBT<T>* root;

	static void rec_insertBT(NodeBT<T>*& subRoot, T key);

	static void rec_inorder_traverse(NodeBT<T>* subRoot);

	static void rec_preorder_traverse(NodeBT<T>* subRoot);

	static void rec_postorder_traverse(NodeBT<T>* subRoot);
	
	static void rec_destroy_tree(NodeBT<T>*& subRoot);

	
	static void rec_find(NodeBT<T>* subRoot, T key, T fork);
	
	static NodeBT<T>* rec_find_min(NodeBT<T>* subRoot);
	
	static int rec_max_depth(NodeBT<T>* subRoot);

	void rec_printBT(const std::string& prefix, NodeBT<T>* NodeBT, bool isLeft);

public:
	void printBT();

	BTree(void);

	~BTree(void);
	
	bool IsEmpty();

	void insertBT(T key);
	
	void inorder_traverse() const;
	
	void preorder_traverse() const;
	
	void postorder_traverse() const;
	
	void destroy_tree();

	void find(T key, T fork) const;
	
	NodeBT<T>* find_min() const;
	
	T max_depth() const;
};
//====================================================================================================================//
//======================================================== CLASS IMPLIMENTATION ======================================//
//====================================================================================================================//
template<typename T>
void BTree<T>::rec_insertBT(NodeBT<T>*& subRoot, T key)
{
	if (subRoot == nullptr)
		subRoot = new NodeBT<T>(key);
	else if (!subRoot->getleft() && !subRoot->getright())
	{
		rec_insertBT(subRoot->getleft(), key);
	}
	else if (!subRoot->getright() && subRoot->getleft())
	{
		rec_insertBT(subRoot->getright(), key);
	}
	else if (subRoot->getright() && !subRoot->getleft())
	{
		rec_insertBT(subRoot->getleft(), key);
	}
	else
	{
		rec_insertBT(subRoot->getleft(), key);
		rec_insertBT(subRoot->getright(), key);
	}

}

template<typename T>
void BTree<T>::rec_inorder_traverse(NodeBT<T>* subRoot)
{
	if (subRoot == nullptr) return;
	rec_inorder_traverse(subRoot->getleft());
	cout << subRoot->getdata() << " ";
	rec_inorder_traverse(subRoot->getright());
}

template<typename T>
void BTree<T>::rec_preorder_traverse(NodeBT<T>* subRoot)
{
	if (subRoot == nullptr) return;
	cout << subRoot->getdata() << "  ";
	rec_preorder_traverse(subRoot->getleft());
	rec_preorder_traverse(subRoot->getright());
}

template<typename T>
void BTree<T>::rec_postorder_traverse(NodeBT<T>* subRoot)
{
	if (subRoot == nullptr) return;
	rec_postorder_traverse(subRoot->getleft());
	rec_postorder_traverse(subRoot->getright());
	cout << subRoot->getdata() << "  ";
}

template<typename T>
void BTree<T>::rec_destroy_tree(NodeBT<T>*& subRoot)
{
	if (subRoot != nullptr)
	{
		// recursive call on left
		rec_destroy_tree(subRoot->getleft());

		// recursive call on right
		rec_destroy_tree(subRoot->getright());

		delete subRoot;
		subRoot = nullptr;
	}
}

template<typename T>
void BTree<T>::rec_find(NodeBT<T>* subRoot, T key, T fork)
{
	if (subRoot == nullptr)
		return;
	else if (subRoot->getdata() == key)
	{
		if (!subRoot->getleft())
			subRoot->setleft(new NodeBT<T>(fork));
		else
			subRoot->setright(new NodeBT<T>(fork));
	}
	else
	{
		rec_find(subRoot->getleft(), key, fork);
		rec_find(subRoot->getright(), key, fork);
	}
}

template<typename T>
NodeBT<T>* BTree<T>::rec_find_min(NodeBT<T>* subRoot)
{
	//TO DO
	return nullptr; // should be changed
}

template<typename T>
int BTree<T>::rec_max_depth(NodeBT<T>* subRoot)
{
	//TO DO
	return 0; // should be changed
}

template<typename T>
void BTree<T>::rec_printBT(const std::string& prefix, NodeBT<T>* NodeBT, bool isLeft)
{
	if (NodeBT != nullptr)
	{
		cout << prefix;

		cout << (isLeft ? "├─L─" : "└─R─");

		// print the value of the NodeBT
		cout << "(" << NodeBT->getdata() << ") " << std::endl;
		//string 
		// enter the next tree level - left and right branch
		rec_printBT(prefix + (isLeft ? "│   " : "    "), NodeBT->getleft(), true);
		rec_printBT(prefix + (isLeft ? "│   " : "    "), NodeBT->getright(), false);
	}
}

template<typename T>
BTree<T>::BTree()
{
	root = nullptr;
}

template<typename T>
BTree<T>::~BTree()
{
	destroy_tree();
}

template<typename T>
inline bool BTree<T>::IsEmpty()
{
	if(root==nullptr)
		return true;
	return false;
}

//==================================================================== Public Functions ============================================//
template<typename T>
void BTree<T>::insertBT(T key)
{
	rec_insertBT(root, key);
}

template<typename T>
void BTree<T>::inorder_traverse() const
{
	rec_inorder_traverse(root);
}

template<typename T>
void BTree<T>::preorder_traverse() const
{
	rec_preorder_traverse(root);
}

template<typename T>
void BTree<T>::postorder_traverse() const
{
	rec_postorder_traverse(root);
}

template<typename T>
void BTree<T>::destroy_tree() 
{
	rec_destroy_tree(root);
}

template<typename T>
void BTree<T>::printBT()
{
	cout << "\nPrinting BT, L means Left Child, R means Right Child (or Root)\n";
	rec_printBT("", root, false);
	cout << endl;
}

template<typename T>
void BTree<T>::find(T key, T fork) const // return a pointer to the NodeBT that hold the maximum value in binary search tree.
{
	rec_find(root,key, fork);
}

template<typename T>
NodeBT<T>* BTree<T>::find_min() const // return a pointer to the NodeBT that hold the minimum value in binary search tree.
{
	//TO DO
	return nullptr; // should be changed
}

template<typename T>
T BTree<T>::max_depth() const
{
	//TO DO
	return 0; // should be changed
}
#endif