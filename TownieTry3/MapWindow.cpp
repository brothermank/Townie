#include "MapWindow.h"
#include <math.h>
#include "Debugger.h"

SDL_RendererFlip flipType = SDL_FLIP_NONE;

MapWindow::MapWindow(SDL_Window * wind, SDL_Renderer * renderer) : Window(wind, renderer) {
	wpos.x = 0;
	wpos.y = 0;
	zoom = 1;

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
MapWindow::MapWindow(SDL_Window * wind, SDL_Renderer * renderer, shared_ptr<Map> m) : Window(wind, renderer) {
	wpos.x = 0;
	wpos.y = 0;
	zoom = 1;
	map = m;
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

void MapWindow::DrawMap() {

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

			t = map->getTileAtSafe(x, y);
			t->texture->render(scrpos, tileh, rend);
			//SDL_RenderCopy(rend, t->texture.mTexture, NULL, &renderArea);
		}
	}
}

void MapWindow::saveMap() {
	map->save();
}

Vector2D MapWindow::screnPosToMapPos(Vector2D pos) {

	Vector2D rpos;
	rpos.x = ((pos.x - tilew * 0.25f) / tilew - (pos.y - tileh * 0.25f) / tileh) + 0.5f + wpos.x;
	rpos.y = (pos.x - tilew * 0.25f) / tilew + (pos.y - tileh * 0.25f) / tileh + wpos.y;
	return rpos;
}

Vector2D MapWindow::mapPosToScreenPos(Vector2D pos) {
	Vector2D posr;
	pos.x -= wpos.x;
	pos.y -= wpos.y;

	posr.x = pos.x * tilew * 0.5f + pos.y * tilew * 0.5f;
	posr.y = pos.y * tileh * 0.5f - pos.x * tileh * 0.5f;
	//Debugger::print("In calculation, scrx:" + toString(results[0]) + " scry:" + toString(results[1]) + " with input, x:" + toString(x) + " y:" + toString(y) + "\n");
	return posr;
}
shared_ptr<Map> MapWindow::getMap() {
	return map;
}

void MapWindow::DrawEntities(double dTime) {
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
	while (iEntity < entities.size() && iStruct < structures.size()) {
		if ((scrposEntity = mapPosToScreenPos(entities[iEntity]->pos + Vector2D(0, 1))).y < (scrposStruct = mapPosToScreenPos(structures[iStruct]->pos + Vector2D(0, 1))).y) {
			entities[iEntity]->render(this, dTime);
			iEntity++;
		}
		else {
			if (scrposStruct.x >= -tilew && scrposStruct.x <= scrw && scrposStruct.y >= -tileh && scrposStruct.y <= scrh) {
				double scale = structures[iStruct]->texture.mWidth * 1.0f / structures[iStruct]->texture.mHeight;
				int width = (int)(structures[iStruct]->imgSize * zoom * scale);
				structures[iStruct]->texture.render(scrposStruct, structures[iStruct]->imgSize * zoom, rend, NULL, NULL, Vector2D(0.5, 0.5));
			}
			iStruct++;
		}
	}
	while (iEntity < entities.size()) {
		entities[iEntity]->render(this, dTime);
		iEntity++;
	}
	while (iStruct < structures.size()) {
		scrposStruct = mapPosToScreenPos(structures[iStruct]->pos + Vector2D(0, 1));
		if (scrposStruct.x >= -tilew && scrposStruct.x <= scrw && scrposStruct.y >= -tileh && scrposStruct.y <= scrh) {
			double scale = structures[iStruct]->texture.mWidth * 1.0f / structures[iStruct]->texture.mHeight;
			int width = (int)(structures[iStruct]->imgSize * zoom * scale);
			structures[iStruct]->texture.render(scrposStruct, structures[iStruct]->imgSize * zoom, rend, NULL, NULL, Vector2D(0.5, 0.5));
		}
		iStruct++;
	}
}

void MapWindow::UpdateEntities(double dTime) {
	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->update(dTime);
	}
}
void MapWindow::UpdateZones(double time) {
	for (size_t i = 0; i < zones.size(); i++) {
		zones[i]->tick(time);
	}
}
void MapWindow::registerEntity(shared_ptr<Entity> e) {
	entities.push_back(e);
}
void MapWindow::registerStructure(shared_ptr<Structure> e) {
	structures.push_back(e);
	e->SetPosition((size_t)e->pos.x, (size_t)e->pos.y, map);
}
void MapWindow::registerZone(shared_ptr<Zone> e) {
	zones.push_back(e);
}

void MapWindow::removeEntity(Entity * e) {
	for (size_t i = 0; i < entities.size(); i++) {
		if (entities[i]->id == e->id) {
			entities.erase(entities.begin() + i);
		}
	}
}

