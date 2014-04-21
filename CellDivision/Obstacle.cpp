#include "Obstacle.h"

using namespace std;

Corner::Corner() {
	location = { 0, 0 };
	does_overlap = false;
}

Corner::Corner(Point _location) {
	location = _location;
	does_overlap = false;
}

Point Corner::get_location() {
	return location;
}

bool Corner::verify_overlap(vector<Obstacle> _obstacles) {
	for (int i = 0; i < _obstacles.size(); i++) {
		if ((location.x >= _obstacles[i].x_min())
			&& (location.x <= _obstacles[i].x_max())
			&& (location.y >= _obstacles[i].y_min())
			&& (location.y <= _obstacles[i].y_max())) {
			does_overlap = true;
			return true;
		}
	}
	does_overlap = false;
	return false;
}

Obstacle::Obstacle() {
	for (int i = 0; i < 4; i++) {
		corners[i] = Corner();
	}
}

Obstacle::Obstacle(Point _topleft_corner, int _size) {
	corners[0] = Corner(_topleft_corner);
	Point topright = { _topleft_corner.x + _size, _topleft_corner.y };
	Point bottomright = { _topleft_corner.x + _size, _topleft_corner.y - _size };
	Point bottomleft = { _topleft_corner.x, _topleft_corner.y - _size };
	corners[1] = Corner(topright);
	corners[2] = Corner(bottomright);
	corners[3] = Corner(bottomleft);
}

Point Obstacle::locate_corner(unsigned int _corner_index) {
	if (_corner_index >= 4) {
		return { -1, -1 };
	} else {
		return corners[_corner_index].get_location();
	}
}

int Obstacle::x_min() {
	return corners[0].get_location().x;
}

int Obstacle::x_max() {
	return corners[1].get_location().x;
}

int Obstacle::y_min() {
	return corners[2].get_location().y;
}

int Obstacle::y_max() {
	return corners[1].get_location().y;
}

Point Obstacle::check_intersection(Point _line_start, bool _travels_positive) {
	for (int i = 0; i < 4; i++) {
		if (_line_start == corners[i].get_location()) {
			cerr << "Error: exact same corners in check_intersection" << endl;
			return { -1, -1 };
		}
	}
	
	if (_line_start.x > this->x_max() || _line_start.x < this->x_min()) {
		//The x value of the line and this obstacle don't align
		return { -1, -1 };
	} else if (_line_start.y > this->y_max() && _travels_positive) {
		//The line starts above and travels away from this obstacle
		return { -1, -1 };
	} else if (_line_start.y < this->y_min() && !(_travels_positive)) {
		//The line starts below and travels away from this obstacle
		return { -1, -1 };
	} else if (_travels_positive) {
		//The line starts below and travels towards this obstacle
		return { _line_start.x, this->y_min() };
	} else if (!(_travels_positive)) {
		//The line starts above and travels towards this obstacle
		return { _line_start.x, this->y_max() };
	} else {
		cerr << "Error: unknown edge case in check_intersection" << endl;
		return { -1, -1 };
	}
}

int Obstacle::clipLineSegment(LineSegment& _s) {
	//case where start is inside obstacle, and we should throw out the line entirely
	if ((_s.start.x >= this->x_min())
		&& (_s.start.x <= this->x_max())
		&& (_s.start.y >= this->y_min())
		&& (_s.start.y <= this->y_max())) {
		//cerr << "Warning: segment starts inside obstacle in clipLineSegment." << endl;
		return 1;
	}

	//is line moving up or down?
	bool travels_positive;
	if (_s.end.y > _s.start.y) {
		//end is above start, travels up
		travels_positive = true;
	} else if (_s.end.y < _s.start.y) {
		//end is below start, travels down
		travels_positive = false;
	} else {
		//start and end have the same y value, therefore
		//segment is 1 pixel in length and cannot be clipped further
		//cout << "segment of length 1" << endl;
		return 2;
	}

	//Many cases where the line would not collide
	if (_s.start.x < this->x_min() || _s.start.x > this->x_max()) {
		//case where line's x is outside obstacle
		//cout << "line's x is outside obstacle" << endl;
		return 3;
	} else if (_s.start.y > this->y_max() && travels_positive) {
		//case where line starts above obstacle and moves up
		//cout << "line starts above obstacle and moves up" << endl;
		return 4;
	} else if (_s.start.y < this->y_min() && !(travels_positive)) {
		//case where line starts below obstacle and moves down
		//cout << "line starts below obstacle and moves down" << endl;
		return 5;
	} else if (_s.end.y < this->y_min() && travels_positive) {
		//case where line ends below obstacle and moves up
		//cout << "line ends below obstacle and moves up" << endl;
		return 6;
	} else if (_s.end.y > this->y_max() && !(travels_positive)) {
		//case where line ends above obstacle and moves down
		//cout << "line ends above obstacle and moves down" << endl;
		return 7;
	} else if (travels_positive) {
		//if none of the previous cases happen, the line collides with the obstacle
		//case where line collides obstacles from below
		_s.end.y = this->y_min();
		//cout << "line collides obstacles from below" << endl;

		return 0;
	} else if (!(travels_positive)) {
		//case where line collides obstacles from above
		_s.end.y = this->y_max();
		//cout << "line collides obstacles from above" << endl;

		return 0;
	} else {
		//Due to logic, we should never reach this point anyway
		cerr << "Error: unknown case in clipLineSegment." << endl;
		return -1;
	}
}
