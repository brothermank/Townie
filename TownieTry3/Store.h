#pragma once
#include "Structure.h"
#include <vector>
#include "Entity.h"
#include <memory>


enum TradeGoods { TGUndefined = 0, Armor_t = 1, Weapon_t = 2 };

class Store : public Structure {
public:
	Store(Structure s) : Structure(s) {
		type = Store_t;
		isSafe = s.isSafe;
	}
	Store(LTexture text, Vector2D position = Vector2D(0, 0), bool isSafe = true) : Structure(text, Store_t, position, isSafe) {}
	Store(SDL_Renderer * rend, string textureType, string name, Vector2D position = Vector2D(0, 0), bool isSafe = true) : Structure(rend, textureType, name, Store_t, position, isSafe) {}


	bool SellItem(int itemIndex, Entity* customer);
	bool BuyItem(shared_ptr<Item> i, Entity* customer);
	void changeMoney(double amount) { money += amount; }

	vector<TradeGoods> itemsForSale;
	vector<shared_ptr<Item>> items;

	Vector2D entrancePoint;


private:
	double money;
};