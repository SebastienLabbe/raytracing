#ifndef POLYGONE_H
#define POLYGONE_H

#include "Header.h"
#include "Point.h"
#include "Plane.h"


class Polygone {
	public:
		Point center = Point();
		Plane plane = Plane();
		color_t color = 0xFFFFFFFF;
		int n = 0, i0, i1, i2;
		Point* points = NULL;

		//Constructors
		Polygone() = default;
		
		//Setters
		void computeCenter();
		void setPoints(const Point* points, int n);
		void computePlane();
		void copy(const Polygone* polygone);
};

#endif
