#include "Cell.h"

using namespace std;

Cell::Cell() {
	left_edge = LineSegment(Point(0, 0), Point(0, 0));
	size = 0;
}

Cell::Cell(LineSegment _left_edge) {
	left_edge = _left_edge;
	size = 1;
}

int Cell::calcSize(vector<LineSegment> _segs, vector<Obstacle> _obs, int _x_max) {
	int closest_x = _x_max;
	for (int i = 0; i < _obs.size(); i++) {
		//find horizontally closest obstacle
		if ((_obs[i].x_min() > left_edge.get_x()) && (_obs[i].x_min() < closest_x)) {
			//If either end of the left_edge is at the obstacle's y range:
			if (((left_edge.y_max() >= _obs[i].y_min()) && (left_edge.y_max() <= _obs[i].y_max()))
				|| ((left_edge.y_min() >= _obs[i].y_min()) && (left_edge.y_min() <= _obs[i].y_max()))) {
				closest_x = _obs[i].x_min();
			}
		}
	}
	
	for (int i = 0; i < _segs.size(); i++) {
		//find horizontally closest line
		if ((_segs[i].get_x() > left_edge.get_x()) && (_segs[i].get_x() < closest_x)) {
			if (((left_edge.y_max() >= _segs[i].y_min()) && (left_edge.y_max() <= _segs[i].y_max()))
				|| ((left_edge.y_min() >= _segs[i].y_min()) && (left_edge.y_min() <= _segs[i].y_max()))) {
				closest_x = _segs[i].get_x();
			}
		}
	}
	size = closest_x - left_edge.get_x();
	cout << "cell with size: " << size << endl;
	
	return size;
}

vector<Point> Cell::get_points() {
	vector<Point> result_points;
	if (size == 1) {
		result_points.push_back(left_edge.start);
		result_points.push_back(left_edge.end);
	} else if (size > 1) {
		result_points.push_back({left_edge.y_max(), left_edge.get_x()});
		result_points.push_back({ left_edge.y_max(), left_edge.get_x() + size - 1});
		result_points.push_back({ left_edge.y_min(), left_edge.get_x() + size - 1 });
		result_points.push_back({ left_edge.y_min(), left_edge.get_x() });
	} else {
		cerr << "Error: size less than 1 in calcSize" << endl;
	}
	return result_points;
}

bool Cell::isInside(Point _p) {
	if ((_p.y <= left_edge.y_max())
		&& (_p.y >= left_edge.y_min())
		&& (_p.x >= left_edge.get_x())
		&& (_p.x <= (left_edge.get_x() + size - 1))) {
		return true;
	} else {
		return false;
	}
}

bool areTouching(Cell _a, Cell _b) {
	//TODO implement
	return false;
}