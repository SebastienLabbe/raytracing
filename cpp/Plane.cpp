#include "Plane.h"
#include "Point.h"

Plane::Plane(const Point* point, real d) {
	normal = Point(point);
	this->d = d;
}

void Plane::copy(const Plane* plane) {
	normal.copy(&plane->normal);
	d = plane->d;
}
