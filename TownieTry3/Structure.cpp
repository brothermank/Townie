#include "Structure.h"
#include "Map.h"
#include "Entity.h"

Structure::Structure(StructT Type, Vector2 position, double size, double imgSize, bool* borderData, bool isSafe) : type(Type), size(size), WObject(position, imgSize), isSafe(isSafe) {
	for (int i = 0; i < 4; i++) {
		if (borderData != 0) {
			this->borderData[i] = borderData[i];
		}
		else {
			this->borderData[i] = true;
		}
	}
}
Structure::Structure(LTexture text, StructT Type, Vector2 position, double size, double imgSize, bool* borderData, bool isSafe) :type(Type), size(size), isSafe(isSafe), WObject(text, position, imgSize) {
	for (int i = 0; i < 4; i++) {
		if (borderData != 0) {
			this->borderData[i] = borderData[i];
		}
		else {
			this->borderData[i] = true;
		}
	}
}
Structure::Structure(SDL_Renderer * rend, string textureType, string name, StructT Type, Vector2 position, double size, double imgSize, bool* borderData, bool isSafe) :type(Type), size(size), isSafe(isSafe), WObject(rend, textureType, name, position, imgSize) {
	for (int i = 0; i < 4; i++) {
		if (borderData != 0) {

			this->borderData[i] = borderData[i];
		}
		else {
			this->borderData[i] = true;
		}
	}
}

Structure Structure::copy() {
	return Structure(this->texture, this->type, this->pos, this->size, this->imgSize, this->borderData, this->isSafe);
}

void Structure::render() {

}

bool Structure::isWithin(Entity* e) {
	return e->pos.x > pos.x - size && e->pos.x < pos.x + size
		&& e->pos.y > pos.y - size && e->pos.y < pos.y + size;
}
void Structure::SetPosition(int x, int y, Map* map) {
	if (map->setStructureAt(x, y, this) != -1) {
		map->removeStructureAt((int)pos.x, (int)pos.y);
		pos.x = x;
		pos.y = y;
	}
}