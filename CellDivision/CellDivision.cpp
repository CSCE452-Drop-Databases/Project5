// CellDivision.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Obstacle.h"
#include "LineSegment.h"
#include "Cell.h"
#include <fstream>

#include <GL/glut.h>

using namespace std;

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

vector<Obstacle> obstacles;
vector<LineSegment> cell_divisions;
vector<Cell> cells;

vector<int> mergeList;

Point origin, destination;

struct Edge {
	int dst; // destination cell
};
vector<vector<Edge> > graph;

void addGLVertex(Point _point) {
	glVertex2i(_point.x, _point.y);
}

void drawObstacle(Obstacle _obst) {
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	for (int i = 0; i < 4; i++) {
		addGLVertex(_obst.locate_corner(i));
	}
	glEnd();
	//glFlush();
}

void drawSegment(LineSegment _seg) {
	glColor3f(0.5f, 0.0f, 0.5f);
	glBegin(GL_LINES);
	addGLVertex(_seg.start);
	addGLVertex(_seg.end);
	glEnd();
	//glFlush();
}

void colorCell(Cell c) {
	glBegin(GL_POLYGON);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(c.left_edge.get_x(), c.left_edge.y_min());
	glVertex2f(c.left_edge.get_x() + c.size, c.left_edge.y_min());
	glVertex2f(c.left_edge.get_x() + c.size, c.left_edge.y_max());
	glVertex2f(c.left_edge.get_x(), c.left_edge.y_max());
	glEnd();
}

void makeDivisions() {
	for (int i = 0; i < obstacles.size(); i++) {
		//Make 5 division lines using the obstacle's borders
		//Note: not all proto_segs will be used, such as when corners overlap
		vector<LineSegment> proto_segs;
		
		Point topleft_start = obstacles[i].locate_corner(0);
		topleft_start.y += 1;
		LineSegment seg0(topleft_start, { topleft_start.x, WINDOW_HEIGHT });
		proto_segs.push_back(seg0);

		Point bottomleft_start = obstacles[i].locate_corner(3);
		bottomleft_start.y -= 1;
		LineSegment seg3(bottomleft_start, { bottomleft_start.x, 0 });
		proto_segs.push_back(seg3);

		Point topright_start = obstacles[i].locate_corner(1);
		topright_start.y += 1;
		topright_start.x += 1;
		LineSegment seg1(topright_start, { topright_start.x, WINDOW_HEIGHT });
		proto_segs.push_back(seg1);

		Point bottomright_start = obstacles[i].locate_corner(2);
		bottomright_start.y -= 1;
		bottomright_start.x += 1;
		LineSegment seg2(bottomright_start, { bottomright_start.x, 0 });
		proto_segs.push_back(seg2);

		
		topright_start.y -= 1;
		bottomright_start.y += 1;
		LineSegment seg5(topright_start, bottomright_start);
		proto_segs.push_back(seg5);
		
		
		for (int j = 0; j < proto_segs.size(); j++) {
			int clip_result = 0;
			for (int k = 0; k < obstacles.size(); k++) {
				clip_result = obstacles[k].clipLineSegment(proto_segs[j]);
				if (clip_result == 1) {
					//overlapping corner
					break;
				}
			}
			if (clip_result != 1) {
				cell_divisions.push_back(proto_segs[j]);
			}
		}
	}

}

bool mergeCells() {
	// for merging cells that are on top of each other

	//vector<vector<int> > mergeList(cells.size());
	mergeList = vector<int>(cells.size(), -1);
	bool rValue = false;
	for (int i = 0; i < cells.size(); ++i) {
		for (int j = 0; j < cells.size(); ++j) {
			if (i == j) continue; // not checking if should merge with self
			if (cells[i].left_edge.get_x() == cells[j].left_edge.get_x() &&
				cells[i].left_edge.y_min() == cells[j].left_edge.y_max() + 1) { // should merge
				mergeList[i] = j;
				rValue = true;
				break; // only considering first merge
			}
		}
	}

	return rValue;
}

