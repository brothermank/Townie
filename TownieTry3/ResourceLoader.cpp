#include "ResourceLoader.h"
#include "Debugger.h"
#include "StringHelper.h"

using namespace std;

TTF_Font *gFont;

LTexture::LTexture() {
}
LTexture::LTexture(SDL_Renderer *rend, string type, string name) {
	loadTexture(rend, type, name);
}

LTexture LTexture::loadTexture(SDL_Renderer *rend, string type, string name) {
	return loadTexture(rend, type, name, standardEnding);
}
LTexture LTexture::loadTexture(SDL_Renderer *rend, string type, string name, string ending) {
	SDL_Texture* newTexture = NULL;
	string path = "Images/" + type + "/" + name + ending;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else {
		newTexture = SDL_CreateTextureFromSurface(rend, loadedSurface);
		if (newTexture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		SDL_FreeSurface(loadedSurface);
	}
	mTexture = newTexture;

	Uint32 * format, f = 0;
	int *acess, *w, *h, ac = 0, ww = 0, hh = 0;
	acess = &ac; w = &ww; h = &hh; format = &f;
	SDL_QueryTexture(mTexture, format, acess, w, h);
	mHeight = *h;
	mWidth = *w;
	center.x = (int)(*w * 0.5);
	center.y = *h;

	return *this;
}

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor, SDL_Renderer *renderer)
{
	//Get rid of preexisting texture
	//free();
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else {
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (mTexture == NULL) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else {
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mTexture != NULL;
}


void LTexture::render(Vector2 scrpos, double scale, SDL_Renderer* renderer, double angle, SDL_Rect* clip, Vector2 center, SDL_RendererFlip flip) {
	SDL_Point * c = NULL;
	//Set rendering space and render to screen
	double proportion = mWidth * 1.0f / mHeight;
	SDL_Rect renderQuad = { (int)(scrpos.x - (proportion * scale) * center.x), (int)(scrpos.y - scale * center.y), (int)(scale * proportion), (int)scale };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	//Render to screen
	SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, angle, c, flip);
}
void LTexture::render(Vector2 scrpos, Vector2 size, SDL_Renderer* renderer, double angle, SDL_Rect* clip, Vector2 center, SDL_RendererFlip flip) {
	SDL_Point * c = NULL;
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { (int)(scrpos.x - size.x * center.x), (int)(scrpos.y - size.y * center.y), (int)(size.x), (int)size.y };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	//Render to screen
	SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, angle, c, flip);
}

void LoadFont(string fontName) {
	gFont = TTF_OpenFont(("Fonts/" + fontName + ".ttf").c_str(), 28);
}