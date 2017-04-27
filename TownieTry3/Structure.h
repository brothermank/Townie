#pragma once
#include "WObject.h"

class Entity;

using namespace std;

enum StructT { SUndefined, Store_t };

class Map;


class Structure : public WObject {
public:

	Structure(StructT Type = SUndefined, Vector2D position = Vector2D(0, 0), double size = 1, double imgSize = 1, bool* borderData = 0, bool isSafe = false);
	Structure(LTexture text, StructT Type = SUndefined, Vector2D position = Vector2D(0, 0), double size = 1, double imgSize = 1, bool* borderData = 0, bool isSafe = false);
	Structure(SDL_Renderer * rend, string textureType, string name, StructT Type = SUndefined, Vector2D position = Vector2D(0, 0), double size = 1, double imgSize = 1, bool* borderData = 0, bool isSafe = false);
	Structure copy();
	bool isWithin(Entity* e);
	void SetPosition(size_t x, size_t y, Map* m);

	virtual void render();

	bool borderData[8];

	StructT type;
	double size;

	bool isSafe;
};