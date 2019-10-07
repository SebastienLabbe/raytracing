#ifndef COLISION_H
#define COLISION_H

#include "Header.h"
#include "Point.h"

class Colision {
	public:
		Point point = Point(), normal = Point();
		int32_t color = 0;
		bool found = false;
		
		//Constructors
		Colision() = default;
		Colision(const Point *point);
		Colision(const Colision *colision);
		Colision(int x, int y, int z);
		Colision(int x, int y, int z, 
				int32_t color, bool found);

		//Setters
		void copy(const Colision* colision);
};

#endif
