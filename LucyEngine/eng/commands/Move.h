#pragma once

#include "glm.hpp"
#include "../abstract/AbstractCommand.h"

namespace eng::cmd {

class Move final : public ICommand {
public: //---------------|Constructor/Destructor/copy/move|--------------
	
	Move(glm::vec2 velocity);
	~Move()	= default;

	Move				(const Move&)	= delete;
	Move& operator=	(const Move&)	= delete;

	Move				(const Move&&)	= delete;
	Move& operator=	(const Move&&)	= delete;

public: //---------------------------|Execute|------------------------

	bool Execute(Actor& target) override;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Fields|----------------------------
	glm::vec2 m_Velocity;

}; // !MoveCommand

} // !eng::cmd

