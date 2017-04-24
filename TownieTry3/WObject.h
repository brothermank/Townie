#pragma once
#include "ResourceLoader.h"

class WObject {
public:
	WObject(Vector2 pos = Vector2(0, 0), double imgSize = 1) : pos(pos), imgSize(imgSize) { id = rand(); }
	WObject(LTexture text, Vector2 pos = Vector2(0, 0), double imgSize = 1);
	WObject(SDL_Renderer * rend, string textureType, string name, Vector2 pos = Vector2(0, 0), double imgSize = 1);

	int id;
	Vector2 pos;

	void setPos(Vector2 pos) { this->pos = pos; }
	Vector2 getPos() { return pos; }

	double distanceTo(WObject * e) { return (((*e).pos - this->pos).magnitude()); }
	double distanceTo(Vector2 pos) { return ((pos - this->pos).magnitude()); }

	double imgSize;

	LTexture texture;
};