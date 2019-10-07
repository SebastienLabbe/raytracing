#ifndef CHUNK_H
#define CHUNK_H

#include "Header.h"

#include "Point.h"

class Object;

class Chunk {
	public:
		Object* objects = NULL;
		int objectQuantity = 0;
		Chunk() = default;
		void addObjects(const Object* objects, int n);
		
};

#endif
