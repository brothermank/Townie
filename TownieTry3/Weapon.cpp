#include "Weapon.h"
#include <math.h>
#include "Scenario.h"

[[deprecated]]  //Rendering is now done by render location (i.e. ScenarioView, item explorer)
void Weapon::Render(Scenario* world, Vector2D gAnchor, SDL_Renderer* rend, bool animating, double dTime) {
	/*isTriggering = false;
	int width = texture.mWidth;
	int height = texture.mHeight;
	double scale = texture.mWidth * 1.0 / texture.mHeight;
	if (animating) {
		a.time += dTime;
		while (a.time > a.points[a.step].time * atkTime) {
			a.time -= a.points[a.step].time * atkTime;
			a.step = (a.step + 1) % a.points.size();
			if (a.points[a.step].trigger) {
				isTriggering = true;
			}
		}

		double rotLast = a.points[a.step].fRot;
		double rotNext = a.points[a.step].tRot;
		double fraction = a.time / (a.points[a.step].time * atkTime);
		double rot = rotLast + (rotNext - rotLast) * fraction;
		if (!((rot = fmod(rot, 360)) < 360 && rot > -360)) rot = 0;
		Vector2D scaledAnchor = Vector2D(pos.x - 0.5, (pos.y - 0.5) / scale);
		Vector2D rotatedAnchor = (scaledAnchor).getRotatedDeg(rot);
		Vector2D anchor = (Vector2D(rotatedAnchor.x, rotatedAnchor.y * scale) + Vector2D(0.5, 0.5));

		texture.render(gAnchor, imgSize * world->zoom, rend, rot, NULL, anchor);
		
	}
	else {
		texture.render(gAnchor, imgSize * world->zoom, rend, 0, NULL, pos);;
	}
	*/
}