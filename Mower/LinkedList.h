#pragma once
template <class T> class LinkedList
{
public:
	LinkedList();
	LinkedList(T);
	~LinkedList();

	T getItem();
	LinkedList<T>* getNext();
	void add(T);
	void add(LinkedList<T> list);
	bool isEndOfList();
	void empty();

private:
	T _item;
	LinkedList* _next;
};

template<class T>
inline LinkedList<T>::LinkedList()
{
	_next == nullptr;
}

template<class T>
inline LinkedList<T>::LinkedList(T item) : LinkedList()
{
	_item = item;
}


template<class T>
inline LinkedList<T>::~LinkedList()
{
	empty();
}

template<class T>
inline T LinkedList<T>::getItem()
{
	return _item;
}

template<class T>
inline LinkedList<T>* LinkedList<T>::getNext()
{
	return _next;
}

template<class T>
inline void LinkedList<T>::add(T item)
{
	if (_item == T())
		_item = item;

	else if (isEndOfList())
		_next = new LinkedList<T>(item);
	else _next->add(item);
}

template<class T>
inline void LinkedList<T>::add(LinkedList<T> list)
{
	if (isEndOfList())
		_next = list;
	else _next->add(list);
}

template<class T>
inline bool LinkedList<T>::isEndOfList()
{
	return _next == nullptr;
}

template<class T>
inline void LinkedList<T>::empty()
{
	if (isEndOfList())
		return;

	_next->empty();
	delete _next;
}