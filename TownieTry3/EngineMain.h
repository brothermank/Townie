#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "MapWindow.h"


#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768

using namespace std;

class Game {
public:
	Game();
	void start();
	bool initGame();
	void closeGame();
	void mainLoop();
	void loadAllTiles();
	void loadAllEntities();
	void loadAllStructures();
	void loadAllItems();
	void loadLevel(ScenarioView window);
	void loadLevel(Map m);
	void loadLevel();
	void saveLevel();
	vector<LTexture> textures;
	vector<Tile> tiles;
	vector<Entity> entityTemplates;
	vector<Structure> structTemplates;
	vector<Item> itemTemplates;

private:
	SDL_Window * gWindow;
	SDL_Surface* gScreenSurface = NULL;
	SDL_Renderer* gRenderer = NULL;
	shared_ptr<ScenarioView> currentWindow;
	shared_ptr<Map> map;

};
