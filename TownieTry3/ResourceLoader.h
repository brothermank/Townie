#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <SDL_ttf.h>
#include <stdio.h>
#include "Vector.h"

#define standardEnding ".png"


using namespace std;

void LoadFont(string fontName);
//Texture wrapper class
class LTexture
{
public:
	LTexture();
	LTexture(SDL_Renderer *rend, string type, string name);
	//~LTexture();

	////Loads image at specified path
	LTexture loadTexture(SDL_Renderer *rend, string type, string name);
	LTexture loadTexture(SDL_Renderer *rend, string type, string name, string ending);

	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor, SDL_Renderer *renderer);

	////Deallocates texture
	//void free();

	////Set color modulation
	//void setColor(Uint8 red, Uint8 green, Uint8 blue);

	////Set blending
	//void setBlendMode(SDL_BlendMode blending);

	////Set alpha modulation
	//void setAlpha(Uint8 alpha);

	////Renders texture at given point
	void render(Vector2D scrpos, double scale, SDL_Renderer* renderer, double angle = 0.0, SDL_Rect* clip = NULL, Vector2D center = Vector2D(0, 0), SDL_RendererFlip flip = SDL_FLIP_NONE);
	void render(Vector2D scrpos, Vector2D size, SDL_Renderer* renderer, double angle = 0.0, SDL_Rect* clip = NULL, Vector2D center = Vector2D(0, 0), SDL_RendererFlip flip = SDL_FLIP_NONE);
	////Gets image dimensions
	//int getWidth();
	//int getHeight();
	SDL_Texture* mTexture;
	SDL_Point center;
	int mWidth;
	int mHeight;
private:
	//The actual hardware texture


	//Image dimensions
};