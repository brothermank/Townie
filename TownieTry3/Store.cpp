#include "Store.h"


bool Store::SellItem(int itemIndex, Entity* customer) {
	if (customer->getMoney() > items[itemIndex]->getValue()) {
		customer->changeMoney(-items[itemIndex]->getValue());
		changeMoney(items[itemIndex]->getValue());
		customer->addItem(items[itemIndex]);
		items.erase(items.begin() + itemIndex);
	}
	return false;
}

bool Store::BuyItem(shared_ptr<Item> item, Entity* customer) {
	if (money > item->getValue() * 0.5) {
		changeMoney(-item->getValue() * 0.5);
		customer->changeMoney(item->getValue() * 0.5);
		items.push_back(item);
		customer->removeItem(item);
	}
	return false;
}