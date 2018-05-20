#include "DependencyHandler.h"
#include "EngineMain.h"
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


