#include "GUI.h"
#include "MapWindow.h"

void GButtonSelectTile::OnClick() {
	w->paintingKey = tileType % totalTextures;
}


void GButtonSelectBrushSize::OnClick() {
	w->changePaintSize(increment);
}

