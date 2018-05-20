#include "MonsterZone.h"
#include "Scenario.h"

void MonsterZone::tick(double time) {
	for (size_t i = 0; i < monsters.size(); i++) {
		if (monsters[i]->getHealth() <= 0) {
			monsters.erase(monsters.begin() + i);
			i--;
		}
	}
	if (monsters.size() < (size_t)targetPopulation) { //Spawn a new random monster selected from the monster templates
		int index = (int)(rand() * 1.0 / RAND_MAX * monsterTemplates.size() - 0.01);
		shared_ptr<Monster> newMonster = world->spawnMonster(monsterTemplates[index], area);
		if (newMonster != NULL) {
			monsters.push_back(newMonster);
		}
	}

}

void MonsterZone::addMonsterTemplate(Monster m) {
	monsterTemplates.push_back(make_shared<Monster>(m));
}