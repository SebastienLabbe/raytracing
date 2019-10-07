#include "Chunk.h"
#include "Object.h"


void Chunk::addObjects(const Object* objects, int n) {
	this->objects = (Object *) realloc(this->objects, 
			sizeof(Object) * (objectQuantity + n));

	for(int i = 0; i < n; i++) {
		this->objects[objectQuantity + i] = Object();
		this->objects[objectQuantity + i].copy(&objects[i]);
	}
	objectQuantity += n;
}
