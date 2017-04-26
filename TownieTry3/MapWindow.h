#pragma once
#define tilew (400 * zoom)
#define tileh (200 * zoom)
#define scrollSpeed 0.4f
#define totalTextures 5

#include <SDL.h>
#include <SDL_render.h>
#include "Map.h"
#include "Monster.h"
#include "Hero.h"
#include <memory>
#include "Window.h"
#include "Structure.h"
#include "MonsterZone.h"

class MapWindow : public Window {
public:
	MapWindow(SDL_Window * window, SDL_Renderer * renderer);
	MapWindow(SDL_Window * window, SDL_Renderer * renderer, Map m);
	Vector2D wpos;
	double zoom;
	void DrawMap();
	void saveMap();
	void DrawEntities(double dTime);
	void UpdateEntities(double dTime);
	void UpdateZones(double time);
	Vector2D screnPosToMapPos(Vector2D pos);
	Vector2D mapPosToScreenPos(Vector2D pos);
	Map * getMap();
	void update(double dTime);
	bool ReceiveClick(Vector2D pos, Uint32 mask, bool buttonDown);
	void ReceiveHotkeyInput(SDL_Event e);
	void changePaintSize(bool increment);

	void registerEntity(shared_ptr<Entity> e);
	void registerStructure(shared_ptr<Structure> e);
	void registerZone(shared_ptr<Zone> e);
	void removeEntity(Entity * e);
	shared_ptr<Monster> spawnMonster(shared_ptr<Entity> templ, int x, int y);
	shared_ptr<Monster> spawnMonster(shared_ptr<Entity> templ, Rectangle area);

	vector<shared_ptr<Hero>> getHeroes();
	vector<shared_ptr<Monster>> getMonsters();
	vector<shared_ptr<Store>> getStores();

	bool entityAt(int x, int y);

	vector<shared_ptr<Entity>> entities; //Should use int/ID based hashmaps for faster removal (instead of searching through entire vector)
	vector<shared_ptr<Structure>> structures;
	vector<shared_ptr<Zone>> zones;
	int paintingKey = 1;

private:
	Map map;
	bool heldLeft = false;
	bool heldRight = false;
	shared_ptr<GTextField> saveText;
	shared_ptr<GTextField> heroMoney;
	shared_ptr<GTextField> monsterHealth;
	shared_ptr<GTextField> heroHealth;

	int paintSize = 2;
};