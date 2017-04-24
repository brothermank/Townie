#pragma once
#include "Zone.h"

class TownZone : Zone {
public:
	void tick(double time);
	void onEnter(Entity e);
	void onExit(Entity e) = 0;


};