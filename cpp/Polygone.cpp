#include "Polygone.h"

void Polygone::computeCenter() {
	center.copy(&points[0]);
	for(int i = 1; i < n; i++) {
		center.add(&points[i]);
	}
	center.divide(n);
}

void Polygone::setPoints(const Point* points, int n) {
	free(this->points);
	this->points = (Point *) malloc(sizeof(Point) * n);
	for(int i = 0; i < n; i ++) {
		this->points[i].copy(&points[i]);
	}
	this->n = n;
	computeCenter();
	computePlane();
}

void Polygone::computePlane() {
	Point normal = (points[2] - &points[0]);
	Point temp = (points[1] - &points[0]);
	normal.crossProduct(&temp);
	normal.normalize();
	plane = Plane(&normal, -points[0] % &normal);
	i0 = normal.dominantAxis();
	if(i0 == 0) {
		i1 = 1;
		i2 = 2;
	} else if(i0 == 1) {
		i1 = 0;
		i2 = 2;
	} else {
		i1 = 0;
		i2 = 1;
	}
}

void Polygone::copy(const Polygone* polygone) {
	center.copy(&polygone->center);
	plane.copy(&polygone->plane);
	color = polygone->color;
	n = polygone->n;
	i0 = polygone->i0;
	i1 = polygone->i1;
	i2 = polygone->i2;
	free(points);
	points = (Point *) malloc(sizeof(Point) * n);
	memcpy(points, polygone->points, sizeof(Point) * n);
}
