#include "RayTracing.h"

real screenDistance = 10;
real pixelsPerUnit = 100;
real maxView = 100;
int frameNumber = 0;
int msWaitTime = 10;
int ppr = 1;

bool shadowsOn = true;

std::atomic<int> pos;
extern std::atomic<int> working;

std::mutex coutM;
std::mutex iM;
extern std::mutex frameM;
int ipos = 0;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int NPIXELS;
extern color_t* pixels;
extern World* world;
extern bool QUIT;
extern int NFRAMES;
extern int NTHREADS;

Point direction = Point();
Point origin = Point();
Point right = Point();
Point up = Point();

bool traceRaysThreaded(const int nthreads) {
	pos = 0;
	
	direction.copy(&world->player->direction);
	origin.copy(&world->player->position);
	right.copy(&world->player->right);
	up.copy(&world->player->up);
	right.divide(screenDistance * pixelsPerUnit);
	up.divide(screenDistance * pixelsPerUnit);

	std::thread threads[nthreads];
	for(int i = 0; i < nthreads; i++) {
		threads[i] = std::thread(threadRayRoutine);
		std::cout << "created " << i << std::endl;
	}
	for(int i = 0; i < nthreads; i++) {
		while(!threads[i].joinable()) {
			std::cout << "cant join" << std::endl;	
		}
		threads[i].join();
	}
	std::cout << " all threads joined" << std::endl;
}

void nextFrame() {
	pos = 0;

	NFRAMES ++;

	std::cout << "Frame " << NFRAMES << std::endl;

	direction.copy(&world->player->direction);
	origin.copy(&world->player->position);
	right.copy(&world->player->right);
	up.copy(&world->player->up);
	right.divide(screenDistance * pixelsPerUnit);
	up.divide(screenDistance * pixelsPerUnit);
	
	frameM.unlock();
	
	while(working != NTHREADS) {
		std::this_thread::sleep_for(
				std::chrono::milliseconds(
					msWaitTime));
	}
	frameM.lock();
	while(working != 0) {
		std::this_thread::sleep_for(
				std::chrono::milliseconds(
					msWaitTime));
	}

}

inline int chunkPos(const KeyPoint* chunk) {
	return (chunk->x - world->center.x + world->chunksLoaded ) +
		       (chunk->y - world->center.y + world->chunksLoaded)
		       * (2 * world->chunksLoaded + 1) + 
		       (chunk->z - world->center.z + world->chunksLoaded)
		       * (2 * world->chunksLoaded + 1) 
		       * (2 * world->chunksLoaded + 1);
}

inline bool chunkLoaded(const KeyPoint* chunk) {
	return (abs(chunk->x - world->center.x) 
			<= world->chunksLoaded) &&
		(abs(chunk->y - world->center.y) 
		 	<= world->chunksLoaded) &&
		(abs(chunk->z - world->center.z) 
		 	<= world->chunksLoaded);
}

void threadRayRoutine() {
	Line ray = Line();
	Line sunRay = Line();
	Colision colision = Colision();
	Colision sunColision = Colision();
	int i;
	real ratio;
	coutM.lock();
	std::cout << world->totalChunksLoaded() << std::endl;
	coutM.unlock();
	bool* seenChunks = (bool *) malloc(world->totalChunksLoaded());


	while(1) {
		frameM.lock();
		frameM.unlock();
		if(QUIT) std::terminate();
		working++;

		sunRay.direction.copy(&world->sunDirection);
		sunRay.direction.multiply(-1);
		

		while(1) {
			i = pos.fetch_add(1);
			if(i >= NPIXELS/ppr/ppr) break;
			
			ray.origin.copy(&origin);
			ray.direction.copy(&direction);
			ray.direction.addMultiply(&up, i/(SCREEN_WIDTH/ppr) - (SCREEN_HEIGHT/ppr/2));
			ray.direction.addMultiply(&right, i%(SCREEN_WIDTH/ppr) - SCREEN_WIDTH/2/ppr);
			ray.direction.normalize();
			
			//Colision
			colision.point.copy(&origin);
			colision.point.addMultiply(&ray.direction, maxView);
			colision.found = false;
			
			traceRay(&ray, &colision, seenChunks);
			
			if(!colision.found) {
				colision.color = 0;
			} else {
			if(shadowsOn) {
			//Sunray
			sunRay.origin.copy(&colision.point);
			sunRay.origin.addMultiply(&sunRay.direction, 0.0001);
			
			//Suncolision
			sunColision.point.copy(&sunRay.origin);
			sunColision.point.addMultiply(&sunRay.direction, 100);
			sunColision.found = false;

			traceRay(&sunRay, &sunColision, seenChunks);


			if(!sunColision.found) {
				ratio = ( - world->sunDirection % &colision.normal );
				//ratio = ( - ray.direction % &colision.normal) *
				ratio = std::max(real(ratio * 0.8 + 0.2), real(0));
			} else {
				ratio = 0.2;
			}

			} else {
				ratio = ( - world->sunDirection % &colision.normal );// * ( - ray.direction % &colision.normal) ;
				ratio = std::max(real(ratio * 0.8 + 0.2), real(0));
			}
			
			colision.color = dimColor(colision.color, ratio);
			}
			int ppp = i%(SCREEN_WIDTH/ppr)*ppr +
				i/(SCREEN_WIDTH/ppr)
				*ppr*SCREEN_WIDTH;
			for(int ii = 0; ii < ppr; ii++) {
			for(int jj = 0; jj < ppr; jj++) {
				if((ppp + ii + jj*SCREEN_WIDTH)
						< NPIXELS) {
				pixels[ppp + ii + jj*SCREEN_WIDTH] 
					= colision.color;
				}
			}}
			/*
			for(int ii = 0; ii < ppr; ii++) {
			for(int jj = 0; jj < ppr; jj++) {
				if((i*ppr*ppr + ii + jj*SCREEN_WIDTH)
						< NPIXELS) {
				pixels[i*ppr*ppr + ii + jj*SCREEN_WIDTH] 
					= colision.color;
				}
			}}*/
		}
		working --;
	}
	coutM.lock();
	std::cout << " thread dying "  << i << std::endl;
	coutM.unlock();
}

