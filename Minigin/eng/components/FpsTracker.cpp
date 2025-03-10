#include "FpsTracker.h"
#include "../engine/Time.h"
#include "../Actor.h"
#include "TextRenderer.h"
#include <string>
#include "../input/InputProcessor.h"

void cpt::FpsTracker::Start() {
	assert(GetOwner().GetComponent<TextRenderer>() && "An FpsTracker Component requirs a Text Renderer");
	m_TextRendererPtr = GetOwner().GetComponent<TextRenderer>();
}

void cpt::FpsTracker::Update() {
	//Only update this component every ~0.5 seconds so our fps is actually readable
	static double f_timePassed{};
	if (f_timePassed += eng::time::DeltaTime(); f_timePassed < 0.5) return;
	f_timePassed = 0;

	int fps =  static_cast<int>( 1/eng::time::DeltaTime());

	m_TextRendererPtr->SetText("fps: " + std::to_string(fps));
}
