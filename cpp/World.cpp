#include "World.h"
#include "Point.h"
#include "Object.h"
#include "Player.h"



World::World() {
	name = "temp save";
	chunksRendered = 2;
	chunksLoaded = 2;
	chunkSize = 10;
	sunDirection = Point(10, 5, -3);
	sunDirection.normalize();
	center = KeyPoint();
	player = NULL;
	chunks = (Chunk **) malloc(sizeof(Chunk*) 
			* totalChunksLoaded());
	for(int i = 0; i < totalChunksLoaded(); i++) {
		chunks[i] = new Chunk();
	}
}

void World::getChunkOfPoint(const Point* point, 
		KeyPoint* chunk) const {
	chunk->x = int(floor(point->x / chunkSize));
	chunk->y = int(floor(point->y / chunkSize));
	chunk->z = int(floor(point->z / chunkSize));
}

void World::addObject(const Object* object) {
	KeyPoint chunk = KeyPoint();
	getChunkOfPoint(&object->center, &chunk);
	//std::cout << chunk << std::endl;
	if(isChunkLoaded(&chunk)) {
		getChunkOfPoint(&object->center)->addObjects(object, 1);
	}
}

void World::addPlayer(Player* player) {
	free(player);
	this->player = (Player *) malloc(sizeof(Player));
	this->player->copy(player);
}

Chunk* World::getChunkOfPoint(const Point* point) const{
	KeyPoint chunk = KeyPoint();
	getChunkOfPoint(point, &chunk);
	return getChunkOfChunk(&chunk);
}

Chunk* World::getChunkOfChunk(const KeyPoint* chunk) const{
	return chunks[getChunkPos(chunk)];
}

bool World::isChunkLoaded(const KeyPoint* chunk) const{
	return (abs(chunk->x - center.x) 
			<= chunksLoaded) &&
		(abs(chunk->y - center.y) 
		 	<= chunksLoaded) &&
		(abs(chunk->z - center.z) 
		 	<= chunksLoaded);
}

inline int World::getChunkPos(const KeyPoint* chunk) const{
	return (chunk->x - center.x + chunksLoaded ) +
		       (chunk->y - center.y + chunksLoaded)
		       * (2 * chunksLoaded + 1) + 
		       (chunk->z - center.z + chunksLoaded)
		       * (2 * chunksLoaded + 1) 
		       * (2 * chunksLoaded + 1);
}

int World::totalChunksLoaded() const {
	return (2 * chunksLoaded + 1) *
		(2 * chunksLoaded + 1) *
		(2 * chunksLoaded + 1);
}

