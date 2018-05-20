#pragma once
#include "Map.h"
#include "Item.h"
#include "Hero.h"
#include "Monster.h"
#include "Zone.h"


class Scenario {
public:
	Scenario() {}
	Scenario(shared_ptr<Map> map) : map(map) {}

	void loadScenario(string scenarioName);
	
	void update(double dTime);
	void UpdateEntities(double dTime);
	void UpdateZones(double time);

	void registerEntity(shared_ptr<Entity> e);
	void registerStructure(shared_ptr<Structure> e);
	void registerZone(shared_ptr<Zone> e);
	void removeEntity(Entity * e);
	shared_ptr<Monster> spawnMonster(shared_ptr<Entity> templ, int x, int y);
	shared_ptr<Monster> spawnMonster(shared_ptr<Entity> templ, Rectangle area);
	shared_ptr<Hero> spawnHero(shared_ptr<Entity> templ, Rectangle area, shared_ptr<Weapon> startWeapon);
	void spawnHeroes(shared_ptr<Entity> templ, Rectangle area, shared_ptr<Weapon> startWeapon, double amount = 1);

	vector<shared_ptr<Hero>> getHeroes();
	vector<shared_ptr<Monster>> getMonsters();
	vector<shared_ptr<Store>> getStores();

	void saveMap();
	shared_ptr<Map> getMap();

	bool entityAt(int x, int y);

	vector<shared_ptr<Entity>> entities; //Should use int/ID based hashmaps for faster removal (instead of searching through entire vector)
	vector<shared_ptr<Structure>> structures;
	vector<shared_ptr<Zone>> zones;

private:
	shared_ptr<Map> map;
};