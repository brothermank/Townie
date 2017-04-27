#define frameCap 100000

#include "EngineMain.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include "Debugger.h"
#include "Timer.h"
#include "Hero.h"
#include "Monster.h"
#include <memory>
#include "GUI.h"
#include "Store.h"
//#include <SDL_ttf.h>
double dTime;
Uint32 timeLast = 0;

Game::Game() {
	initGame();
	shared_ptr<MapWindow> temp(new MapWindow(gWindow, gRenderer));
	currentWindow = temp;
	loadAllEntities();
	loadAllStructures();
	loadAllItems();
	loadAllTiles();
	currentWindow = make_shared<MapWindow>(MapWindow(gWindow, gRenderer, Map(tiles, "Placeholder", "Placeholder")));
	//currentWindow->registerEntity(Hero(entityTemplates[0].copy()));

	MonsterZone mz = MonsterZone(0, 0, 5, 5, currentWindow.get(), 3);
	mz.addMonsterTemplate(Monster(entityTemplates[1], 10, 3, 1, 0.2));
	currentWindow->registerZone(make_shared<MonsterZone>(mz));

	//currentWindow->registerEntity(make_shared<Monster>(entityTemplates[1].copyEntity(), 10));
	currentWindow->registerEntity(make_shared<Hero>(entityTemplates[0].copyEntity(), 10));
	currentWindow->registerStructure(make_shared<Store>(structTemplates[0].copy()));
	currentWindow->structures[0]->SetPosition(3, 3, currentWindow->getMap());

	vector<Weapon::Event> points;
	points.push_back(Weapon::Event(false, 0.1, 0, 100));
	points.push_back(Weapon::Event(true, 0.3, 100, 90));
	points.push_back(Weapon::Event(false, 0.2, 100, 0));

	Weapon::Animation slash = Weapon::Animation(points);
	shared_ptr<Weapon> sword = make_shared<Weapon>(Weapon(itemTemplates[0], slash));
	shared_ptr<Weapon> fists = make_shared<Weapon>(Weapon(itemTemplates[1], slash));
	dynamic_pointer_cast<Store>(currentWindow->structures[0])->items.push_back(sword);
	currentWindow->getHeroes()[0]->weapon = fists;

	currentWindow->getHeroes()[0]->changeMoney(100);

	currentWindow->guiElements.push_back(shared_ptr<GButtonSelectTile>(new GButtonSelectTile(currentWindow, gRenderer, textures[0], 0, Vector2I(1560, 20), Vector2I(150, 75))));
	currentWindow->guiElements.push_back(shared_ptr<GButtonSelectTile>(new GButtonSelectTile(currentWindow, gRenderer, textures[1], 1, Vector2I(1730, 20), Vector2I(150, 75))));
	currentWindow->guiElements.push_back(shared_ptr<GButtonSelectTile>(new GButtonSelectTile(currentWindow, gRenderer, textures[2], 2, Vector2I(1560, 115), Vector2I(150, 75))));
	currentWindow->guiElements.push_back(shared_ptr<GButtonSelectTile>(new GButtonSelectTile(currentWindow, gRenderer, textures[3], 3, Vector2I(1730, 115), Vector2I(150, 75))));
	currentWindow->guiElements.push_back(shared_ptr<GButtonSelectTile>(new GButtonSelectTile(currentWindow, gRenderer, textures[4], 4, Vector2I(1560, 200), Vector2I(150, 75))));
	currentWindow->guiElements.push_back(shared_ptr<GButtonSelectBrushSize>(new GButtonSelectBrushSize(currentWindow, gRenderer, textures[4], false, Vector2I(1560, 400), Vector2I(150, 150))));
	currentWindow->guiElements.push_back(shared_ptr<GButtonSelectBrushSize>(new GButtonSelectBrushSize(currentWindow, gRenderer, textures[4], true, Vector2I(1730, 400), Vector2I(150, 150))));


	Debugger::print("Rectangle in initialization " + strh::toString(currentWindow->zones[0]->area.x) + "," + strh::toString(currentWindow->zones[0]->area.y)
		+ " - is within space " + strh::toString(currentWindow->zones[0]->area.w) + "," + strh::toString(currentWindow->zones[0]->area.h) + " rectangle at " + strh::toString(&(currentWindow->zones[0]->area.x)) + "\n");

	currentWindow->zoom = 0.7;
	currentWindow->wpos = Vector2D(2, -4);

}

