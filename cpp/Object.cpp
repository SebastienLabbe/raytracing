#include "Object.h"
#include "Polygone.h"
#include "Sphere.h"

void Object::addPolygones(const Polygone* polygones, int n) {
	this->polygones = (Polygone *) realloc(this->polygones, 
			sizeof(Polygone) * (nPolygones + n));
	for(int i = 0; i < n; i ++) {
		this->polygones[nPolygones + i].copy(&polygones[i]);
	}
	nPolygones += n;
	computeCenter();
}

void Object::addSpheres(const Sphere* spheres, int n) {
	this->spheres = (Sphere *) realloc(this->spheres, 
			sizeof(Sphere) * (nSpheres + n));
	for(int i = 0; i < n; i ++) {
		this->spheres[nSpheres + i].copy(&spheres[i]);
	}
	nSpheres += n;
	computeCenter();
}

void Object::computeCenter() {
	center = Point(0, 0, 0);
	for(int i = 0; i < nPolygones; i ++) {
		center.add(&polygones[i].center);
	}
	for(int i = 0; i < nSpheres; i ++) {
		center.add(&spheres[i].center);
	}
	center.divide(nPolygones + nSpheres);
}

void Object::copy(const Object* object) {
	free(polygones);
	free(spheres);
	polygones = NULL;
	nPolygones = 0;
	addPolygones(object->polygones, object->nPolygones);
	spheres = NULL;
	nSpheres = 0;
	addSpheres(object->spheres, object->nSpheres);
}

