#pragma once
#include "ResourceLoader.h"
#include "Debugger.h"
#include "StringHelper.h"
#include <memory>

class MapWindow;

enum GuiT { GUndefined = 0, Button_t = 1, Text_t = 2 };

struct GuiElement {
	GuiT type;
	LTexture icon;
	Vector2 pos;
	SDL_Renderer * rend;

	GuiElement(SDL_Renderer * Rend, LTexture Icon = LTexture(), Vector2 Pos = Vector2(0, 0), GuiT Type = GUndefined) {
		icon = Icon;
		pos = Pos;
		type = Type;
		rend = Rend;
	}
	virtual void DrawElement() = 0;
	virtual Vector2 getSize() = 0;
	virtual void OnClick() = 0;
};
struct GButton : GuiElement {
	Vector2 size;
	bool isPressed;
	void(*onPress)();

	GButton(SDL_Renderer * rend, LTexture Icon, Vector2 Pos = Vector2(0, 0), Vector2 Size = Vector2(10, 10)) : GuiElement(rend, Icon, Pos, Button_t) {
		size = Size;
		isPressed = false;
	}
	void DrawElement() {
		icon.render(pos, size, rend);
	}
	Vector2 getSize() { return size; }
	virtual void OnClick() = 0;
};
struct GButtonSelectTile : GButton {
	int tileType;
	shared_ptr<MapWindow> w;
	GButtonSelectTile(shared_ptr<MapWindow> W, SDL_Renderer * rend, LTexture Icon, int TileType = 0, Vector2 Pos = Vector2(0, 0), Vector2 Size = Vector2(10, 10)) : GButton(rend, Icon, Pos, Size) {
		tileType = TileType;
		w = W;
	}
	void OnClick();
};
struct GButtonSelectBrushSize : GButton {
	bool increment;
	shared_ptr<MapWindow> w;
	GButtonSelectBrushSize(shared_ptr<MapWindow> W, SDL_Renderer * rend, LTexture Icon, bool Increment = false, Vector2 Pos = Vector2(0, 0), Vector2 Size = Vector2(10, 10)) : GButton(rend, Icon, Pos, Size) {
		increment = Increment;
		w = W;
	}
	void OnClick();
};
struct GTextField : GuiElement {
	int size;
	SDL_Color color;
	bool active;

	GTextField(SDL_Renderer * rend, string Text = "Placeholder", Vector2 Pos = Vector2(0, 0), int Size = 40, SDL_Color Color = SDL_Color()) : GuiElement(rend, LTexture(), Pos, Text_t) {
		icon.loadFromRenderedText(Text, color, rend);
		size = Size;
		active = false;
		text = Text;
		updateTexture = true;
	}
	void DrawElement() {
		if (updateTexture) updateText();
		icon.render(pos, size, rend);
	}
	void updateText() {
		icon.loadFromRenderedText(text, color, rend);
		updateTexture = false;
	}
	Vector2 getSize() {
		double proportion = icon.mWidth * 1.0f / icon.mHeight;
		return Vector2(proportion * size, size);
	}
	void OnClick() {
		active = true;
		Debugger::print("Text, on click " + strh::toString(active) + "\n");
	}
	void addText(string t) {
		text += t;
		updateTexture = true;
	}
	void setText(string t) {
		text = t;
		updateTexture = true;
	}
	void deleteCharacter() {
		text.pop_back();
		updateTexture = true;
	}
	string getText() { return text; }

private:
	string text;
	bool updateTexture;

};
