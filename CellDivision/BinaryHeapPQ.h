#ifndef BINARY_HEAP_PQ_H
#define BINARY_HEAP_PQ_H

#include "BinaryHeap.h"
#include "Item.h"
#include "CompPQ.h"
#include "Locator.h"

template<typename ElemType>
class BinaryHeapPQ {
protected:
	typedef Item<ElemType> Item;
	typedef CompPQ<ElemType> comp;
	Locator loc;

private:
	BinaryHeap<Item, comp> T;
	static const int DEF_SIZE = 8;

public:
	BinaryHeapPQ(int size = DEF_SIZE) : T(size) {}
	void createPriorityQueue() {
		T.buildHeap();
	}

	bool isEmpty() const { return T.isEmpty(); }
	int insert(const int k, const ElemType& e) {
		Item temp(k, e);
		temp.setLocator(loc.nextLocator());
		T.insert(temp);
		return temp.getLocator();
	}

	const ElemType& minElement() throw(EmptyPriorityQueue) {
		if (T.isEmpty()) throw EmptyPriorityQueue();
		return T.findMin().getElem();
	}

	const int minKey() throw(EmptyPriorityQueue) {
		if (T.isEmpty()) throw EmptyPriorityQueue();
		return T.findMin().getKey();
	}

	void removeMin() throw(EmptyPriorityQueue) {
		if (T.isEmpty()) throw EmptyPriorityQueue();
		T.deleteMin();
	}

	//removes the item with given locator
	void remove(int l) throw(EmptyPriorityQueue) {
		if (T.isEmpty()) throw EmptyPriorityQueue();
		T.deleteLoc(l);
	}

	int min() throw(EmptyPriorityQueue) {
		if (T.isEmpty()) throw EmptyPriorityQueue();
		return T.findMin().getLocator();
	}

	void decreaseKey(int l, int k) {
		T.updateKey(l, k);
	}


};

#endif
