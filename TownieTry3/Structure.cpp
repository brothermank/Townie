#include "Structure.h"
#include "Map.h"
#include "Entity.h"


Structure::Structure(StructT Type, Vector2D position, double size, double imgSize, bool* borderData, bool isSafe, Vector2D midPoint) 
	: type(Type), size(size), midPoint(midPoint), WObject(position, imgSize), isSafe(isSafe) {
	for (int i = 0; i < 4; i++) {
		if (borderData != 0) {
			this->borderData[i] = borderData[i];
			this->borderData[i + 4] = borderData[i];
		}
		else {
			this->borderData[i] = true;
			this->borderData[i + 4] = true;
		}
	}
}
Structure::Structure(LTexture text, StructT Type, Vector2D position, double size, double imgSize, bool* borderData, 
	bool isSafe, Vector2D midPoint) :type(Type), size(size), isSafe(isSafe), midPoint(midPoint), WObject(text, position, imgSize) {
	for (int i = 0; i < 4; i++) {
		if (borderData != 0) {
			this->borderData[i] = borderData[i];
			this->borderData[i + 4] = borderData[i];
		}
		else {
			this->borderData[i] = true;
			this->borderData[i + 4] = true;
		}
	}
}
Structure::Structure(SDL_Renderer * rend, string textureType, string name, StructT Type, Vector2D position, double size,
	double imgSize, bool* borderData, bool isSafe, Vector2D midPoint) :type(Type), size(size), isSafe(isSafe), midPoint(midPoint),
	WObject(rend, textureType, name, position, imgSize) {
	for (int i = 0; i < 4; i++) {
		if (borderData != 0) {
			this->borderData[i] = borderData[i];
			this->borderData[i + 4] = borderData[i];
		}
		else {
			this->borderData[i] = true;
			this->borderData[i + 4] = true;
		}
	}
}

Structure Structure::copy() {
	return Structure(this->texture, this->type, this->pos, this->size, this->imgSize, this->borderData, this->isSafe, this->midPoint);
}

void Structure::render() {

}

bool Structure::isWithin(Entity* e) {
	return e->pos.x > pos.x - size && e->pos.x < pos.x + size
		&& e->pos.y > pos.y - size && e->pos.y < pos.y + size;
}
void Structure::SetPosition(size_t x, size_t y, shared_ptr<Map> map) {
	if (map->setStructureAtSafe(x, y, *this)) {
		map->removeStructureAtSafe(x, y);
		pos.x = x;
		pos.y = y;
	}
}