void Game::start() {
	//currentWindow->monsters[0].pos = Vector2(2, 2);
	//saveLevel();
	mainLoop();
}
bool Game::initGame() {
	bool success = true;

	gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
	}
	else {
		gRenderer = SDL_CreateRenderer(&*gWindow, -1, SDL_RENDERER_ACCELERATED);
		if (gRenderer == NULL) {
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		}
		gScreenSurface = SDL_GetWindowSurface(&*gWindow);
	}
	LoadFont("Raleway-Regular");
	return success;
}
void Game::closeGame() {

	//Free loaded image
	int textureCount = textures.size();
	for (int i = textureCount - 1; i > 0; i--) {
		SDL_DestroyTexture(textures[i].mTexture);
	}
	textures.erase(textures.begin(), textures.end());

	//Destroy window    
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

}

void Game::mainLoop() {
	Debugger::print("Rectangle in main loop " + strh::toString(currentWindow->zones[0]->area.x) + "," + strh::toString(currentWindow->zones[0]->area.y)
		+ " - is within space " + strh::toString(currentWindow->zones[0]->area.w) + "," + strh::toString(currentWindow->zones[0]->area.h) + " rectangle at " + strh::toString(&(currentWindow->zones[0]->area.x)) + "\n");
	bool quit = false;
	SDL_Event e;
	SDL_StartTextInput();

	timeLast = SDL_GetTicks();
	double timer1 = 0;
	double time1sec = 0;
	int frames = 0;
	int frameRateCount = 0;

	bool heldClick = false;

	while (!quit) {
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN) {
				currentWindow->ReceiveHotkeyInput(e);
			}
			else if (e.type == SDL_TEXTINPUT) {
				currentWindow->ReceiveTextInput(e);
			}
			//TODO: Transfer handle to mapWindow
			else if (e.type == SDL_MOUSEWHEEL) {
				currentWindow->zoom *= (pow(1.1, e.wheel.y));
			}
		}

		//Handle clicks
		int *x, *y;
		int a1 = 0, a2 = 0;
		x = &a1;
		y = &a2;
		Uint32 bmap = SDL_GetMouseState(x, y);
		currentWindow->ReceiveClick(Vector2D(*x, *y), bmap, heldClick);
		if (bmap & SDL_BUTTON(SDL_BUTTON_LEFT) && !heldClick) {
			Debugger::print("Click\n");
			heldClick = true;
		}
		else if (!(bmap & SDL_BUTTON(SDL_BUTTON_LEFT)) && heldClick) {
			Debugger::print("Unclick\n");
			heldClick = false;
		}

		//Update window
		currentWindow->update(dTime);

		//Timer handling
		dTime = 0;
		while (dTime < 1.0 / frameCap) {
			dTime = (SDL_GetTicks() - timeLast) * 0.001;
		}
		timeLast += (Uint32)(dTime * 1000);
		timer1 += dTime;
		time1sec += dTime;
		frames++;
		frameRateCount++;
		if (time1sec > 1.0) {
			currentWindow->frameRate->setText("Frame Rate: " + strh::toString(frameRateCount));
			time1sec -= 1;
			frameRateCount = 0;
		}
		

	}

	closeGame();
}

void Game::loadAllTiles() {
	textures.push_back(LTexture(gRenderer, "Texture", "Black"));
	textures.push_back(LTexture(gRenderer, "Texture", "Grass1"));
	textures.push_back(LTexture(gRenderer, "Texture", "Road1"));
	textures.push_back(LTexture(gRenderer, "Texture", "Road2"));
	textures.push_back(LTexture(gRenderer, "Texture", "Road3"));

	tiles.push_back(Tile(&textures[0], 100));
	tiles.push_back(Tile(&textures[1], 4));
	tiles.push_back(Tile(&textures[2]));
	tiles.push_back(Tile(&textures[3]));
	tiles.push_back(Tile(&textures[4]));

	//Test tile - no custom graphics
	bool temp1[4] = { true, false, true, false };
	bool temp2[4] = { false, true, false, true };
	bool temp3[4] = { false, false, false, false };
	bool temp4[4] = { false, false, true, false };
	tiles.push_back(Tile(&textures[0], 0.5f, temp4));
}
void Game::loadAllEntities() {
	entityTemplates.push_back(Entity(gRenderer, "Character", "Viking", &*currentWindow, 1, Vector2D(0, 0), 70, Hero_t));
	entityTemplates.push_back(Entity(gRenderer, "Character", "Tentacle2", &*currentWindow, 1, Vector2D(0, 0), 70, Monster_t));

}
void Game::loadAllStructures() {
	bool temp1[4] = { false, false, false, true };
	structTemplates.push_back(Structure(gRenderer, "Structure", "HouseNew", Store_t, Vector2D(0, 0), 0.2f, 300, temp1, true));
}
void Game::loadAllItems() {
	itemTemplates.push_back(Item(gRenderer, "Item", "Sword", Item::Undefined, Vector2D(0.5, 1), 50));
	itemTemplates.push_back(Item(gRenderer, "Item", "Blank", Item::Undefined, Vector2D(0.5, 1), 50));
}

void Game::saveLevel() {
	currentWindow->saveMap();
}