#pragma once
#include "Zone.h"
#include <vector>
#include <memory>

class SpawnZone : Zone {
public:
	void spawn();
	vector<Entity> spawnTemplates;
	vector<shared_ptr<Entity>> entitites;

	void tick(double time);
	void onEnter(Entity e);
	void onExit(Entity e) = 0;

private:
	double spawnChance;
};