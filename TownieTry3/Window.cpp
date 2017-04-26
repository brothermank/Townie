#include "Window.h"
#include "StringHelper.h"
#include "Debugger.h"

Window::Window(SDL_Window * wind, SDL_Renderer * renderer) {
	rend = renderer;
	window = wind;
}

void Window::DrawGui() {
	shared_ptr<GTextField> tf;
	shared_ptr<GButton> b;
	for (size_t i = 0; i < guiElements.size(); i++) {
		switch (guiElements[i]->type) {
		case Button_t:
			b = static_pointer_cast<GButton>(guiElements[i]);
			b->DrawElement();
			break;
		case Text_t:
			tf = static_pointer_cast<GTextField>(guiElements[i]);
			tf->DrawElement();
			break;
		default:
			break;
		}
	}
}

void Window::update() {
	DrawGui();
	updateScreen();
}

void Window::updateScreen() {
	//Update screen
	SDL_RenderPresent(rend);
}

bool Window::ReceiveClick(Vector2D pos, Uint32 mask, bool buttonDown) {
	bool buttonClick = false;
	bool textClick = false;
	if (mask & SDL_BUTTON(SDL_BUTTON_LEFT) && !buttonDown) {
		Debugger::print("Clicking window\n");
		shared_ptr<GTextField> tf;
		shared_ptr<GButton> b;
		for (size_t i = 0; i < guiElements.size(); i++) {
			switch (guiElements[i]->type) {
			case Button_t:
				b = static_pointer_cast<GButton>(guiElements[i]);
				if (!buttonClick && pos.isWithin(b->pos, b->getSize())) {
					buttonClick = true;
					b->OnClick();
				}
				break;
			case Text_t:
				Debugger::print("Type: text... Text has been clicked: " + strh::toString(textClick) + "\n");
				tf = static_pointer_cast<GTextField>(guiElements[i]);
				if (!textClick && pos.isWithin(tf->pos, tf->getSize())) {
					textClick = true;
					tf->OnClick();
				}
				break;
			default:
				break;
			}
		}
	}
	return buttonClick || textClick;
}

void Window::ReceiveTextInput(SDL_Event e) {
	for (size_t i = 0; i < guiElements.size(); i++) {
		if (guiElements[i]->type == Text_t) {
			shared_ptr<GTextField> tf = static_pointer_cast<GTextField>(guiElements[i]);
			if (tf->active) {
				if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C') && (e.text.text[0] == 'v' || e.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL)) {
					//Append character
					tf->addText(e.text.text);
					Debugger::print("Added text. New text: " + tf->getText() + "\n");
				}
			}
		}
	}
}

void Window::ReceiveHotkeyInput(SDL_Event e) {

	for (size_t i = 0; i < guiElements.size(); i++) {
		//Debugger::print("GUI Type: " + toString(guiElements[i]->type) + "\n");
		if (guiElements[i]->type == Text_t) {
			shared_ptr<GTextField> tf = static_pointer_cast<GTextField>(guiElements[i]);
			if (e.key.keysym.sym == SDLK_BACKSPACE && tf->getText().length() > 0) {
				tf->deleteCharacter();
			}
			else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
			{
				SDL_SetClipboardText(tf->getText().c_str());
			}
			else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
			{
				tf->addText(SDL_GetClipboardText());
			}
		}
	}
}