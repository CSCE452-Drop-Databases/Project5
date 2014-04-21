#pragma once

#include "stdafx.h"
#include "Point.h"

struct LineSegment {
	Point start;
	Point end;

	LineSegment();

	LineSegment(Point _start, Point _end);
};


std::ostream& operator<<(std::ostream& os, const LineSegment& obj);
