#ifndef ITEM_H
#define ITEM_H

#include "Locator.h"

template<typename ElemType>
class Item {
private:
	int key;
	ElemType elem;
	int locator;


public:
	Item(const int k = 0, const ElemType& e = ElemType()) : key(k), elem(e) {}
	const int getKey() const { return key; }
	const ElemType& getElem() const { return elem; }
	const int getLocator() const { return locator; }
	void setKey(const int k) { key = k; }
	void setElem(const ElemType& e) { elem = e; }
	void setLocator(const int l) { locator = l; }
	friend bool operator<(Item<ElemType> e1, Item<ElemType> e2) {
		return (e1.key - e2.key < 0) ? true : false;
	}
};






#endif
