#pragma once
#include "WObject.h"
#include <memory>

class Entity;

using namespace std;

enum StructT { SUndefined, Store_t, ResourceNode_t };

class Map;


class Structure : public WObject {
public:

	Structure(StructT Type = SUndefined, Vector2D position = Vector2D(0, 0), double size = 1, double imgSize = 1, bool* borderData = 0, bool isSafe = false, Vector2D midPoint = Vector2D(0.5, 0.5));
	Structure(LTexture text, StructT Type = SUndefined, Vector2D position = Vector2D(0, 0), double size = 1, double imgSize = 1, bool* borderData = 0, bool isSafe = false, Vector2D midPoint = Vector2D(0.5, 0.5));
	Structure(SDL_Renderer * rend, string textureType, string name, StructT Type = SUndefined, Vector2D position = Vector2D(0, 0), double size = 1, double imgSize = 1, bool* borderData = 0, bool isSafe = false, Vector2D midPoint = Vector2D(0.5, 0.5));
	Structure copy();
	bool isWithin(Entity* e);
	void SetPosition(size_t x, size_t y, shared_ptr<Map> m);

	virtual void render();

	bool borderData[8];

	StructT type;
	double size;
	Vector2D midPoint;

	bool isSafe;

};