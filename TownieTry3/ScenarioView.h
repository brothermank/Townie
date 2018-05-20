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
#include "ResourceHandler.h"

class Scenario;

class ScenarioView : public Window {
public:
	ScenarioView(SDL_Window * window, SDL_Renderer * renderer, ResourceHandler* resourceHandler);

	Vector2D wpos;
	double zoom;
	
	Vector2D screnPosToMapPos(Vector2D pos);
	Vector2D mapPosToScreenPos(Vector2D pos);
	
	bool ReceiveClick(Vector2D pos, Uint32 mask, bool buttonDown);
	void ReceiveHotkeyInput(SDL_Event e);
	void changePaintSize(bool increment);

	void setScenario(Scenario * s);

	//Rendermethods
	void redraw();
	void renderEntity(shared_ptr<Entity> e);
	void DrawEnvironment();
	void DrawEntities();

	int paintingKey = 1;

	shared_ptr<GTextField> frameRate;

private:
	Scenario* scenario;
	bool heldLeft = false;
	bool heldRight = false;
	shared_ptr<GTextField> saveText;
	shared_ptr<GTextField> heroMoney;
	shared_ptr<GTextField> monsterHealth;
	shared_ptr<GTextField> heroHealth;

	ResourceHandler* resourceHandler;


	int paintSize = 2;
};