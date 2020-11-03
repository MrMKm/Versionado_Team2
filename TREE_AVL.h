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
	
		class Exception : public std::exception {
		private:
			std::string msg;
		public:
			explicit Exception(const char* message) : msg(message) {}

			explicit Exception(const std::string &message) : msg(message) { }

			virtual ~Exception() throw () {}

			virtual const char* what() const throw () {
				return msg.c_str();
			}
		};
		
		class Node
		{
			public:
				class NodeException : public std::exception {
				private:
					std::string msg;
				public:
					explicit NodeException(const char* message) : msg(message) {}

					explicit NodeException(const std::string &message) : msg(message) { }

					virtual ~NodeException() throw () {}

					virtual const char* what() const throw () {
						return msg.c_str();
					}
				};

				Node();
				Node(const T&);
				~Node();

				T& getData();
				T*& getDataPtr();
				Node*& getRight();
				Node*& getLeft();

				void setDataPtr(Node*&);
				void setData(const T&);
				void setRight(Node*&);
				void setLeft(Node*&);

			private:
				T* dataPtr; //Apuntador al dato
				Node* right;
				Node* left;
		};

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

///Implementacion
//Nodo
template<class T>
BTREE<T>::Node::Node() : dataPtr(nullptr), left(nullptr), right(nullptr) { }

template<class T>
BTREE<T>::Node::Node(const T& e) : dataPtr(new T(e)), left(nullptr), right(nullptr) {
	if (dataPtr == nullptr) {
		throw NodeException("MEMORIA NO DISPONIBLE, createNodo");
	}
}

template<class T>
BTREE<T>::Node::~Node()
{
	delete dataPtr;
}
//Nodo getData
template<class T>
T& BTREE<T>::Node::getData()
{
	return *dataPtr;
}

template<class T>
T*& BTREE<T>::Node::getDataPtr()
{
	return dataPtr;
}
