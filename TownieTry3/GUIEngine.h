#pragma once

#include <iostream>
#include <vector>
#include "ScenarioView.h"
#include "ResourceHandler.h"

#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768

class Scenario;

class GUIEngine {
public: 

	GUIEngine(ResourceHandler* resourceHandler) : resourceHandler(resourceHandler) {}

	bool initMainWindow();
	void closeMainWindow();
	
	void showScenario(Scenario scenario);

	vector<LTexture> textures;
	
	void updateGUI();

private:
	SDL_Window * gMainWindow;
	SDL_Surface* gScreenSurface = NULL;
	SDL_Renderer* gRenderer = NULL;
	ResourceHandler* resourceHandler;

	shared_ptr<ScenarioView> scenarioView;
	vector<shared_ptr<Window>> windows;


};
