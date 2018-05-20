#include "Scenario.h"
#define ScenarioFileEnding ".scen"

void Scenario::loadScenario(string scenarioName) {
	
}

shared_ptr<Map> Scenario::getMap() {
	return map;
}

void Scenario::saveMap() {
	map->save();
}

void Scenario::UpdateEntities(double dTime) {
	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->update(dTime);
	}
}
void Scenario::UpdateZones(double time) {
	for (size_t i = 0; i < zones.size(); i++) {
		zones[i]->tick(time);
	}
}
void Scenario::registerEntity(shared_ptr<Entity> e) {
	entities.push_back(e);
}
void Scenario::registerStructure(shared_ptr<Structure> e) {
	structures.push_back(e);
	e->SetPosition((size_t)e->pos.x, (size_t)e->pos.y, map);
}
void Scenario::registerZone(shared_ptr<Zone> e) {
	zones.push_back(e);
}

void Scenario::removeEntity(Entity * e) {
	for (size_t i = 0; i < entities.size(); i++) {
		if (entities[i]->id == e->id) {
			entities.erase(entities.begin() + i);
		}
	}
}

shared_ptr<Monster> Scenario::spawnMonster(shared_ptr<Entity> templ, int x, int y) {
	if (map->isSafeSafe(x, y)) {
		return NULL;
	}
	shared_ptr<Monster> e = make_shared<Monster>(templ->copyEntity());
	e->setPos(Vector2D(x, y));
	registerEntity(e);
	return e;
}
shared_ptr<Monster> Scenario::spawnMonster(shared_ptr<Entity> templ, Rectangle area) {
	shared_ptr<Monster> e = make_shared<Monster>(templ->copyEntity());
	int x = (int)(area.x + rand() * 1.0 / RAND_MAX * area.w);
	int y = (int)(area.y + rand() * 1.0 / RAND_MAX * area.h);
	int i = 0;
	double q = 0;
	double r = 0;
	while (map->isSafeSafe(x, y) || entityAt(x, y)) {
		q = rand() * 1.0 / RAND_MAX * (area.w - 0.001);
		r = rand() * 1.0 / RAND_MAX * (area.h - 0.001);
		x = (int)(area.x + q);
		y = (int)(area.y + r);
		if (i > 1000) return NULL;
		i++;
	}
	e->setPos(Vector2D(x, y));
	registerEntity(e);
	return e;
}

void Scenario::spawnHeroes(shared_ptr<Entity> templ, Rectangle area, shared_ptr<Weapon> startWeapon, double amount) {
	for (int i = 0; i < amount; i++) {
		spawnHero(templ, area, startWeapon);
	}
}

shared_ptr<Hero> Scenario::spawnHero(shared_ptr<Entity> templ, Rectangle area, shared_ptr<Weapon> startWeapon) {
	shared_ptr<Hero> e = make_shared<Hero>(templ->copyEntity());
	e->weapon = startWeapon;
	int x = (int)(area.x + rand() * 1.0 / RAND_MAX * area.w);
	int y = (int)(area.y + rand() * 1.0 / RAND_MAX * area.h);
	int i = 0;
	double q = 0;
	double r = 0;
	while (entityAt(x, y)) {
		q = rand() * 1.0 / RAND_MAX * (area.w - 0.001);
		r = rand() * 1.0 / RAND_MAX * (area.h - 0.001);
		x = (int)(area.x + q);
		y = (int)(area.y + r);
		if (i > 1000) return NULL;
		i++;
	}
	e->setPos(Vector2D(x, y));
	registerEntity(e);
	return e;
}

vector<shared_ptr<Hero>> Scenario::getHeroes() {
	vector<shared_ptr<Hero>> heroes;
	for (size_t i = 0; i < entities.size(); i++) {
		if (entities[i]->type == Hero_t) heroes.push_back(std::dynamic_pointer_cast<Hero>(entities[i]));
	}
	return heroes;
}
vector<shared_ptr<Monster>> Scenario::getMonsters() {
	vector<shared_ptr<Monster>> monsters;
	for (size_t i = 0; i < entities.size(); i++) {
		if (entities[i]->type == Monster_t) monsters.push_back(std::dynamic_pointer_cast<Monster>(entities[i]));
	}
	return monsters;
}
vector<shared_ptr<Store>> Scenario::getStores() {
	vector<shared_ptr<Store>> stores;
	for (size_t i = 0; i < structures.size(); i++) {
		if (structures[i]->type == Store_t) stores.push_back(std::dynamic_pointer_cast<Store>(structures[i]));
	}
	return stores;
}

bool Scenario::entityAt(int x, int y) {
	for (size_t i = 0; i < entities.size(); i++) {
		if ((int)(entities[i]->pos.x + 0.5) == x && (int)(entities[i]->pos.y + 0.5) == y) {
			return true;
		}
	}
	return false;
}

void Scenario::update(double dTime) {
	UpdateZones(dTime);
	UpdateEntities(dTime);

}