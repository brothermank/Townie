#include "WObject.h"

WObject::WObject(LTexture text, Vector2 pos, double imgSize) : imgSize(imgSize) {
	texture = text;
	id = rand();
	this->pos = pos;
}


WObject::WObject(SDL_Renderer * rend, string textureType, string name, Vector2 pos, double imgSize) : imgSize(imgSize) {
	texture = LTexture(rend, textureType, name);
	id = rand();
	this->pos = pos;
}
