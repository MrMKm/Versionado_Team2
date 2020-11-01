#pragma once
#include <iostream>
#include <exception>
#include <string>

using namespace std;

template<class T>
class BTREE
{
	public:
		BTREE();
		BTREE(BTREE&);
		~BTREE();

	private:
		Node* root; //Raiz del Arbol
	
}

///Area Publica del arbol
template<class T>
BTREE<T>::BTREE() : root(nullptr) {}

template<class T>
BTREE<T>::BTREE(BTREE & t) : root(nullptr)
{
	copyAll(t);
}

template<class T>
BTREE<T>::~BTREE()
{
	deleteAll();
}
