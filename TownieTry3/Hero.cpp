#include "MapWindow.h"
#include "Debugger.h"
#include "StringHelper.h"


void Hero::render(MapWindow* world, double dTime) {
	Vector2D scrpos = world->mapPosToScreenPos(pos + Vector2D(0, 1));
	bool isInsideBuilding = false;
	for (size_t i = 0; i < world->structures.size(); i++) {
		if (pos.x < world->structures[i]->pos.x + world->structures[i]->size && pos.x > world->structures[i]->pos.x - world->structures[i]->size
			&& pos.y < world->structures[i]->pos.y + world->structures[i]->size && pos.y > world->structures[i]->pos.y - world->structures[i]->size)
		{
			isInsideBuilding = true;
			break;
		}
	}
	if (!isInsideBuilding) {
		double scale = texture.mWidth * 1.0f / texture.mHeight;
		//int width = (int)(imgSize * world->zoom * scale);
		texture.render(scrpos, imgSize * world->zoom, world->getRend(), NULL, NULL, Vector2D(0.5, 1));
		if (weapon != NULL) {
			Vector2D weaponAnchor = scrpos + Vector2D(imgSize * scale * 0.38, imgSize * -0.22) * world->zoom;
			weapon->Render(world, weaponAnchor, world->getRend(), isWithinRange, dTime);
		}
	}
}

void Hero::TargetNearestMonster(int maxDistance) {
	shared_ptr<Monster> nearestMonster = NULL;
	double distance = INFINITY;
	for (size_t i = 0; i < n.world->entities.size(); i++) {
		if (n.world->entities[i]->type == Monster_t && distanceTo(&(*n.world->entities[i])) < distance) {
			distance = distanceTo(&(*n.world->entities[i]));
			nearestMonster = static_pointer_cast<Monster>(n.world->entities[i]);
		}
	}
	if (nearestMonster != NULL) {
		Path p;
		if ((p = n.findPath(pos, nearestMonster->pos)).pathLength() < maxDistance) {
			hasT = 1;
			target = nearestMonster;
			path = n.findPath(pos, target->pos);
		}
	}

}
void Hero::TargetNearestStore() {
	shared_ptr<Store> nearestStore = NULL;
	double distance = INFINITY;
	vector<shared_ptr<Store>> stores = n.world->getStores();
	for (size_t i = 0; i < stores.size(); i++) {
		if (distanceTo(&(*stores[i])) < distance) {
			distance = distanceTo(&(*stores[i]));
			nearestStore = static_pointer_cast<Store>(stores[i]);
		}
	}
	if (nearestStore != NULL) {
		targetStore = nearestStore;
		hasStoreT = 1;
		path = n.findPath(pos, targetStore->pos);
	}
	else hasStoreT = 0;
}

void Hero::AttackTarget() {
	int q;
	if (weapon->isTriggering) {
		q = target->RecieveAttack(this);

		if (q) { //Attack... If the monster died
			if (target->getMoney() > 0) {//Take money
				changeMoney(target->getMoney());
				target->changeMoney(-target->getMoney());
			}
			//Remove reference
			target = NULL;
			hasT = false;
			if (hasStoreT) {
				path = n.findPath(pos, targetStore->getPos());
			}
		}
	}
}

void Hero::shopAt(shared_ptr<Store> s) {
	vector<shared_ptr<Item>> items = s->items;
	for (size_t i = 0; i < items.size(); i++) {
		if (items[i]->getValue() < money && items[i]->type == Item::Weapon) {
			s->SellItem(i, this);
			weapon = std::dynamic_pointer_cast<Weapon>(items[i]);
			goingShopping = false;
			break;
		}
	}
}

double Hero::getOutfitValue() {
	return 0;
}


void Hero::update(double dTime) {
	timer += dTime;
	isWithinRange = false;
	if (!hasT) {
		TargetNearestMonster(2);
	}
	if (hasT) {
		if (distanceTo(target->pos) < range) {
			isWithinRange = true;
			AttackTarget();
		}
	}
	else if (goingShopping) {
		int i = 0;
		if (hasStoreT == 0) TargetNearestStore();
		else {
			if (targetStore->isWithin(this)) {
				shopAt(targetStore);
			}
		}
	}
	FollowPath();
}

Hero Hero::copyHero() {
	Hero nh = Hero(*this, this->health, this->money, this->dmg, this->range);
	return nh;
}