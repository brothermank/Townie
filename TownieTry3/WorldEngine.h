#pragma once

#include "Scenario.h"

class WorldEngine {
public:
	WorldEngine() {};
	WorldEngine(ResourceHandler* resourceHandler) : resourceHandler(resourceHandler) {}

	void LoadScenario();
	Scenario getScenario() { return scenario; }

	void updateWorld();

private:
	ResourceHandler* resourceHandler;
	Scenario scenario;
};