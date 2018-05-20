#pragma once
#include "ResourceLoader.h"
#include <vector>
#include <SDL_render.h>

enum ETextureType {
	environment,
	entity,
	structure,
	item
};

class ResourceHandler {
public: 

	ResourceHandler() {}
	ResourceHandler(SDL_Renderer* gRenderer) : gRenderer(gRenderer) {}

	void setRenderer(SDL_Renderer* gRenderer);
	LTexture * getTexture(ETextureType type, int id);

	//Resource loading
	void loadAllTextures();
	void loadAllEnvironment();
	void loadAllEntities();
	void loadAllStructures();
	void loadAllItems();

private:
	SDL_Renderer* gRenderer;

	vector<LTexture> environment;
	vector<LTexture> items;
	vector<LTexture> entities;
	vector<LTexture> structures;


};