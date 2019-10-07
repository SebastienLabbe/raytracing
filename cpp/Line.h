#ifndef LINE_H
#define LINE_H

#include "Header.h"

#include "Point.h"

class Line {
	public:
		Point origin = Point(), direction = Point(1, 0, 0);
		Line() = default;
		Line(const Point* origin, const Point* direction);
		void copy(const Line* ray);
};

#endif
