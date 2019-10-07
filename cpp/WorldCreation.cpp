#include "WorldCreation.h"
#include "Point.h"
#include "World.h"
#include "Object.h"
#include "Polygone.h"
#include "Sphere.h"

void createSphere( real x, real y, real z, Object* object, color_t color) {
	Point temp = Point(x, y, z);
	createSphere(&temp, object, color);
}

void createSphere( const Point* position, Object* object, color_t color) {
	Sphere * sph = new Sphere( position, 1, color);
	object->addSpheres( sph, 1);	
}


void createCube( real x, real y, real z, Object* object, color_t color) {
	Point temp = Point(x, y, z);
	createCube(&temp, object, color);
}

void createCube(const Point* position, Object* object, color_t color) {
	Point v[8];
	int c[8][3] = {
		{1, 1, 1},
		{0, 1, 1},
		{1, 0, 1},
		{0, 0, 1},
		{1, 1, 0},
		{0, 1, 0},
		{1, 0, 0},
		{0, 0, 0}};

	for(int i = 0; i < 8; i ++) {
		v[i] = Point(c[i][0], c[i][1], c[i][2]);
		v[i].add(position);
	}

	Polygone pol[6];
	
	int t[6][4] = {
		{0, 2, 3, 1},
		{0, 4, 6, 2},
		{0, 1, 5, 4},
		{7, 3, 2, 6},
		{7, 5, 1, 3},
		{7, 6, 4, 5}};

	Point* pg = (Point *) malloc(sizeof(Point) * 4);

	for(int i = 0; i < 6; i ++) {
		pol[i] = Polygone();
		for(int j = 0; j < 4; j ++) {
			pg[j].copy(&v[t[i][j]]);
		}
		pol[i].setPoints(pg, 4);
		pol[i].color = color;
		//std::cout << pol[i].center << std::endl;
		//std::cout << pol[i].plane.normal << std::endl;
	}
			std::cout << "here" << std::endl;

	object->addPolygones(pol, 6);
}

void createFromFile(std::string fileName, World* world) {
	std::ifstream file;
	real x, y, z, c;
	char T;
	color_t color;
	file.open(fileName);
	if(file.is_open()) {
		while(!file.eof()) {
			T = 'X';
			file >> T;
			if(T == 'X') continue;
			//std::cout << " hello " << std::endl;
			file >> x >> y >> z >> c;
			if(c == 0) color = RED;
			else if(c == 1) color = BLUE;
			else if(c == 2) color = GREEN;
			Object* obj = new Object();
			
			std::cout << T << std::endl;
			
			if(T == 'C') {
				createCube(x, y, z, obj, color);
				world->addObject(obj);
		
			} else if(T == 'S') {
				createSphere(x, y, z, obj, color);
				world->addObject(obj);
		
			}
		}
		std::cout << " EOF " << std::endl;
	}
	file.close();
}
