#ifndef SPHERE_H
#define SPHERE_H

#include "Header.h"
#include "Point.h"

class Sphere {
	public:
		Point center = Point(0, 0, 0);
		color_t color = 0xFFFFFFFF;
		real radius = 1;

		Sphere() = default;
		Sphere(const Point* center, real radius, 
				color_t color);
		void copy(const Sphere* sphere);
};

#endif
