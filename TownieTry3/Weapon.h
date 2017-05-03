#pragma once
#include "Item.h"
#include <vector>

class MapWindow;

class Weapon : public Item {
public:

	struct Event {
		bool trigger;
		double time;
		double tRot;
		double fRot;
		Event(bool trigger, double time, double fromRotation, double targetRotation)
			: trigger(trigger), time(time), fRot(fromRotation), tRot(targetRotation) {}
	};
	struct Animation {
		std::vector<Event> points;
		double time = 0;
		int step = 0;

		Animation() {}
		Animation(vector<Event> points) {
			for (size_t i = 0; i < points.size(); i++) {
				this->points.push_back(points[i]);
			}
		}
	};

	bool isTriggering = false;

	Weapon() : Item(ItemT::Weapon) {}
	Weapon(Item i, Animation attackAnimation, double dmg = 1) : a(attackAnimation), baseDmg(dmg), Item(i.copyItem()) {
		type = ItemT::Weapon;
	}

	Animation a;
	enum WeaponT { Undefined, Sword };
	double atkTime = 1;


	void Render(MapWindow* world, Vector2D gAnchor, SDL_Renderer* rend, bool animating = false, double dTime = 0);

	double baseDmg;

private:
	bool finishingAnimation = false;
};