#include "Point.h"
#include "Plane.h"

//Constructors
Point::Point(const Point* point) {
	x = point->x;
	y = point->y;
	z = point->z;
}

Point::Point(real x, real y, real z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

//Operators
Point Point::operator+(const Point* point) const {
	return Point(x + point->x,
			y + point->y,
			z + point->z);
}

Point Point::operator-(const Point* point) const {
	return Point(x - point->x,
			y - point->y,
			z - point->z);
}

Point Point::operator-() const {
	return Point( - x,
			- y,
			- z);
}

Point Point::operator*(const Point* point) const {
	return Point(y * point->z - z * point->y,
			z * point->x - x * point->z,
			x * point->y - y * point->x);
}

Point Point::operator/(real r) const {
	return Point(x / r,
			y / r,
			z / r);
}

Point Point::operator^(real r) const {
	return Point(x * r,
			y * r,
			z * r);
}

real Point::operator%(const Point* point) const {
	return x * point->x + y * point->y + z * point->z;
}

bool Point::operator==(const Point point) const {
	return (x == point.x && y == point.y && z == point.z);
}

real Point::operator[](int i) const {
	return i == 0 ? x : (i == 1 ? y : z);
}


//Getters
real Point::distanceToPlane(const Plane* plane) const {
	return this->operator%( &plane->normal) + plane->d;
}

real Point::distanceToPoint(const Point* point) const {
	return (*this - point).norm();
}

real Point::norm() const {
	return sqrt(x * x + y * y + z * z);
}

int Point::dominantAxis() const {
	real r = std::max(std::max(abs(x), abs(y)), abs(z));
	if(abs(x) == r) {
		return 0;
	}
	if (abs(y) == r) {
		return 1;
	}
	return 2;
}

bool Point::isParallel(const Point* point) const {
	return (y * point->z - z * point->y == 0 
			&& z * point->x - x * point->z == 0
			&& x * point->y - y * point->x == 0);
}

bool Point::isPerpendicular(const Point* point) const {
	return (x * point->x + y * point->y + z * point->z == 0);
}

bool Point::isBetween(const Point* p1, const Point* p2) const {
	return (((p2->x - x) * (p1->x - x) 
		+ (p2->y - y) * (p1->y - y)
		+ (p2->z - z) * (p1->z - z)) < 0);
}

//Setters
void Point::copy(const Point* point) {
	x = point->x;
	y = point->y;
	z = point->z;
}

void Point::divide(real r) {
	x /= r;
	y /= r;
	z /= r;
}

void Point::multiply(real r) {
	x *= r;
	y *= r;
	z *= r;
}

void Point::normalize() {
	divide(norm());
}

void Point::substract(const Point* point) {
	x -= point->x;
	y -= point->y;
	z -= point->z;
}

void Point::add(const Point* point) {
	x += point->x;
	y += point->y;
	z += point->z;
}

void Point::addMultiply(const Point* point, real r) {
	x += point->x * r;
	y += point->y * r;
	z += point->z * r;
}

void Point::crossProduct(const Point* point) {
	real temp1, temp2;
	temp1 = y * point->z - z * point->y;
	temp2 = z * point->x - x * point->z;
	z = x * point->y - y * point->x;
	y = temp2;
	x = temp1;
}

