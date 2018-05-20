#pragma once
#include "Entity.h"
#include <memory>
#include "Weapon.h"
#include "Store.h"

class Monster;

struct ItemSlot {
	vector<Item::ItemT> acceptedTypes;
	bool isEquipped;
	shared_ptr<Item> item;

	ItemSlot(Item::ItemT acceptedType) : isEquipped(false) {
		acceptedTypes.push_back(acceptedType);
		item = make_shared<Item>(Item(Item::ItemT::Undefined));
		//	item->
	}
	ItemSlot(vector<Item::ItemT> acceptedTypes) : isEquipped(false), acceptedTypes(acceptedTypes) {
	}
};

class Hero : public Entity {
public:

	Hero(Entity baseEntity, double Health = 10, double Money = 1, double Dmg = 1, double Range = 0.2) : fightMode(false), timer(0), Entity(baseEntity.copyEntity()) {
		type = Hero_t; money = Money; dmg = Dmg; health = Health; range = Range; hasT = 0, hasStoreT = 0;
	}

	shared_ptr<Weapon> weapon;

	virtual void render(Scenario* world, double dTime = 0);
	Hero copyHero();
	void update(double dTime);
	void TargetNearestMonster(int maxDistance = 9999);
	void TargetNearestStore();
	double getDmg();

	double getOutfitValue();


	void shopAt(shared_ptr<Store> s);

	shared_ptr<Monster> getTarget() {
		return target;
	}


private:
	void AttackTarget();
	bool isWithinRange = false;
	bool goingShopping = true;
	bool fightMode = false;

	std::shared_ptr<Monster> target;
	std::shared_ptr<Store> targetStore;

	int hasT;
	int hasStoreT;

	double timer;

	void checkTarget();

protected:
	void FollowPath();

};
