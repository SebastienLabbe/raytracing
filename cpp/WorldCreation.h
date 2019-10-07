#ifndef WORLDCREATION_H
#define WORLDCREATION_H

#include "Header.h"

class Point;
class Object;
class Camera;
class World;

void createSphere( const Point* position, Object* object, color_t color);

void createSphere( real x, real y, real z, Object* object, color_t color);

void createCube( const Point* position, Object* object, color_t color);

void createCube( real x, real y, real z, Object* object, color_t color);


void createFromFile(std::string fileName, World* world);


void moveCamera(const Point* position, 
		const Point* lookAt, Camera* camera);

void moveCamera(real px, real py, real pz, 
		real lx, real ly, real lz, 
		Camera* camera);

#endif
