#pragma once
#include "Structure.h"

class ResourceNode : public Structure {
public:
	
	enum ResourceType{Rock_t, Wood_t, Gold_t, Food_t};
	
	ResourceNode(Structure s) : Structure(s) {
		type = ResourceNode_t;
	}
	ResourceNode(LTexture text, Vector2D position = Vector2D(0, 0), bool isSafe = true) : Structure(text, ResourceNode_t, position, isSafe) {}
	ResourceNode(SDL_Renderer * rend, string textureType, string name, Vector2D position = Vector2D(0, 0), bool isSafe = true) : Structure(rend, textureType, name, ResourceNode_t, position, isSafe) {}

	ResourceType resource;



};