#include <vector>
#include <stdlib.h>
#include <memory>
#include "GUI.h"

class Window {

public:
	Window(SDL_Window * window, SDL_Renderer * renderer);
	void DrawGui();
	virtual void update();
	virtual bool ReceiveClick(Vector2D pos, Uint32 mask, bool buttonDown);
	virtual void ReceiveTextInput(SDL_Event e);
	virtual void ReceiveHotkeyInput(SDL_Event e);
	SDL_Window * getWindow() { return window; }

	vector<shared_ptr<GuiElement>> guiElements;

	SDL_Renderer * getRend() { return rend; }

protected:
	SDL_Renderer * rend;
	SDL_Window * window;
	void updateScreen();
};