bool traceRay(Line* ray, Colision* colision, bool* seenChunks) {
	KeyPoint chunk = KeyPoint();
	std::memset(seenChunks, 0x0000, world->totalChunksLoaded());
	world->getChunkOfPoint(&ray->origin, &chunk);
	while(!colision->found && world->isChunkLoaded(&chunk)) {	
		if(chunk.z < 0) {
			colision->point.copy(&ray->origin);
			colision->normal = Point(0, 0, 1);
			colision->found = true;
			colision->color = 0x7F7F7F7F;
			return true;
		}
		chunk.x -= 1;
		chunk.y -= 1;
		chunk.z -= 1;
		for(int i = 0; i < 3; i ++) {
		for(int j = 0; j < 3; j ++) {
		for(int k = 0; k < 3; k ++) {
			if(world->isChunkLoaded(&chunk)) {
			if(!seenChunks[chunkPos(&chunk)]) {
			seenChunks[chunkPos(&chunk)] = true;
			testChunkColision(ray, &chunk, colision);
			}}
		chunk.x += 1;
		}
		chunk.x -= 3;
		chunk.y += 1;
		}
		chunk.y -= 3;
		chunk.z += 1;
		}
		getNextChunk(ray, &chunk);
	}
	return true;	
}

bool testChunkColision(Line* ray, const KeyPoint* chunk, 
		Colision* colision) {	
	Colision tempColision = Colision();
	Chunk* tempChunk;
	Object* tempObject;
	Polygone* tempPolygone;
	Sphere* tempSphere;
	tempChunk = world->getChunkOfChunk(chunk);
	for(int i = 0; i < tempChunk->objectQuantity; i ++) {
		tempObject = &tempChunk->objects[i];
		for(int j = 0; j < tempChunk->objects[i].nPolygones; j ++) {
			
			tempPolygone = &tempObject->polygones[j];
			
			if(!testPolygoneColision(ray, tempPolygone, &tempColision)) continue;
			if(!tempColision.point.isBetween(&colision->point, &ray->origin)) continue;
			
			colision->point.copy(&tempColision.point);
			colision->found = true;
			colision->color = tempPolygone->color;
			colision->normal.copy(&tempColision.normal);
			
		}
		for(int j = 0; j < tempChunk->objects[i].nSpheres; j ++) {
			
			tempSphere = &tempObject->spheres[j];
			
			if(!testSphereColision(ray, tempSphere, &tempColision)) continue;
			if(!tempColision.point.isBetween(&colision->point, &ray->origin)) continue;
			
			colision->point.copy(&tempColision.point);
			colision->found = true;
			colision->color = tempSphere->color;
			colision->normal.copy(&tempColision.normal);
		}
	}
	return true;	
}

