#pragma once
#include "Navigator.h"
#include <stdlib.h>
#include "Item.h"
#include <memory>
#include "WObject.h"
#include "Debugger.h"

#define EntityHeight 70

class MapWindow;

enum EntityT { Undefined = 0, Hero_t = 1, Monster_t = 2 };
enum ActionT { Idle, Attack, EnterBuilding };

class Entity : public WObject {
public:
	Entity() :speed(1), path(), type(Undefined), WObject(Vector2(0, 0), EntityHeight) {}
	Entity(LTexture text, MapWindow * map, double speed = 1, Vector2 pos = Vector2(0, 0), double imgSize = EntityHeight, EntityT t = Undefined);
	Entity(SDL_Renderer * rend, string textureType, string name, MapWindow * map, double speed = 1, Vector2 pos = Vector2(0, 0), double imgSize = EntityHeight, EntityT t = Undefined);

	virtual void update(double dTime);
	Entity copyEntity();
	EntityT type;

	virtual void render(MapWindow* world, double dTime = 0);

	int RecieveAttack(Entity * e);
	void changeMoney(double amount) { money += amount; }
	double getMoney() { return money; }
	double getDmg() { return dmg; }
	double getHealth() { return health; }
	void addItem(shared_ptr<Item> i) {}
	void removeItem(shared_ptr<Item> i) {}


	double speed;
	Path path;
	Navigator n;

protected:
	double money, dmg, health, range = 0.2;
	void FollowPath();
	ActionT action;
};

