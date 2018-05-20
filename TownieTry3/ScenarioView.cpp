
#include "scenario.h"
#include "ScenarioView.h"
#include <math.h>
#include "Debugger.h"

SDL_RendererFlip flipType = SDL_FLIP_NONE;

ScenarioView::ScenarioView(SDL_Window * window, SDL_Renderer * renderer, ResourceHandler* resourceHandler) : Window(window, renderer), resourceHandler(resourceHandler) {
	wpos.x = 0;
	wpos.y = 0;
	zoom = 1;

	this->window = window;
	this->rend = renderer;

	shared_ptr<GTextField> temp(new GTextField(rend));
	shared_ptr<GTextField> temp1(new GTextField(rend));
	shared_ptr<GTextField> temp2(new GTextField(rend));
	shared_ptr<GTextField> temp3(new GTextField(rend));
	shared_ptr<GTextField> temp4(new GTextField(rend));
	temp1->pos = Vector2D(0, 80);
	temp2->pos = Vector2D(0, 160);
	temp3->pos = Vector2D(0, 240);
	temp4->pos = Vector2D(0, 320);
	saveText = temp;
	heroMoney = temp1;
	monsterHealth = temp2;
	heroHealth = temp3;
	frameRate = temp4;

	guiElements.push_back(saveText);
	guiElements.push_back(monsterHealth);
	guiElements.push_back(heroHealth);
	guiElements.push_back(heroMoney);
	guiElements.push_back(frameRate);
}

void ScenarioView::DrawEnvironment() {

	int scrw, scrh;
	scrw = SDL_GetWindowSurface(window)->w;;
	scrh = SDL_GetWindowSurface(window)->h;

	int xl;
	int xh;
	int yl;
	int yh;
	xl = (int)(screnPosToMapPos(Vector2D(-tilew, scrh)).x - 1);
	xh = (int)(screnPosToMapPos(Vector2D(scrw, -tileh)).x + 1);
	yl = (int)(screnPosToMapPos(Vector2D(-tilew, -tileh)).y - 1);
	yh = (int)(screnPosToMapPos(Vector2D(scrw, scrh)).y + 1);

	double offx = fmod(wpos.x, 1.0f);
	double offy = fmod(wpos.y, 1.0f);

	//Clear screen
	SDL_RenderClear(rend);
	Tile * t;
	Vector2D scrpos;
	//Render textures to screen
	for (int y = yh; y > yl; y--) {
		for (int x = xl; x < xh; x++) {

			scrpos = mapPosToScreenPos(Vector2D(x, y));

			t = scenario->getMap()->getTileAtSafe(x, y);
			resourceHandler->getTexture(ETextureType::environment, t->textureID)->render(scrpos, tileh, rend);
			//SDL_RenderCopy(rend, t->texture.mTexture, NULL, &renderArea);
		}
	}
}

Vector2D ScenarioView::screnPosToMapPos(Vector2D pos) {

	Vector2D rpos;
	rpos.x = ((pos.x - tilew * 0.25f) / tilew - (pos.y - tileh * 0.25f) / tileh) + 0.5f + wpos.x;
	rpos.y = (pos.x - tilew * 0.25f) / tilew + (pos.y - tileh * 0.25f) / tileh + wpos.y;
	return rpos;
}

Vector2D ScenarioView::mapPosToScreenPos(Vector2D pos) {
	Vector2D posr;
	pos.x -= wpos.x;
	pos.y -= wpos.y;

	posr.x = pos.x * tilew * 0.5f + pos.y * tilew * 0.5f;
	posr.y = pos.y * tileh * 0.5f - pos.x * tileh * 0.5f;
	//Debugger::print("In calculation, scrx:" + toString(results[0]) + " scry:" + toString(results[1]) + " with input, x:" + toString(x) + " y:" + toString(y) + "\n");
	return posr;
}