shared_ptr<Monster> MapWindow::spawnMonster(shared_ptr<Entity> templ, int x, int y) {
	if (map->isSafeSafe(x, y)) {
		return NULL;
	}
	shared_ptr<Monster> e = make_shared<Monster>(templ->copyEntity());
	e->setPos(Vector2D(x, y));
	registerEntity(e);
	return e;
}
shared_ptr<Monster> MapWindow::spawnMonster(shared_ptr<Entity> templ, Rectangle area) {
	shared_ptr<Monster> e = make_shared<Monster>(templ->copyEntity());
	int x = (int)(area.x + rand() * 1.0 / RAND_MAX * area.w);
	int y = (int)(area.y + rand() * 1.0 / RAND_MAX * area.h);
	int i = 0;
	double q = 0;
	double r = 0;
	while (map->isSafeSafe(x, y) || entityAt(x, y)) {
		q = rand() * 1.0 / RAND_MAX * (area.w - 0.001);
		r = rand() * 1.0 / RAND_MAX * (area.h - 0.001);
		x = (int)(area.x + q);
		y = (int)(area.y + r);
		if (i > 1000) return NULL;
		i++;
	}
	e->setPos(Vector2D(x, y));
	registerEntity(e);
	return e;
}

vector<shared_ptr<Hero>> MapWindow::getHeroes() {
	vector<shared_ptr<Hero>> heroes;
	for (size_t i = 0; i < entities.size(); i++) {
		if (entities[i]->type == Hero_t) heroes.push_back(std::dynamic_pointer_cast<Hero>(entities[i]));
	}
	return heroes;
}
vector<shared_ptr<Monster>> MapWindow::getMonsters() {
	vector<shared_ptr<Monster>> monsters;
	for (size_t i = 0; i < entities.size(); i++) {
		if (entities[i]->type == Monster_t) monsters.push_back(std::dynamic_pointer_cast<Monster>(entities[i]));
	}
	return monsters;
}
vector<shared_ptr<Store>> MapWindow::getStores() {
	vector<shared_ptr<Store>> stores;
	for (size_t i = 0; i < structures.size(); i++) {
		if (structures[i]->type == Store_t) stores.push_back(std::dynamic_pointer_cast<Store>(structures[i]));
	}
	return stores;
}

bool MapWindow::entityAt(int x, int y) {
	for (size_t i = 0; i < entities.size(); i++) {
		if ((int)(entities[i]->pos.x + 0.5) == x && (int)(entities[i]->pos.y + 0.5) == y) {
			return true;
		}
	}
	return false;
}

void MapWindow::update(double dTime) {
	UpdateZones(dTime);
	UpdateEntities(dTime);
	DrawMap();
	DrawEntities(dTime);

	//Update gui
	vector<shared_ptr<Hero>> heroes;
	if ((heroes = getHeroes()).size() != 0) {
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

bool MapWindow::ReceiveClick(Vector2D pos, Uint32 mask, bool buttonDown) {
	bool guiOverlayed = Window::ReceiveClick(pos, mask, buttonDown);
	if (!guiOverlayed) {
		if (mask & SDL_BUTTON(SDL_BUTTON_LEFT) && !buttonDown) {
			heldLeft = true;
			pos = screnPosToMapPos(pos);
			for (int i = 0; i < paintSize * 2 - 1; i++) {
				for (int j = 0; j < paintSize * 2 - 1; j++) {
					map->editorSetTileAt((int)pos.x - paintSize + 1 + i, (int)pos.y - paintSize + 1 + j, paintingKey);
				}
			}

		}
		else if (heldLeft) {
			heldLeft = false;
			map->nextAction();
		}
		if (mask & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
			if (!heldRight) {
				heldRight = true;
				/*Vector2 pos;
				pos = currentWindow.screnPosToMapPos(Vector2((double)*x, (double)*y));
				paintingKey = currentWindow.getMap()->getValueAt((int)pos.x, (int)pos.y);
				*/
				entities[0]->path = entities[0]->n.findPath(entities[0]->pos, screnPosToMapPos(pos));
			}
		}
		else if (heldRight) {
			heldRight = false;
			map->nextAction();
		}
	}
	return guiOverlayed;
}

void MapWindow::ReceiveHotkeyInput(SDL_Event e) {
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
		getMap()->undoLastChange();
		break;
	default:
		break;
	}
	if (key == SDLK_s && SDL_GetModState() & KMOD_CTRL) {
		getMap()->name = saveText->getText();
		getMap()->save();
	}
	if (key == SDLK_l && SDL_GetModState() & KMOD_CTRL) {
		getMap()->name = saveText->getText();
		getMap()->load(saveText->getText());
	}
}

void MapWindow::changePaintSize(bool increment) {
	if (increment) {
		paintSize++;
	}
	else {
		paintSize--;
	}
	if (paintSize <= 1) paintSize = 1;
}