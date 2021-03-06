#pragma once

#include "stdafx.h"
#include "Point.h"
#include "LineSegment.h"

class Obstacle;

class Corner {
private:
	Point location;
	bool does_overlap;
public:
	Corner();

	Corner(Point _location);

	Point get_location();

	//Sets does_overlap on this corner if it
	//overlaps any of the given obstacles
	bool verify_overlap(std::vector<Obstacle> _obstacles);
};

//Perfectly square obstacle
class Obstacle {
private:
	// 0   1
	//
	// 3   2
	std::array<Corner, 4> corners;
public:
	Obstacle();
	
	Obstacle(Point _topleft_corner, int _size);
	
	//Returns {-1, -1} if the index is invalid
	Point locate_corner(unsigned int _corner_index);

	//Helper functions, makes life even easier
	int x_min();
	int x_max();
	int y_min();
	int y_max();
	
	//Deprecated
	Point check_intersection(Point _line_start, bool _travels_positive);

	//If the given segment collides with this obstacle, the segment is clipped
	int clipLineSegment(LineSegment& _segment);
};
