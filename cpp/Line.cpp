#include "Line.h"
#include "Point.h"

Line::Line(const Point* origin, const Point* direction) {
	this->origin = Point(origin);
	this->direction = Point(direction);
}

void Line::copy(const Line* ray) {
	origin.copy(&ray->origin);
	direction.copy(&ray->direction);
}