void ScenarioView::DrawEntities() {
	int scrw = SDL_GetWindowSurface(window)->w;;
	int scrh = SDL_GetWindowSurface(window)->h;
	/*for (size_t i = 0; i < entities.size(); i++) {
	Vector2 scrpos = mapPosToScreenPos(entities[i]->pos + Vector2(0,1));
	if (scrpos.x >= -tilew && scrpos.x <= scrw && scrpos.y >= -tileh && scrpos.y <= scrh) {
	double scale = entities[i]->texture.mWidth * 1.0f / entities[i]->texture.mHeight;
	int width = (int)(EntityHeight * scale);
	entities[i]->texture.render(scrpos, EntityHeight, rend, NULL, NULL, Vector2(0.5,1));
	}
	}*/

	size_t iEntity = 0, iStruct = 0;
	Vector2D scrposEntity;
	Vector2D scrposStruct;
	
	while (iEntity < scenario->entities.size() && iStruct < scenario->structures.size()) {
		if ((scrposEntity = mapPosToScreenPos(scenario->entities[iEntity]->pos + Vector2D(0, 1))).y < (scrposStruct = mapPosToScreenPos(scenario->structures[iStruct]->pos + Vector2D(0, 1))).y) {
			renderEntity(scenario->entities[iEntity]);
			iEntity++;
		}
		else {
			if (scrposStruct.x >= -tilew && scrposStruct.x <= scrw && scrposStruct.y >= -tileh && scrposStruct.y <= scrh) {
				double scale = scenario->structures[iStruct]->texture.mWidth * 1.0f / scenario->structures[iStruct]->texture.mHeight;
				int width = (int)(scenario->structures[iStruct]->imgSize * zoom * scale);
				scenario->structures[iStruct]->texture.render(scrposStruct, scenario->structures[iStruct]->imgSize * zoom, rend, NULL, NULL, Vector2D(0.5, 0.5));
			}
			iStruct++;
		}
	}
	while (iEntity < scenario->entities.size()) {
		renderEntity(scenario->entities[iEntity]);
		iEntity++;
	}
	while (iStruct < scenario->structures.size()) {
		scrposStruct = mapPosToScreenPos(scenario->structures[iStruct]->pos + Vector2D(0, 1));
		if (scrposStruct.x >= -tilew && scrposStruct.x <= scrw && scrposStruct.y >= -tileh && scrposStruct.y <= scrh) {
			double scale = scenario->structures[iStruct]->texture.mWidth * 1.0f / scenario->structures[iStruct]->texture.mHeight;
			int width = (int)(scenario->structures[iStruct]->imgSize * zoom * scale);
			scenario->structures[iStruct]->texture.render(scrposStruct, scenario->structures[iStruct]->imgSize * zoom, rend, NULL, NULL, Vector2D(0.5, 0.5));
		}
		iStruct++;
	}
}

bool ScenarioView::ReceiveClick(Vector2D pos, Uint32 mask, bool buttonDown) {
	bool guiOverlayed = Window::ReceiveClick(pos, mask, buttonDown);
	if (!guiOverlayed) {
		if (mask & SDL_BUTTON(SDL_BUTTON_LEFT) && !buttonDown) {
			heldLeft = true;
			pos = screnPosToMapPos(pos);
			for (int i = 0; i < paintSize * 2 - 1; i++) {
				for (int j = 0; j < paintSize * 2 - 1; j++) {
					scenario->getMap()->editorSetTileAt((int)pos.x - paintSize + 1 + i, (int)pos.y - paintSize + 1 + j, paintingKey);
				}
			}

		}
		else if (heldLeft) {
			heldLeft = false;
			scenario->getMap()->nextAction();
		}
		if (mask & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
			if (!heldRight) {
				heldRight = true;
				/*Vector2 pos;
				pos = currentWindow.screnPosToMapPos(Vector2((double)*x, (double)*y));
				paintingKey = currentWindow.getMap()->getValueAt((int)pos.x, (int)pos.y);
				*/
				scenario->entities[0]->path = scenario->entities[0]->n.findPath(scenario->entities[0]->pos, screnPosToMapPos(pos));
			}
		}
		else if (heldRight) {
			heldRight = false;
			scenario->getMap()->nextAction();
		}
	}
	return guiOverlayed;
}

