#include "WorldEngine.h"
#include "Tile.h"

void WorldEngine::LoadScenario() {
	scenario = Scenario();

	scenario.getMap()->load("Placeholder");
	scenario.getMap()->loadTiles(loadTileSet_default());
}

void WorldEngine::updateWorld() {

}