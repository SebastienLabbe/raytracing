#include "Sphere.h"


Sphere::Sphere(const Point* center, real radius, color_t color) {
	this->center = Point(center);
	this->radius = radius;
	this->color = color;
}

void Sphere::copy(const Sphere* sphere) {
	center.copy(&sphere->center);
	radius = sphere->radius;
	color = sphere->color;
}
