#pragma once
#include <iostream>
#include <exception>
#include <string>
//#include <utility>

using namespace std;

template<class T>
class BTREE
{
public:
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

	void copyAll(Node*&, Node*&);

	void insertData(Node*&, const T&); //Inserta elemento en un sub arbol
	Node*& findData(Node*&, const T&);
	void deleteAll(Node*&);
	int getHeight(Node*&);

	Node*& getTheLowest(Node*&);
	Node*& getTheHighest(Node*&);

	void parsePreOrder(Node*&);
	void parseInOrder(Node*&);
	void parsePostOrder(Node*&);

	int getBalanceFactor(Node*&);

	void simpleLeftRot(Node*&);
	void simpleRightRot(Node*&);

	void doubleLeftRot(Node*&);
	void doubleRightRot(Node*&);

	void doBalancing(Node*&);

public:
	BTREE();
	BTREE(BTREE&);
	~BTREE();

	bool isEmpty() const;
	bool isLeaf(Node*&);

	void insertData(const T&);
	void deleteData(Node*&);

	Node*& findData(const T&);

	T& retrieve(Node*&);

	void parsePreOrder();
	void parseInOrder();
	void parsePostOrder();

	void deleteAll();

	int getHeight();

	BTREE& operator=(BTREE&);
};

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

template<class T>
typename BTREE<T>::Node*& BTREE<T>::Node::getRight()
{
	return right;
}

template<class T>
typename BTREE<T>::Node*& BTREE<T>::Node::getLeft()
{
	return left;
}

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

template<class T>
void BTREE<T>::Node::setRight(Node*& p)
{
	right = p;
}

template<class T>
void BTREE<T>::Node::setLeft(Node*& p)
{
	left = p;
}

//Implementacion Arbol

			///Area Privada

template<class T>
void BTREE<T>::copyAll(Node*& from, Node*& to)
{
	if (from == nullptr) {
		return;
	}

	try {
		if ((to = new Node(from->getData())) == nullptr) {
			throw Exception("MEMORIA NO DISPONIBLE, copyAll");
			}
		}
	catch (typename Node::NodeException ex) {
		throw Exception(ex.what());
	}

	copyAll(from->getLeft(), to->getLeft());
	copyAll(from->getRight(), to->getRight());
}

template<class T>
void BTREE<T>::deleteAll()			
{
	deleteAll(root);
}

template<class T>
int BTREE<T>::getHeight()
{
	return getHeight(root);
}

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

