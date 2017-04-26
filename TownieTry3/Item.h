#pragma once
#include "WObject.h"

class Item : public WObject {
public:
	enum ItemT { Undefined, Weapon };

	Item(ItemT type = Undefined, Vector2D anchorPoint = Vector2D(0, 0), double imageSize = 1) : type(type), WObject(anchorPoint, imageSize) {}
	Item(LTexture text, ItemT type = Undefined, Vector2D anchorPoint = Vector2D(0, 0), double imageSize = 1) :type(type), WObject(text, anchorPoint, imageSize) {}
	Item(SDL_Renderer * rend, string textureType, string name, ItemT type = Undefined, Vector2D anchorPoint = Vector2D(0, 0), double imageSize = 1) :type(type), WObject(rend, textureType, name, anchorPoint, imageSize) {}

	Item copyItem();
	ItemT type = Undefined;

	virtual double getValue() { return value * power; }
	void setValue(double val) { value = val; }

private:
	double value, power = 1;
};