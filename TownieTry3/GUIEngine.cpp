#include "GUIEngine.h"
#include "DependencyHandler.h"
#include "ResourceLoader.h"
#include "Scenario.h"



bool GUIEngine::initMainWindow() {
	bool success = true;

	gMainWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gMainWindow == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
	}
	else {
		gRenderer = SDL_CreateRenderer(&*gMainWindow, -1, SDL_RENDERER_ACCELERATED);
		if (gRenderer == NULL) {
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		}
		gScreenSurface = SDL_GetWindowSurface(&*gMainWindow);
	}
	LoadFont("Raleway-Regular");

	resourceHandler->setRenderer(gRenderer);
	
	return success;
}
void GUIEngine::closeMainWindow() {

	//Free loaded images
	int textureCount = textures.size();
	for (int i = textureCount - 1; i > 0; i--) {
		SDL_DestroyTexture(textures[i].mTexture);
	}
	textures.erase(textures.begin(), textures.end());

	//Destroy window    
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gMainWindow);
	gMainWindow = NULL;
	gRenderer = NULL;

}

void GUIEngine::showScenario(Scenario scenario) {
	scenarioView = make_shared<ScenarioView>(ScenarioView(gMainWindow, gRenderer, resourceHandler)); //Create scenario exploration view (map view)
	windows.push_back(dynamic_pointer_cast<Window>(scenarioView)); //add window to update list
	resourceHandler->loadAllTextures(); //Load texture set (all textures for now)
	//create and add element inspection view
}

void GUIEngine::updateGUI() {

}
