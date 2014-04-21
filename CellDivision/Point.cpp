#include "Point.h"

Point::Point(int _x, int _y) {
	x = _x;
	y = _y;
}

std::ostream& operator<<(std::ostream& os, const Point& obj) {
	os << "x=" << obj.x << ", y=" << obj.y;
	return os;
}
