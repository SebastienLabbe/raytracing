#include "SDL/SDL.h"
#include <string>
#include <iostream>
#include "RayTracing.h"

extern const int NTHREADS = 4;
std::atomic<int> working;
extern const int SCREEN_WIDTH = 1000;
extern const int SCREEN_HEIGHT = 600;
extern const int SCREEN_BPP = 32;
extern const int NPIXELS = SCREEN_HEIGHT * SCREEN_WIDTH;

std::mutex frameM;
int NFRAMES = 0;

SDL_Surface *screen = NULL;
color_t *pixels = NULL;
SDL_Event event;
bool QUIT = false;

World* world = NULL;



int main( int argc, char* args[] )
{
	SDL_Init( SDL_INIT_EVERYTHING );
	
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_DOUBLEBUF | SDL_SWSURFACE );
	
	SDL_WM_SetCaption( "Amazing ray tracing", NULL );	
	
	pixels = (Uint32 *) screen->pixels;
	
	world = new World();
	
	createFromFile("data/first_world.txt", world);

	Player* player = new Player();
	Point center = Point(0, 0, 2);
	real distanceT = 15;
	real angle = 0;
	
	world->addPlayer(player);

	frameM.lock();
	working = 0;

	std::thread threads[NTHREADS];
	
	for(int i = 0; i < NTHREADS; i++) {
		threads[i] = std::thread(threadRayRoutine);
	}
	auto start =  std::chrono::system_clock::now();
	while(1) {
		if(!(NFRAMES %10)) start =  std::chrono::system_clock::now();
		player->lookAt(&center);
		player->moveTo(cos(angle) * distanceT, sin(angle) * distanceT, 5); 
		
		angle += 0.1;

		nextFrame();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		
		SDL_Flip( screen );
		
		while( SDL_PollEvent( &event ) ) {
			if(event.type == SDL_QUIT) {
				QUIT = true;
			}
		}
		
		/*
		if(QUIT) {
			for(int i = 0; i < NTHREADS; i++) {
				threads[i].join();
			}

		}
		*/
		if(NFRAMES %10) continue; 
	 	std::chrono::duration<double> diff = 
			std::chrono::system_clock::now()-start;
		std::cout << diff.count()/10.0 << std::endl;
	}
		
	SDL_Quit();
    
	return 0;    
}
