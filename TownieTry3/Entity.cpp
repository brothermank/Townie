#include "Entity.h"
#include "MapWindow.h"
#include "Timer.h"
#include "Debugger.h"


Entity::Entity(LTexture text, MapWindow * map, double speed, Vector2 pos, double imgSize, EntityT t) : WObject(text, pos, imgSize) {
	n = Navigator(map);
	this->speed = speed;
	this->pos = pos;
	type = t;
}
Entity::Entity(SDL_Renderer * rend, string textureType, string name, MapWindow * map, double speed, Vector2 pos, double imgSize, EntityT t) : WObject(rend, textureType, name, pos, imgSize) {
	n = Navigator(map);
	this->speed = speed;
	this->pos = pos;
	type = t;
}
Entity Entity::copyEntity() {
	Entity t;
	t.speed = this->speed;
	t.pos = this->pos;
	t.texture = this->texture;
	t.path = this->path;
	t.n = Navigator(this->n.world);
	t.type = this->type;
	t.imgSize = this->imgSize;
	t.money = this->money;
	t.dmg = this->dmg;
	t.health = this->health;
	return t;
}

void Entity::update(double dTime) {
	FollowPath();
}
void Entity::render(MapWindow* world, double dTime) {
	Vector2 scrpos = world->mapPosToScreenPos(pos + Vector2(0, 1));
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
		int width = (int)(imgSize * world->zoom * scale);
		texture.render(scrpos, imgSize * world->zoom, world->getRend(), NULL, NULL, Vector2(0.5, 1));
	}
}

void Entity::FollowPath() {
	//move along path
	if (path.size() > 0) {
		Vector2 direction = path[0].pos - pos;
		double distance = direction.magnitude();
		direction.normalize();
		if (dTime * speed > distance) {
			//Debugger::print("arrived... Settin pos: " + toString(path.checkpoints[0].pos.x) + "," + toString(path.checkpoints[0].pos.y) + "\n\n\n\n\n\n");
			pos = path[0].pos;
			//Debugger::print("Dtime: " + toString(dTime) + " speed:" + toString(speed) + " lengthTravelled: " + toString(direction.magnitude()) + "\n");
			path.pop();
		}
		else {
			pos += (direction * dTime * speed);
		}
	}
}

int Entity::RecieveAttack(Entity * e) {
	health -= e->dmg;
	if (health <= 0) {//If dead
					  //remove reference from world
		n.world->removeEntity(this);
		//Return that this entity is dead
		return 1;
	}
	return 0;
}
