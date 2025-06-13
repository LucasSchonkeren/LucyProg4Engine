#include "Move.h"
#include "glm.hpp"
#include "../components/MovementHandler.h"

eng::cmd::Move::Move(glm::vec2 velocity) : 
	m_Velocity(velocity) {
}

bool eng::cmd::Move::Execute(Actor& target) {
	auto f_MoveHandler{target.GetComponent<cpt::MovementHandler>()};
	if (f_MoveHandler) {
		f_MoveHandler->SetDirection(m_Velocity);
		f_MoveHandler->SetSpeed(static_cast<float>(m_Velocity.length()));
		return true;
	}
	else {
		target.GetTransform().TranslatePosition(static_cast<float>(target.DeltaTime()) * m_Velocity);
		return true;
	}
}

