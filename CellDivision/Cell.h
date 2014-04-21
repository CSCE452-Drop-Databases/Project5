#pragma once

#include "stdafx.h"
#include "LineSegment.h"
#include "Obstacle.h"

class Cell {
private:
	LineSegment left_edge;
	int size;
public:
	//default constructor
	Cell();

	Cell(LineSegment _left_edge);

	//Calculate the horizontal size of the Cell by
	//considering all other divisions, obstacles, and a finite right edge
	int calcSize(std::vector<LineSegment> _segments, std::vector<Obstacle> _obstacles, int _x_max);

	//Returns the 4 vertices of the cell (2 if the cell is just a line)
	std::vector<Point> get_points();
};