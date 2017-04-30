#pragma once
#include "Entity.h"
#include "Rectangle.h"

class MapWindow;

class Zone {
public:
	virtual void tick(double time) = 0;
	virtual void onEnter(Entity e) = 0;
	virtual void onExit(Entity e) = 0;

	Rectangle area;
	shared_ptr<MapWindow> world;

	Zone(int x, int y, int w, int h, shared_ptr<MapWindow> world) : area(x, y, w, h) {
		this->world = world;
	}

};