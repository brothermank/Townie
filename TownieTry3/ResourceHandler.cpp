#include "ResourceHandler.h"

void ResourceHandler::setRenderer(SDL_Renderer* newRenderer) {
	gRenderer = newRenderer;
}


LTexture * ResourceHandler::getTexture(ETextureType type, int id) {
	switch (type) {
	case ETextureType::environment:
		return &environment[id];
	case ETextureType::entity:
		return &entities[id];
	case ETextureType::structure:
		return &structures[id];
	case ETextureType::item:
		return &items[id];
	default:
		return NULL;
	}
}

void ResourceHandler::loadAllTextures() {
	loadAllEnvironment();
	loadAllEntities();
	loadAllStructures();
	loadAllItems();
}

void ResourceHandler::loadAllEnvironment() {
	environment.push_back(LTexture(gRenderer, "Texture", "Black"));
	environment.push_back(LTexture(gRenderer, "Texture", "Grass1"));
	environment.push_back(LTexture(gRenderer, "Texture", "Road1"));
	environment.push_back(LTexture(gRenderer, "Texture", "Road2"));
	environment.push_back(LTexture(gRenderer, "Texture", "Road3"));
/*
	tiles.push_back(Tile(&textures[0], 100));
	tiles.push_back(Tile(&textures[1], 4));
	tiles.push_back(Tile(&textures[2]));
	tiles.push_back(Tile(&textures[3]));
	tiles.push_back(Tile(&textures[4]));

	//Test tile - no custom graphics
	bool temp1[4] = { true, false, true, false };
	bool temp2[4] = { false, true, false, true };
	bool temp3[4] = { false, false, false, false };
	bool temp4[4] = { false, false, true, false };
	tiles.push_back(Tile(&textures[0], 0.5f, temp4));*/
}
void ResourceHandler::loadAllEntities() {
	entities.push_back(LTexture(gRenderer, "Character", "Viking"));
	entities.push_back(LTexture(gRenderer, "Character", "Tentacle2"));
}
void ResourceHandler::loadAllStructures() {
	bool temp1[4] = { false, false, false, true };
	structures.push_back(LTexture(gRenderer, "Structure", "HouseNew"));
}
void ResourceHandler::loadAllItems() {
	items.push_back(LTexture(gRenderer, "Item", "Sword"));
	items.push_back(LTexture(gRenderer, "Item", "Blank"));
}
