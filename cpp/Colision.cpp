#include "Colision.h"
#include "Point.h"

//Constructors
Colision::Colision(const Point *point) {
	this->point = Point(point);
	found = false;
}

Colision::Colision(const Colision *colision) {
	point = Point(&colision->point);
	normal = Point(&colision->normal);
	color = colision->color;
	found = colision->found;
}

Colision::Colision(int x, int y, int z) {
	point = Point(x, y, z);
	found = false;
}

Colision::Colision(int x, int y, int z, 
		int32_t color, bool found) {
	point = Point(x, y, z);
	this->color = color;
	this->found = found;
}

//Setters
void Colision::copy(const Colision* colision) {
	point.copy(&colision->point);
	normal.copy(&colision->normal);
	color = colision->color;
	found = colision->found;
}
