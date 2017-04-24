#pragma once
#include "Entity.h"
class Monster : public Entity {
public:
	Monster(Entity baseEntity) : Entity(baseEntity.copyEntity())
	{
		type = Monster_t;
	}
	Monster(Entity baseEntity, double Health, double Money, double Dmg, double Range) : Entity(baseEntity.copyEntity())
	{
		type = Monster_t; health = Health; money = Money; dmg = Dmg; range = Range;
	}

	Monster copyMonster();
};