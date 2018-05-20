#pragma once
#include "GUIEngine.h"
#include "WorldEngine.h"

class GameHandler {
public:
	enum ItemT { Undefined, Weapon };

	GameHandler();

	void SetUpGame();
	void StartGame();
	void PauseGame();
	void StopGame();

	void gameMainLoop();

private:
	double value, power = 1;
	GUIEngine mFrame;
	WorldEngine wEngine;
	ResourceHandler resourceHandler;

};