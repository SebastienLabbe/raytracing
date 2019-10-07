all: main

main: main.cpp Header.h point line plane object chunk world colision player polygone sphere keypoint worldcreation raytracing
	g++ -O3 -o program main.cpp Point.o Line.o Plane.o Object.o Chunk.o World.o Colision.o Player.o Polygone.o Sphere.o KeyPoint.o WorldCreation.o RayTracing.o -lSDL -pthread

clean:
	rm program *.o

run: main
	./program

#CLASSES = Point Line Plane Object Chunk World Colision Player Polygone Sphere KeyPoint WorldCreation RayTracing

point: Point.cpp Point.h
	g++ -O3 -c Point.cpp

line: Line.cpp Line.h
	g++ -O3 -c Line.cpp

plane: Plane.cpp Plane.h
	g++ -O3 -c Plane.cpp

object: Object.cpp Object.h
	g++ -O3 -c Object.cpp

chunk: Chunk.cpp Chunk.h
	g++ -O3 -c Chunk.cpp

world: World.cpp World.h
	g++ -O3 -c World.cpp

colision: Colision.cpp Colision.h
	g++ -O3 -c Colision.cpp

player: Player.cpp Player.h
	g++ -O3 -c Player.cpp

polygone: Polygone.cpp Polygone.h
	g++ -O3 -c Polygone.cpp

sphere: Sphere.cpp Sphere.h
	g++ -O3 -c Sphere.cpp

keypoint: KeyPoint.cpp KeyPoint.h
	g++ -O3 -c KeyPoint.cpp

worldcreation: WorldCreation.cpp WorldCreation.h
	g++ -O3 -c WorldCreation.cpp

raytracing: RayTracing.cpp RayTracing.h
	g++ -O3 -c RayTracing.cpp
