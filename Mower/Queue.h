#pragma once
#include <Arduino.h>

template <class T> class Queue
{
public:
	Queue();
	~Queue();

	//adds item T to the end of the queue
	void enqueue(T);

	//returns the first item in the queue and removes it 
	T dequeue();

	//returns the first item in the queue without removint it
	T front();

	//returns the number of items in the queue
	int count();

	//returns trur if there is no items in the queue
	bool isEmpty();

	//returns an exact copy of the queue
	Queue<T> copy();

private:

#define START_SIZE 2
	//const int _startSize = 2;

	//returns trur if the queue if full
	bool isFull();

	//increce the size of the queue
	void resize(int);

	T* _arr;

	int _arraySize;
	int _startIndex;
	int _itemCount;
};



//---------template class implementation ----------
template<class T>
inline Queue<T>::Queue()
{
	_arr = new T[START_SIZE];
	_startIndex = 0;
	_itemCount = 0;
}

template<class T>
inline Queue<T>::~Queue()
{
	delete _arr;
}

template<class T>
inline void Queue<T>::enqueue(T item)
{
	if (isFull())
		resize(_arraySize * 2);

	_arr[_startIndex + _itemCount] = item;
	_itemCount++;
}

template<class T>
inline T Queue<T>::dequeue()
{
	if (isEmpty())
	{
		//Serial.println("QUEUE: can't deqeue, queue is empty");
		return;
	}

	T item = front();
	_itemCount--;

	_startIndex++;	
	if (_startIndex == _arraySize)
		_startIndex = 0;

	return item;
}

template<class T>
inline T Queue<T>::front()
{
	return _arr[_startIndex];
}

template<class T>
inline int Queue<T>::count()
{	return _itemCount;}

template<class T>
inline bool Queue<T>::isEmpty()
{
	return _itemCount == 0;
}

template<class T>
inline Queue<T> Queue<T>::copy()
{
	Queue queue;
	queue._arr = new T[_arraySize];

	queue._arraySize = _arraySize;
	queue._itemCount = _itemCount;
	queue._startIndex = _startIndex;

	for (int i = 0; i < _arraySize; i++)
	{
		queue._arr[i] = _arr[i];
	}
	return queue;
}

template<class T>
inline bool Queue<T>::isFull()
{
	return _itemCount == _arraySize;
}

template<class T>
inline void Queue<T>::resize(int newSize)
{
	T* newArr = new T[newSize];

	for (int i = 0; i < _arraySize; i++)
	{newArr[i] = _arr[i];}

	delete _arr;
	_arr = newArr;
	_arraySize = newSize;
}
