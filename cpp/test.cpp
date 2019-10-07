#include <iostream>
#include "RayTracing.h"

using namespace std;

void test_points() {
	
	bool points_working = true;

	Point* p1 = new Point();
	Point* p2 = new Point(p1);
	Point* p3 = new Point(1, 2, 3);

	if(!(*p1 == p2)) {
		cout << "Constructor from point not working properly" << endl;
		points_working = false;
	}

	if(p3->x != 1 || p3->y != 2 || p3->z != 3) {
		cout << "Constructor from x y z not working fine" << endl;
		points_working = false;
	}
	Point a = Point(0, 0, 0);
	Point b = Point(1, 0, 0);
	Point c = Point(2, 0, 0);
	cout << c.isBetween(&a, &b) << endl;
	cout << a.isBetween(&c, &b) << endl;
	cout << b.isBetween(&a, &c) << endl;
	

	if(points_working) {
		cout << "Points working fine" << endl;
	}
}

void test_colisions() {
	
	bool colisions_work = true;

	Point* p = new Point();

	Colision* c1 = new Colision();
	Colision* c2 = new Colision(c1);
	Colision* c3 = new Colision(p);
	Colision* c4 = new Colision(1, 2, 3);
	Colision* c5 = new Colision(1, 2, 3, 0, true);

	if(colisions_work) {
		cout << "Colisions working fine" << endl;
	}

}

void test_planes() {
	
	bool planes_work = true;

	Plane* p = new Plane();

	if(planes_work) {
		cout << "Planes working fine" << endl;
	}
}

void test_lines() {
	bool lines_work = true;

	Line* l = new Line();


	if(lines_work) {
		cout << "Lines working fine" << endl;
	}
}


int main() {
	test_points();
	test_colisions();
	test_planes();
	return 0;	
}
