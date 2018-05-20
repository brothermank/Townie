#include "Zone.h"
#include "ScenarioView.h"

Zone::Zone(int x, int y, int w, int h, shared_ptr<ScenarioView> world) : area(x, y, w, h) {
	this->world = world;
	/*if (x + w >= world->getMap()->sizex) area.w = world->getMap()->sizex - x - 1;
	if (y + h >= world->getMap()->sizey) area.h = world->getMap()->sizey - y - 1;*/
}