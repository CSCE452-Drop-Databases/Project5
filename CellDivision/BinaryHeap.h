#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include "CompPQ.h"
#include "runtime_exceptions.h"
#include <iostream>

template<typename Elemtype, typename Comp>
class BinaryHeap {
private:
	Comp comp; // CompPQ is a comparator class
	int curSize; //number of elements in heap
	Elemtype *array; //(dynamic) heap array
	int length; //the length of the array
	bool orderOK;
	static const int DEF_SIZE = 8;

	void getNewArray(int newSize) {
		array = new Elemtype[newSize];
		length = newSize;
	}

public:
	BinaryHeap(int size = DEF_SIZE) {
		curSize = 0;
		orderOK = true;
		getNewArray(size);
	}

	Elemtype& findMin() throw(EmptyHeap) {
		if (isEmpty()) throw EmptyHeap();
		if (!orderOK) buildHeap();
		return array[0];
	}

	bool isEmpty() const { return curSize == 0; }

	void insert(const Elemtype& x) {
		if (!orderOK) { //establish heap order later
			toss(x);
			return;
		}
		checkSize();
		//walk up (establish heap order now)
		int hole = curSize++;
		for (; hole > 0 && comp(array[(hole - 1) / 2], x) > 0; hole = (hole - 1) / 2) {
			array[hole] = array[(hole - 1) / 2];
		}
		array[hole] = x;
	}

	void deleteMin() throw(EmptyHeap) {
		if (isEmpty()) throw EmptyHeap();
		if (!orderOK) buildHeap();
		array[0] = array[--curSize]; //decrease size
		walkDown(0);
	}

	void deleteLoc(int l) {
		if (isEmpty()) throw EmptyHeap();
		for (int i = 0; i < curSize; ++i) {
			if (array[i].getLocator() == l) {
				array[i] = array[--curSize];
				walkDown(i);
				return;
			}
		}
		throw LocatorNotInPQ();
	}

	void walkDown(int hole) {
		int child;
		Elemtype key = array[hole];
		for (; 2 * hole + 1 < curSize; hole = child) {
			child = 2 * hole + 1;
			if (child != curSize - 1 && comp(array[child], array[child + 1]) > 0)
				child++; // right child = 2*hole+2
			if (comp(key, array[child]) > 0) array[hole] = array[child];
			else break;
		}
		array[hole] = key;
	}

	void checkSize() {
		if (curSize == length) {
			Elemtype *oldArray = array;
			getNewArray(2 * curSize);
			for (int i = 0; i < curSize; ++i) {
				array[i] = oldArray[i];
			}
			delete[] oldArray;
		}
	}

	void toss(const Elemtype& x) {
		checkSize();
		array[curSize++] = x;
		if (comp(array[(curSize - 1) / 2], x) > 0) orderOK = false;
		//if (array[(curSize-1)/2] > x) orderOK = false;
	}

	void buildHeap() {
		for (int i = (curSize - 2) / 2; i >= 0; i--) walkDown(i);
		orderOK = true;
	}

	void updateKey(int l, int Key) {
		if (isEmpty()) throw EmptyHeap();
		for (int i = 0; i < curSize; ++i) {
			if (array[i].getLocator() == l) {
				array[i].setKey(Key);
				orderOK = false;
				return;
			}
		}
		throw LocatorNotInPQ();
	}

};

#endif
