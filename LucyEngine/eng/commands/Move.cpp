#include "Move.h"
#include "../engine/Time.h"
#include "glm.hpp"

eng::cmd::Move::Move(glm::vec2 velocity) : 
	m_Velocity(velocity) {
}

bool eng::cmd::Move::Execute(Actor& target) {
	target.GetTransform().TranslatePosition(static_cast<float>(eng::time::DeltaTime()) *  m_Velocity);
	return true;
}

