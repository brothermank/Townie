#include "GUI.h"
#include "ScenarioView.h"

void GButtonSelectTile::OnClick() {
	w->paintingKey = tileType % totalTextures;
}


void GButtonSelectBrushSize::OnClick() {
	w->changePaintSize(increment);
}

