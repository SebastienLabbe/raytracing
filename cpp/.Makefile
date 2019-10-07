all: main

main: main.cpp Header.h point line plane triangle object chunk world camera colision player
	g++ -o program main.cpp Point.o Line.o Plane.o Triangle.o Object.o Chunk.o World.o Camera.o Colision.o Player.o -lSDL

clean:
	rm program *.o test

run: main
	./program

#classes = Point Line Plane Triangle Object Chunk World Camera Colision Player

#CLASSES_START

player: Player.cpp Player.h
	g++ -c Player.cpp

line: Line.cpp Line.h
	g++ -c Line.cpp

triangle: Triangle.cpp Triangle.h
	g++ -c Triangle.cpp

object: Object.cpp Object.h
	g++ -c Object.cpp

chunk: Chunk.cpp Chunk.h
	g++ -c Chunk.cpp

world: World.cpp World.h
	g++ -c World.cpp

point: Point.cpp Point.h
	g++ -c Point.cpp

plane: Plane.cpp Plane.h
	g++ -c Plane.cpp

colision: Colision.cpp Colision.h
	g++ -c Colision.cpp

camera: Camera.cpp Camera.h
	g++ -c Camera.cpp

#CLASSES_END

test: test.cpp main
	g++ -o test test.cpp Point.o Plane.o Colision.o Line.o Triangle.o Object.o Chunk.o World.o Camera.o
	./test