template<class T>
typename BTREE<T>::Node*& BTREE<T>::findData(Node *& r, const T & e)
{
	if (r == nullptr || e == r->getData()) {
		return r;
	}
	if (e < r->getData()) {
		return findData(r->getLeft(), e);
	}
	return findData(r->getRight(), e);
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

template<class T>
int BTREE<T>::getHeight(Node *& r)
{
	/*if(r == nullptr){
	return 0;
	}
	int leftHight(getHeight(r->getLeft()));
	int rightHeight(getHeight(r->getRight()));

	if(leftHight > rightHeight){
	return leftHight + 1;
	}
	return rightHeight + 1;*/

	if (r == nullptr) {
		return 0;
	}
	int leftHight(getHeight(r->getLeft()));
	int rightHeight(getHeight(r->getRight()));

	return 1 + (leftHight > rightHeight ? leftHight : rightHeight);
}

template<class T>
typename BTREE<T>::Node*& BTREE<T>::getTheLowest(Node *& r)
{
	if (r == nullptr || r->getLeft() == nullptr) {
		return r;
	}
	return getTheLowest(r->getLeft());
}

template<class T>
typename BTREE<T>::Node*& BTREE<T>::getTheHighest(Node*& r)
{
	if (r == nullptr || r->getRight() == nullptr) {
		return r;
	}
	return getTheHighest(r->getRight());
}

template<class T>
void BTREE<T>::parsePreOrder(Node*& r)
{
	if (r == nullptr) {
		return;
	}

	cout << r->getData() << ",";

	parsePreOrder(r->getLeft());
	parsePreOrder(r->getRight());
}

template<class T>
void BTREE<T>::parseInOrder(Node *& r)
{
	if (r == nullptr) {
		return;
	}

	parseInOrder(r->getLeft());

	cout << r->getData() << ",";

	parseInOrder(r->getRight());
}

template<class T>
void BTREE<T>::parsePostOrder(Node*& r)
{
	if (r == nullptr) {
		return;
	}

	parsePostOrder(r->getLeft());
	parsePostOrder(r->getRight());

	cout << r->getData() << ",";
}

///Balancing Methods

template<class T>
int BTREE<T>::getBalanceFactor(Node*& r)
{
	return getHeight(r->getRight()) - getHeight(r->getLeft());
}

template<class T>
void BTREE<T>::simpleLeftRot(Node*& r)
{
	Node* aux1(r->getRight());
	Node* aux2(aux1->getLeft());

	r->setRight(aux2);
	aux1->setLeft(r);
	r = aux1;
}

template<class T>
void BTREE<T>::simpleRightRot(Node*& r)
{
	Node* aux1(r->getLeft());
	Node* aux2(aux1->getRight());

	r->setLeft(aux2);
	aux1->setRight(r);
	r = aux1;
}

template<class T>
void BTREE<T>::doubleLeftRot(Node*& r)
{
	simpleRightRot(r->getRight());
	simpleLeftRot(r);
}

template<class T>
void BTREE<T>::doubleRightRot(Node*& r)
{
	simpleLeftRot(r->getLeft());
	simpleRightRot(r);
}

template<class T>
void BTREE<T>::doBalancing(Node*& r)
{
	switch (getBalanceFactor(r))
	{
		case 2: //Aplicar Rotación a la izquierda
			if (getBalanceFactor(r->getRight()) == 1) { //Signo coincide, rotación simple
				//cout << "RSI: " << r->getData() << endl;
				simpleLeftRot(r);
			}

			else { //Signo no coincide, rotación doble
				//cout << "RDI: " << r->getData() << endl;
				doubleLeftRot(r);
			}
			break;

		case -2: //Aplicar Rotación a la derecha
			if (getBalanceFactor(r->getLeft()) == -1) {	//Signo coincide, rotación simple
				//cout << "RSD: " << r->getData() << endl;
				simpleRightRot(r);
			}

			else { //Signo no coincide, rotación doble
				//cout << "RDD: " << r->getData() << endl;
				doubleRightRot(r);
			}
			break;
	}
}

				///Area Publica

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

template<class T>
bool BTREE<T>::isEmpty() const
{
	return root == nullptr;
}

template<class T>
bool BTREE<T>::isLeaf(Node*& r)
{
	return r!= nullptr && r->getLeft() == r->getRight();
}

template<class T>
void BTREE<T>::insertData(const T & e)
{
	insertData(root, e);
}

template<class T>
void BTREE<T>::deleteData(Node *& r)
{
	if (r == nullptr) {
		throw Exception("POSICION INVALIDA, deleteData");
	}

	if (isLeaf(r)) {
		delete r;
		r = nullptr;
		return;
	}

	Node*& substitute(r->getLeft() != nullptr ? getTheHighest(r->getLeft()) : getTheLowest(r->getRight()));

	swap(r->getDataPtr(), substitute->getDataPtr());

	deleteData(substitute);
}

template<class T>
typename BTREE<T>::Node*& BTREE<T>::findData(const T & e)
{
	return findData(root, e);
}

template<class T>
T & BTREE<T>::retrieve(Node *& r)
{
	if (r == nullptr) {
		throw Exception("Posicion invalida, retrieve");
	}
	return r->getData();
}

template<class T>
void BTREE<T>::parsePreOrder()
{
	parsePreOrder(root);
}

template<class T>
void BTREE<T>::parseInOrder()
{
	parseInOrder(root);
}

template<class T>
void BTREE<T>::parsePostOrder()
{
	parsePostOrder(root);
}

template<class T>
BTREE<T>& BTREE<T>::operator=(BTREE & t)
{
	deleteAll();
	copyAll(t.root, root);

	return *this;
}

