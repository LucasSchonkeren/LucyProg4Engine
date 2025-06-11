#include "MoveAlongGrid.h"
#include "../components/GridTransform.h"

eng::cmd::MoveAlongGrid::MoveAlongGrid(bool horizontal, float magnitude, float allignAllowance) :
	m_Allowance{std::clamp(allignAllowance, 0.f, 1.f)} {
	float& f_Axis = horizontal ? m_Movement.x : m_Movement.y;
	f_Axis = magnitude;
}

bool eng::cmd::MoveAlongGrid::Execute(Actor& target)
{
	if (!m_ActorComponentPtrs.contains(&target)) {
		m_ActorComponentPtrs.emplace(&target, 
			std::make_tuple<cpt::GridTransform*>(
				target.GetComponent<cpt::GridTransform>()
		));
		target.AddToggleObserver(*this);
	}
	auto& f_GridTransform{ *std::get<cpt::GridTransform*>(m_ActorComponentPtrs.at(&target)) };

	//check grid allignment
	glm::vec2 f_Allignment{ f_GridTransform.GetAllignment() };
	if (m_Movement.y == 0) {
		if (std::abs(f_Allignment.x) > m_Allowance / 2) return false;
		else if (f_Allignment.x != 0) f_GridTransform.AllignWithGrid();
	}
	if (m_Movement.x == 0) {
		if (std::abs(f_Allignment.y) > m_Allowance / 2) return false;
		else if (f_Allignment.y != 0) f_GridTransform.AllignWithGrid();
	}

	target.GetTransform().TranslatePosition(static_cast<float>(target.DeltaTime()) * m_Movement);

	return true;
}

void eng::cmd::MoveAlongGrid::OnEvent(Event event) {
	if (event.eventTypeHash != eventHash::ActorDisabled) return;

	Actor* f_ActorPtr{ std::any_cast<Actor*>(event.context) };

	m_ActorComponentPtrs.erase(f_ActorPtr);
}
