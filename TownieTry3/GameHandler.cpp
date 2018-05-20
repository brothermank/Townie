#include "GameHandler.h"


GameHandler::GameHandler() : mFrame(GUIEngine(&resourceHandler)) {}

void GameHandler::SetUpGame(){
	mFrame = GUIEngine(&resourceHandler);
	wEngine = WorldEngine(&resourceHandler);
	mFrame.initMainWindow();
	wEngine.LoadScenario();
	mFrame.showScenario(wEngine.getScenario());
	
}
void GameHandler::StartGame(){
}
void GameHandler::PauseGame(){
}
void GameHandler::StopGame() {
	mFrame.closeMainWindow();
	//mframe.destroy();
}

void GameHandler::gameMainLoop() {
	/*bool quit = false;
	
	wEngine.updateWorld();
	mFrame.updateGUI();

	SDL_Event e;
	SDL_StartTextInput();

	Uint32 timeLast = SDL_GetTicks();
	double dTime;

	double timer1 = 0;
	double time1sec = 0;
	int frames = 0;
	int frameRateCount = 0;

	bool heldClick = false;
	Debugger::print("Set: InitVar\tIniFlg\tMainL\tNNode\tFNode\tSNodeR\tSNodeF\tNNodeS\tNNodeF\n");

	while (!quit) {
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN) {
				currentWindow->ReceiveHotkeyInput(e);
			}
			else if (e.type == SDL_TEXTINPUT) {
				currentWindow->ReceiveTextInput(e);
			}
			//TODO: Transfer handle to mapWindow
			else if (e.type == SDL_MOUSEWHEEL) {
				currentWindow->zoom *= (pow(1.1, e.wheel.y));
			}
		}

		//Handle clicks
		int *x, *y;
		int a1 = 0, a2 = 0;
		x = &a1;
		y = &a2;
		Uint32 bmap = SDL_GetMouseState(x, y);
		currentWindow->ReceiveClick(Vector2D(*x, *y), bmap, heldClick);
		if (bmap & SDL_BUTTON(SDL_BUTTON_LEFT) && !heldClick) {
			Debugger::print("Click\n");
			heldClick = true;
		}
		else if (!(bmap & SDL_BUTTON(SDL_BUTTON_LEFT)) && heldClick) {
			Debugger::print("Unclick\n");
			heldClick = false;
		}

		//Update window
		currentWindow->update(dTime);

		//Timer handling
		dTime = 0;
		while (dTime < 1.0 / frameCap) {
			dTime = (SDL_GetTicks() - timeLast) * 0.001;
		}
		timeLast += (Uint32)(dTime * 1000);
		timer1 += dTime;
		time1sec += dTime;
		frames++;
		frameRateCount++;
		if (time1sec > 1.0) {
			currentWindow->frameRate->setText("Frame Rate: " + strh::toString(frameRateCount));
			time1sec -= 1;
			frameRateCount = 0;
		}


	}

	//closeGame();
	*/

}