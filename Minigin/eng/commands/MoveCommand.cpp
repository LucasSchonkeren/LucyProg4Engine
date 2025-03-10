#include "MoveCommand.h"
#include "../engine/Time.h"
#include "glm.hpp"

eng::cmd::MoveCommand::MoveCommand(glm::vec2 velocity) : 
	m_Velocity(velocity) {
}

bool eng::cmd::MoveCommand::Execute(Actor& target) {
	target.GetTransform().TranslatePosition(static_cast<float>(eng::time::DeltaTime()) *  m_Velocity);
	return true;
}
