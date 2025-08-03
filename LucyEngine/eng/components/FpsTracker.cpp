#include "FpsTracker.h"
#include "../Actor.h"
#include "TextRenderer.h"
#include <string>
#include "../input/Input.h"

namespace eng::cpt {

REGISTER_COMPONENT(FpsTracker)

std::unique_ptr<FpsTracker> FpsTracker::Deserialize(Actor& owner, const nlohmann::json&) {
	return std::make_unique<FpsTracker>(owner);
}

void eng::cpt::FpsTracker::Start() {
	assert(Owner().GetComponent<TextRenderer>() && "An FpsTracker Component requirs a Text Renderer");
	m_TextRendererPtr = Owner().GetComponent<TextRenderer>();
}

void eng::cpt::FpsTracker::Update() {
	//Only update this component every ~0.5 seconds so our fps is actually readable
	static double f_timePassed{};
	if (f_timePassed += Owner().DeltaTime(); f_timePassed < 0.5) return;
	f_timePassed = 0;

	int fps = static_cast<int>(1 / Owner().DeltaTime());

	m_TextRendererPtr->SetText("fps: " + std::to_string(fps));
}

} // !eng::cpt