#include "LineSegment.h"

using namespace std;

LineSegment::LineSegment() {
	start = { 0, 0 };
	end = { 0, 0 };
}

LineSegment::LineSegment(Point _start, Point _end) {
	//TODO for the purposes of this assignment,
	//misaligned x values should be an error
	start = _start;
	end = _end;
}

int LineSegment::y_max() {
	return max(start.y, end.y);
}

int LineSegment::y_min() {
	return min(start.y, end.y);
}

int LineSegment::get_x() {
	return start.x;
}

std::ostream& operator<<(std::ostream& os, const LineSegment& obj) {
	os << "(" << obj.start << ") -> (" << obj.end << ")";
	return os;
}
