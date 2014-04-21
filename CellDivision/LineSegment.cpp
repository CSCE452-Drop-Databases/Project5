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

std::ostream& operator<<(std::ostream& os, const LineSegment& obj) {
	os << "(" << obj.start << ")->(" << obj.end << ")";
	return os;
}
