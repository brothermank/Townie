#include "WObject.h"

WObject::WObject(LTexture text, Vector2D pos, double imgSize) : imgSize(imgSize) {
	texture = text;
	id = rand();
	this->pos = pos;
}


WObject::WObject(SDL_Renderer * rend, string textureType, string name, Vector2D pos, double imgSize) : imgSize(imgSize) {
	texture = LTexture(rend, textureType, name);
	id = rand();
	this->pos = pos;
}
