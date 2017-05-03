#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include "EngineMain.h"
#include <time.h>
#include "Debugger.h"
#include "Navigator.h"

//Key press surfaces constants
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};
enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* randomImg = NULL;
//Loads individual image as texture

//The window renderer

//Current displayed texture
SDL_Texture* gTexture = NULL;

bool initateDependencies() {
	srand((long)time);

	bool success = false;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else {
		int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
		if (!(IMG_Init(imgFlags) & imgFlags)) {
			printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			success = false;
		}
	}

	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	return success;
}
void stopDependencies() {
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool Navigator::flags[40][40] = { false };
Node Navigator::referenceNodes[40][40] = { Node() };


void initNavigator() {
	for (size_t y = 0; y < 40; y++) {
		for (size_t x = 0; x < 40; x++) {
			Navigator::referenceNodes[y][x].pos = Vector2ST(x, y);
		}
	}
}

int main(int argc, char ** argv)
{
	initDebug();
	initNavigator();
	initateDependencies();
	Game g = Game::Game();
	int i = 2;
	g.start();
	stopDependencies();
	return 0;
}