void ScenarioView::ReceiveHotkeyInput(SDL_Event e) {
	Window::ReceiveHotkeyInput(e);
	SDL_Keycode key = e.key.keysym.sym;

	//Select surfaces based on key press
	switch (key) {
	case SDLK_UP:
		wpos.y -= scrollSpeed;
		wpos.x += scrollSpeed;
		break;
	case SDLK_DOWN:
		wpos.y += scrollSpeed;
		wpos.x -= scrollSpeed;
		break;
	case SDLK_LEFT:
		wpos.y -= scrollSpeed;
		wpos.x -= scrollSpeed;
		break;
	case SDLK_RIGHT:
		wpos.y += scrollSpeed;
		wpos.x += scrollSpeed;
		break;
	case SDLK_d:
		paintingKey = (paintingKey + 1) % totalTextures;
		break;
	case SDLK_a:
		paintingKey--;
		if (paintingKey < 0) paintingKey = totalTextures - 1;
		break;
	case SDLK_z:
		scenario->getMap()->undoLastChange();
		break;
	default:
		break;
	}
	if (key == SDLK_s && SDL_GetModState() & KMOD_CTRL) {
		scenario->getMap()->name = saveText->getText();
		scenario->getMap()->save();
	}
	if (key == SDLK_l && SDL_GetModState() & KMOD_CTRL) {
		scenario->getMap()->name = saveText->getText();
		scenario->getMap()->load(saveText->getText());
	}
}

void ScenarioView::changePaintSize(bool increment) {
	if (increment) {
		paintSize++;
	}
	else {
		paintSize--;
	}
	if (paintSize <= 1) paintSize = 1;
}

void ScenarioView::setScenario(Scenario* s) {

	scenario = s;

	zoom = 0.7;
	wpos = Vector2D(2, -4);
}

void ScenarioView::redraw() {
	DrawEnvironment();
	DrawEntities();

	//Update gui
	vector<shared_ptr<Hero>> heroes;
	if ((heroes = scenario->getHeroes()).size() != 0) {
		heroMoney->setText("Hero money: " + strh::toString(heroes[0]->getMoney()));
		heroHealth->setText("Hero health: " + strh::toString(heroes[0]->getHealth()));
		shared_ptr<Monster> monster;
		if ((monster = heroes[0]->getTarget()) != NULL) {
			monsterHealth->setText("Monster health: " + strh::toString(monster->getHealth()));
		}
		else {
			monsterHealth->setText(" ");
		}
	}

	DrawGui();
	updateScreen();
}

void ScenarioView::renderEntity(shared_ptr<Entity> e) {
	
	Vector2D scrpos = mapPosToScreenPos(e->pos + Vector2D(0, 1));
	bool isInsideBuilding = false;
	for (size_t i = 0; i < scenario->structures.size(); i++) {
		if (e->pos.x < scenario->structures[i]->pos.x + scenario->structures[i]->size && e->pos.x > scenario->structures[i]->pos.x - scenario->structures[i]->size
			&& e->pos.y < scenario->structures[i]->pos.y + scenario->structures[i]->size && e->pos.y > scenario->structures[i]->pos.y - scenario->structures[i]->size)
		{
			isInsideBuilding = true;
			break;
		}
	}
	if (!isInsideBuilding) {
		double scale = e->texture.mWidth * 1.0f / e->texture.mHeight;
		int width = (int)(e->imgSize * zoom * scale);
		e->texture.render(scrpos, e->imgSize * zoom, getRend(), NULL, NULL, Vector2D(0.5, 1));
	}
}