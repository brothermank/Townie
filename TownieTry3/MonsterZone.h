#pragma once
#include "Zone.h"
#include "Monster.h"
#include <memory>
#include "Debugger.h"
#include "StringHelper.h"

class MonsterZone : public Zone {
public:
	vector<shared_ptr<Monster>> monsters;

	int targetPopulation;

	MonsterZone(int x, int y, int w, int h, shared_ptr<MapWindow> world, int targetPopulation = 3) : targetPopulation(targetPopulation), Zone(x, y, w, h, world) {
		Debugger::print("Created monster zone with pos:" + strh::toString(area.x) + "," + strh::toString(area.y) + "  size: " + strh::toString(area.w) + "," + strh::toString(area.h) + "\n");
	}

	void addMonsterTemplate(Monster m);

	void tick(double time);
	void onEnter(Entity e) {}
	void onExit(Entity e) {}
private:
	vector<shared_ptr<Monster>> monsterTemplates;

};