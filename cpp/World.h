#ifndef WORLD_H
#define WORLD_H

#include "Header.h"

#include "Point.h"
#include "KeyPoint.h"
#include "Chunk.h"

class Object;
class Player;

class World {
	public:
		std::string name = "temp save";
		int chunksRendered = 2,
		    chunksLoaded = 2,
		    chunkSize = 10;
		Point sunDirection;
	       	KeyPoint center;
		Chunk** chunks = NULL;
		Player* player = NULL;
	
		//Constructors
		World();

		//Getters
		void getChunkOfPoint(const Point* point, 
				KeyPoint* chunk) const;
		Chunk* getChunkOfPoint(const Point* point) const;
		Chunk* getChunkOfChunk(const KeyPoint* chunk) const;
		bool isChunkLoaded(const KeyPoint* chunk) const;
		int getChunkPos(const KeyPoint* chunk) const;
		int totalChunksLoaded() const;
		
		//Setters
		void addObject(const Object* object);
		void addPlayer(Player* player);
		void loadAroundPlayer();
		
		friend std::ostream &operator<<(std::ostream &os, 
				const World &w) {
			for(int i = 0; i < w.totalChunksLoaded(); 
					i ++) {
				os << " i   " << i << std::endl 
					<< w.chunks[i]->objectQuantity 
					<< std::endl;
			}
			return os;
		}
};

#endif
