#include "FpsTracker.h"
#include "../Engine.h"
#include "../Actor.h"
#include "TextRenderer.h"
#include <string>

void cpt::FpsTracker::Update() {
	float fps = 1 / eng::deltaTime;

	if (!GetOwner()) return;
	if (!GetOwner().val_get().GetComponent<TextRenderer>()) return;

	GetOwner().val_get().GetComponent<TextRenderer>().val_get().SetText("fps: " + std::to_string(fps));
}
