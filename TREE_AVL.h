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
		void deleteAll();
	
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
		void deleteAll(Node*&);
	
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
//Nodo setData
template<class T>
void BTREE<T>::Node::setDataPtr(Node*& p)
{
	dataPtr = p;
}

template<class T>
void BTREE<T>::Node::setData(const T & e)
{
	if (dataPtr == nullptr) {
		if ((dataPtr = new T(e)) == nullptr) {
			throw NodeException("MEMORIA NO DISPONIBLE, setData");
		}
	}

	else {
		*dataPtr = e;
	}
}
//area privada
//insertar en el arbol método privado
template<class T>
void BTREE<T>::insertData(Node*& r, const T & e)
{
	if (r == nullptr) {
		try {
			if ((r = new Node(e)) == nullptr) //Vacio, inserta como hoja (esta balanceada)
				throw Exception("MEMORIA NO DISPONIBLE, insertData");
		}
		catch (Node::NodeException ex) {
			throw Exception(ex.what());
		}

		return;
	}

	//Inserta en un subarbol (recursivamente)
	if (e < r->getData()) {
		insertData(r->getLeft(), e);
	}
	else {
		insertData(r->getRight(), e);
	}

	//Saliendo de la recursividad

	doBalancing(r); //Revisa factor de equilibro y aplica, rotación si es necesario
}
//area pública
//insertar en el arbol método público
template<class T>
void BTREE<T>::insertData(const T & e)
{
	insertData(root, e);
=======


//Implementacion Arbol

			///Area Privada
template<class T>
void BTREE<T>::deleteAll()			
{
	deleteAll(root);
}

template<class T>
void BTREE<T>::deleteAll(Node*& r)
{
	if (r == nullptr) {
		return;
	}

	deleteAll(r->getLeft());
	deleteAll(r->getRight());

	delete r;

	r = nullptr;
}
