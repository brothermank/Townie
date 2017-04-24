#include "Item.h"

Item Item::copyItem() {
	Item i = Item(texture, i.type, pos, imgSize);
	i.value = value; i.power = power;
	return i;
}
