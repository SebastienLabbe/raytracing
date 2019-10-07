#ifndef PLAYER_H
#define PLAYER_H

#include "Header.h"

#include "Point.h"

class Player {
	public:
		Point position = Point();
	        Point direction = Point(1, 0, 0);
	        Point right = Point(0, 1, 0);
	        Point up = Point(0, 0, 1);

		Player() = default;
		void computeRightUp();
		void copy(const Player* player);
		void moveTo(const Point* position);
		void lookAt(const Point* point);
		void moveTo(real x, real y, real z);
		void lookAt(real x, real y, real z);
		void rotate(real t1, real t2);
		void rotateAroundZ(real t);
		void rotateAroundRight(real t);

};




#endif
