#ifndef OBJECT_H
#define CBJECT_H

#include "Header.h"
#include "Point.h"

class Triangle;
class Polygone;
class Sphere;

class Object {
	public:
		Point center = Point();
		int nPolygones = 0, nSpheres = 0;
		Polygone* polygones = NULL;
		Sphere* spheres = NULL;
		
		//Constructors
		Object() = default;

		//Setters
		void addPolygones(const Polygone* polygones, int n);
		void addSpheres(const Sphere* spheres, int n);
		void computeCenter();
		void copy(const Object* object);
		
};

#endif
