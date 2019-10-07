#ifndef PLANE_H
#define PLANE_H

#include "Header.h"

#include "Point.h"

class Plane {
	public:
		Point normal = Point();
		real d = 0;
		Plane() = default;
		Plane(const Point* point, real d);
		void copy(const Plane* plane);
};

#endif
