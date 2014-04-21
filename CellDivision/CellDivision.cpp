// CellDivision.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Obstacle.h"
#include "LineSegment.h"
#include <GL/glut.h>

using namespace std;

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

//TODO somewhere, check for overlapping corners, and account for them

vector<Obstacle> obstacles;
vector<LineSegment> cell_divisions;

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
	glFlush();
}

void drawSegment(LineSegment _seg) {
	glColor3f(0.5f, 0.0f, 0.5f);
	glBegin(GL_LINES);
	addGLVertex(_seg.start);
	addGLVertex(_seg.end);
	glEnd();
	glFlush();
}

void makeCells() {
	//Point topleft_start = { -1, -1 };
	for (int i = 0; i < obstacles.size(); i++) {
		vector<LineSegment> proto_segs;
		
		Point topleft_start = obstacles[i].locate_corner(0);
		topleft_start.y += 1;
		LineSegment seg0(topleft_start, {topleft_start.x, 500});
		proto_segs.push_back(seg0);

		Point bottomleft_start = obstacles[i].locate_corner(3);
		bottomleft_start.y -= 1;
		LineSegment seg3(bottomleft_start, { bottomleft_start.x, 0 });
		proto_segs.push_back(seg3);

		Point topright_start = obstacles[i].locate_corner(1);
		topright_start.y += 1;
		topright_start.x += 1;
		LineSegment seg1(topright_start, { topright_start.x, 500 });
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

		//TODO clip line foreach obstacle
		/*
		int clip_result = 0;
		for (int k = 0; k < obstacles.size(); k++) {
			clip_result = obstacles[k].clipLineSegment(seg0);
			if (clip_result == 1) {
				//overlapping corner
				break;
			}
		}
		if (clip_result != 1) {
			cell_divisions.push_back(seg0);
		}
		*/
	}

}

void display(void) {
	//draw stuff
	//cout << "display called" << endl;
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < obstacles.size(); i++) {
		drawObstacle(obstacles[i]);
	}
	for (int i = 0; i < cell_divisions.size(); i++) {
		cout << cell_divisions[i] << endl;
		drawSegment(cell_divisions[i]);
	}
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

	obstacles.push_back(Obstacle(Point(30, 450), 200));
	obstacles.push_back(Obstacle(Point(260, 300), 150));
	obstacles.push_back(Obstacle(Point(300, 120), 100));

	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	printf("Screen cleared.\n");

	makeCells();
}

void mouseMove(int x, int y) {
	//This function is called continueously on drag
	cout << "mouseMove called" << endl;
	glutPostRedisplay();
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
	glutKeyboardFunc(keyboard);
	printf("OpenGL version: %s \n", glGetString(GL_VERSION));
	glutMainLoop();
	return 0;
}

