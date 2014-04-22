#ifndef COMPPQ_H
#define COMPPQ_H

#include "Item.h"

template<typename ElemType>
class CompPQ {
public:
	int operator()(const Item<ElemType>& e1, const Item<ElemType>& e2) {
		return e1.getKey() - e2.getKey();
	}
};

#endif