bool testPolygoneColision(const Line* ray, 
		const Polygone* polygone, Colision* colision) {
	
	if(ray->direction.isPerpendicular(&polygone->plane.normal)) {
		return false;
	}	

	real t = -((polygone->plane.d + 
		ray->origin.x * polygone->plane.normal.x +
		ray->origin.y * polygone->plane.normal.y +
		ray->origin.z * polygone->plane.normal.z) / 
		(ray->direction.x * polygone->plane.normal.x +
		 ray->direction.y * polygone->plane.normal.y +
		 ray->direction.z * polygone->plane.normal.z));

	colision->point.copy(&ray->origin);
	colision->point.addMultiply(&ray->direction, t);

	#define i1 polygone->i1
	#define i2 polygone->i2
	#define p colision->point
	#define v polygone->points
	
	real t1 = p[i1] - v[0][i1];
	real t2 = p[i2] - v[0][i2];

	for(int i = 0; i < polygone->n - 2; i ++) {
		
		bool c1 = (((v[i + 1][i1] - v[0][i1]) * t2) 
				- ((v[i + 1][i2] - v[0][i2]) * t1)) 
			> 0;

		if(((((v[i + 2][i1] - v[0][i1]) * t2)
			- ((v[i + 2][i2] - v[0][i2]) * t1)) > 0)
			== c1) {
			continue;
		}
		if(((((v[i + 2][i1] - v[i + 1][i1]) * 
				(p[i2] - v[i + 1][i2])) 
			- ((v[i + 2][i2] - v[i + 1][i2]) * 
				(p[i1] - v[i + 1][i1]))) > 0)
			!= c1) {
			continue;
		}
		colision->normal.copy(&polygone->plane.normal);
		return true;
	}	
	return false;

	#undef i1
	#undef i2
	#undef p 
	#undef v

}

bool testSphereColision(const Line* ray, 
		const Sphere* sphere, Colision* colision) {
	
	real t = sphere->center.x * ray->direction.x 
		+ sphere->center.y * ray->direction.y 
		+ sphere->center.z * ray->direction.z
		- ray->origin.x * ray->direction.x 
		- ray->origin.y * ray->direction.y 
		- ray->origin.z * ray->direction.z;
	
	colision->point.copy(&ray->origin);
	colision->point.addMultiply(&ray->direction, t);
	
	real distance = colision->point.distanceToPoint(
			&sphere->center);

	if(distance > sphere->radius) return false;
	
	real b = std::sqrt(sphere->radius * sphere->radius 
			- distance * distance);

	if(ray->origin.distanceToPoint(&sphere->center) 
			> sphere->radius) 
		b *= -1;
	colision->point.addMultiply(&ray->direction, b);
	colision->normal.copy(&colision->point);
	colision->normal.substract(&sphere->center);
	colision->normal.divide(sphere->radius);
	return true;
}

real getNextCoord(real x, real dx) {
	if(dx == 0) {
		return 2 * world->chunkSize;
	} else if(dx > 0) {
		if(x >= 0) {
			return (world->chunkSize - fmod(x, world->chunkSize)) / dx;
		} else {
			return ( - fmod(x, world->chunkSize)) / dx;
		}
	} else {
		if(x <= 0) {
			return - (world->chunkSize + fmod(x, world->chunkSize)) / dx;
		} else {
			return ( - fmod(x, world->chunkSize)) / dx;
		}
	}
}

bool getNextChunk(Line* ray, KeyPoint* chunk) {
	real xleft, yleft, zleft, minDir;
	xleft = getNextCoord(ray->origin.x, ray->direction.x);
	yleft = getNextCoord(ray->origin.y, ray->direction.y);
	zleft = getNextCoord(ray->origin.z, ray->direction.z);
	if(xleft == 0) xleft = world->chunkSize;
	if(yleft == 0) yleft = world->chunkSize;
	if(zleft == 0) zleft = world->chunkSize;
	minDir = std::min(xleft, std::min(yleft, zleft));
	if(xleft < yleft) {
		if(xleft < zleft) {
			ray->origin.addMultiply(&ray->direction, xleft);
			world->getChunkOfPoint(&ray->origin, chunk);
			if(ray->direction.x < 0) chunk->x -= 1;
		} else {
			ray->origin.addMultiply(&ray->direction, zleft);
			world->getChunkOfPoint(&ray->origin, chunk);
			if(ray->direction.z < 0) chunk->z -= 1;
		}

	} else {
		if(yleft < zleft) {
			ray->origin.addMultiply(&ray->direction, yleft);
			world->getChunkOfPoint(&ray->origin, chunk);
			if(ray->direction.y < 0) chunk->y -= 1;
		} else {
			ray->origin.addMultiply(&ray->direction, zleft);
			world->getChunkOfPoint(&ray->origin, chunk);
			if(ray->direction.z < 0) chunk->z -= 1;
		}
	}
}

inline color_t dimColor(color_t c, real r) {
	return    (color_t((c & 0x000000FF) * r) & 0x000000FF)
		| (color_t((c & 0x0000FF00) * r) & 0x0000FF00) 
		| (color_t((c & 0x00FF0000) * r) & 0x00FF0000) 
		| (color_t((c & 0xFF000000) * r) & 0xFF000000);
}

