#include "Monster.h"

Monster Monster::copyMonster() {
	Monster nm = Monster(*this, health, money, dmg, range);
	return nm;
}