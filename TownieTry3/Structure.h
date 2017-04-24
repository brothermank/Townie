#pragma once
#include "WObject.h"

class Entity;

using namespace std;

enum StructT { SUndefined, Store_t };

class Map;


class Structure : public WObject {
public:

	Structure(StructT Type = SUndefined, Vector2 position = Vector2(0, 0), double size = 1, double imgSize = 1, bool* borderData = 0, bool isSafe = true);
	Structure(LTexture text, StructT Type = SUndefined, Vector2 position = Vector2(0, 0), double size = 1, double imgSize = 1, bool* borderData = 0, bool isSafe = true);
	Structure(SDL_Renderer * rend, string textureType, string name, StructT Type = SUndefined, Vector2 position = Vector2(0, 0), double size = 1, double imgSize = 1, bool* borderData = 0, bool isSafe = true);
	Structure copy();
	bool isWithin(Entity* e);
	void SetPosition(int x, int y, Map* m);

	virtual void render();

	bool borderData[4];

	StructT type;
	double size;

	bool isSafe;
};