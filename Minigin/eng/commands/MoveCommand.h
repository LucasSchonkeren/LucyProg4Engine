#pragma once

#include "glm.hpp"
#include "../abstract/AbstractCommand.h"

namespace eng::cmd {

class MoveCommand final : public AbstractCommand {
public: //---------------|Constructor/Destructor/copy/move|--------------
	
	MoveCommand(glm::vec2 velocity);
	~MoveCommand()	= default;

	MoveCommand				(const MoveCommand&)	= delete;
	MoveCommand& operator=	(const MoveCommand&)	= delete;

	MoveCommand				(const MoveCommand&&)	= delete;
	MoveCommand& operator=	(const MoveCommand&&)	= delete;

public: //---------------------------|Execute|------------------------

	bool Execute(Actor& target) override;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Fields|----------------------------
	glm::vec2 m_Velocity;

}; // !MoveCommand

} // !_namespace_