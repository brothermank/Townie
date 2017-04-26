#pragma once
#include "ResourceLoader.h"

class WObject {
public:
	WObject(Vector2D pos = Vector2D(0, 0), double imgSize = 1) : pos(pos), imgSize(imgSize) { id = rand(); }
	WObject(LTexture text, Vector2D pos = Vector2D(0, 0), double imgSize = 1);
	WObject(SDL_Renderer * rend, string textureType, string name, Vector2D pos = Vector2D(0, 0), double imgSize = 1);

	int id;
	Vector2D pos;

	void setPos(Vector2D pos) { this->pos = pos; }
	Vector2D getPos() { return pos; }

	double distanceTo(WObject * e) { return (((*e).pos - this->pos).magnitude()); }
	double distanceTo(Vector2D pos) { return ((pos - this->pos).magnitude()); }

	double imgSize;

	LTexture texture;
};