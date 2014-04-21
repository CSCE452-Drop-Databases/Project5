#pragma once

#include "stdafx.h"

class Point {
public:
	int x, y;

	Point(int _x = 0, int _y = 0);
};

inline bool operator==(Point lhs, Point rhs) {
	if ((lhs.x == rhs.x) && (lhs.y == rhs.y)) {
		return true;
	} else {
		return false;
	}
}

std::ostream& operator<<(std::ostream& os, const Point& obj);
