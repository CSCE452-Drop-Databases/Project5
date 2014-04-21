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
