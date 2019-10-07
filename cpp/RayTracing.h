#include "Point.h"
#include "Line.h"
#include "Plane.h"
#include "Object.h"
#include "Chunk.h"
#include "World.h"
#include "Player.h"
#include "Colision.h"
#include "Sphere.h"
#include "Polygone.h"
#include "KeyPoint.h"
#include "WorldCreation.h"

bool traceRaysThreaded(const int nthreads);

void nextFrame();

inline bool getBit(void* a, int n);

void threadRayRoutine();

bool traceRays(color_t* buffer, const int width, const int height);

bool traceRay(Line* ray, Colision* colision, bool* seenChunks);

bool testChunkColision(Line* ray, const KeyPoint* chunk, Colision* colision);

bool testPolygoneColision(const Line* ray, const Polygone* polygone, Colision* colision);

bool testSphereColision(const Line* ray, const Sphere* sphere, Colision* colision);

real getNextCoord(real x, real dx);

bool getNextChunk(Line* ray, KeyPoint* chunk);

color_t dimColor(color_t c, real r);
