#ifndef POINT_H
#define POINT_H

#include "Header.h"


class Plane;

class Point {
	public:
		real x = 0, y = 0, z = 0;
		
		//Constructors
		Point() = default;
		Point(const Point *point);
		Point(real x, real y, real z);

		//Operators
		Point operator+(const Point* point) const;
		Point operator-(const Point* point) const;
		Point operator-() const;
		Point operator*(const Point* point) const;
		Point operator/(real r) const;
		Point operator^(real r) const;
		real operator%(const Point* point) const;
		bool operator==(const Point point) const;
		real operator[](int i) const;
		
		//Getters
		real distanceToPlane(const Plane* plane) const;
		real distanceToPoint(const Point* point) const;
		real norm() const;
		int dominantAxis() const;
		bool isParallel(const Point* point) const;
		bool isPerpendicular(const Point* point) const;
		bool isBetween(const Point* p1, const Point* p2) const;
		
		//Setters
		void copy(const Point* point);
		void normalize();
		void divide(real r);
		void multiply(real r);
		void substract(const Point* point);
		void add(const Point* point);
		void addMultiply(const Point* point, real r);
		void crossProduct(const Point* point);
		
		friend std::ostream &operator<<(std::ostream &os, 
				const Point &p) {
			return os << "x : " << p.x 
				<< " y : " << p.y 
				<< " z : " << p.z << std::endl;
		}
};

#endif