void makeCells() {
	cells.push_back(Cell(LineSegment(Point(0, 0), Point(500, 500))));
	for (int i = 0; i < cell_divisions.size(); i++) {
		cells.push_back(Cell(cell_divisions[i]));
	}

	int itn = 0;
	while (mergeCells()) {
		cout << "MERGE: Iteration: " << itn << endl;
		cout << '\t' << "Cell Count: " << cells.size() << endl;
		vector<bool> skipList(mergeList.size(), false);
		vector<Cell> newCells;
		for (int i = 0; i < mergeList.size(); ++i) {
			if (mergeList[i] != -1 && !skipList[i]) {
				// need to merge
				cout << "MERGE: merging " << i << " and " << mergeList[i] << endl;
				skipList[mergeList[i]] = true;
				int curCellI = i;
				Cell *curCell = &cells[i];
				Cell *nextCell = &cells[mergeList[i]];
				Cell tempCell;

					int leftX = curCell->left_edge.get_x();
					int minY = min(curCell->left_edge.y_min(), nextCell->left_edge.y_min());
					int maxY = max(curCell->left_edge.y_max(), nextCell->left_edge.y_max());
					LineSegment tempLeftEdge(Point(leftX, minY), Point(leftX, maxY));
					tempCell = Cell(tempLeftEdge);
					curCell = &tempCell;
					nextCell = &cells[mergeList[curCellI]];
					curCellI = mergeList[curCellI];

				newCells.push_back(tempCell);

			}
			else if (!skipList[i]) {
				// no merge needed
				newCells.push_back(cells[i]);
			}
		}

		cells = newCells;
		itn++;
	}

	for (int i = 0; i < cells.size(); i++) {
		cells[i].calcSize(cell_divisions, obstacles, WINDOW_WIDTH);
	}
}

void display(void) {
	//draw stuff
	glClear(GL_COLOR_BUFFER_BIT);

	//cout << "Robot: (" << robot.x << ", " << robot.y << ")" << endl;
	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.0f, 0.0f);
	addGLVertex(origin);
	addGLVertex(destination);
	glEnd();

	for (int i = 0; i < obstacles.size(); i++) {
		drawObstacle(obstacles[i]);
	}
	for (int i = 0; i < cell_divisions.size(); i++) {
		cout << cell_divisions[i] << endl;
		drawSegment(cell_divisions[i]);
	}

	cout << "---------------------" << endl;
	cout << cells[3].left_edge << endl;
	cout << cells[5].left_edge << endl;
	cout << cells[4].left_edge << endl;

	colorCell(cells[6]);
	//colorCell(cells[4]);

	glFlush();

}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1':
		//do stuff
		break;
	case '2':
		//do other stuff
		break;
	}
	glutPostRedisplay();
}

void init(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	gluOrtho2D(0, WINDOW_WIDTH - 1, 0, WINDOW_HEIGHT - 1);

	//obstacles.push_back(Obstacle(Point(30, 450), 200));
	//obstacles.push_back(Obstacle(Point(260, 300), 150));
	//obstacles.push_back(Obstacle(Point(300, 120), 100));

	ifstream infile("configuration.txt");
	int x, y;

	infile >> x >> y;
	origin = Point(x, y);

	infile >> x >> y;
	destination = Point(x, y);

	infile >> x >> y;
	obstacles.push_back(Obstacle(Point(x, y), 200));

	infile >> x >> y;
	obstacles.push_back(Obstacle(Point(x, y), 150));

	infile >> x >> y;
	obstacles.push_back(Obstacle(Point(x, y), 100));

	infile.close();


	//glClear(GL_COLOR_BUFFER_BIT);
	//glFlush();
	printf("Screen cleared.\n");

	makeDivisions();
	makeCells();

	graph = vector<vector<Edge> >(cells.size());

	for (int i = 0; i < cells.size(); ++i) {
		for (int j = i + 1; j < cells.size(); ++j) {
			bool touching = areTouching(cells[i], cells[j]);
			cout << "Cells " << i << " and " << j << " are ";
			if (!touching) cout << "NOT ";
			cout << " touching" << endl;

			if (touching) {
				Edge tempEdge;
				tempEdge.dst = j;
				graph[i].push_back(tempEdge);
			}

		}
	}

	for (int i = 0; i < graph.size(); ++i) {
		cout << "Cell " << i << " Has edges going to cells: ";
		for (int j = 0; j < graph[i].size(); ++j) {
			cout << graph[i][j].dst << " ";
		}
		cout << endl;
	}

}

void mouseMove(int x, int y) {
	//This function is called continueously on drag
	cout << "mouseMove called" << endl;
	//glutPostRedisplay();
}

void mouseClick(int button, int state, int x, int y) {
	if (state == GLUT_UP) {

		glutPostRedisplay();
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(800, 100); //TODO change me!
	glutCreateWindow("DROP DATABASE - Robot Pathing");
	init();
	glutDisplayFunc(display);
	glutMotionFunc(mouseMove);
	glutMouseFunc(mouseClick);
	glutKeyboardFunc(keyboard);
	printf("OpenGL version: %s \n", glGetString(GL_VERSION));
	glutMainLoop();
	return 0;
}

