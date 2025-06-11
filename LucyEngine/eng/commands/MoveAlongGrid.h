#pragma once

#include <map>

#include "glm.hpp"
#include "../abstract/AbstractCommand.h"
#include "../Actor.h"
#include "../components/GridTransform.h"

namespace eng::cmd {

class MoveAlongGrid final : public ICommand, public IObserver {
public: //---------------|Constructor/Destructor/copy/move|--------------

	MoveAlongGrid(bool horizontal, float magnitude, float allignAllowance);
	~MoveAlongGrid() = default;

	MoveAlongGrid				(const MoveAlongGrid&) = delete;
	MoveAlongGrid& operator=	(const MoveAlongGrid&) = delete;

	MoveAlongGrid				(const MoveAlongGrid&&) = delete;
	MoveAlongGrid& operator=	(const MoveAlongGrid&&) = delete;

public: //---------------------------|Execute|------------------------

	bool Execute(Actor& target) override;

public: //---------------------------|Execute|------------------------

	virtual void OnEvent(Event event);

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Command fields|----------------------------
	glm::vec2 m_Movement{};
	float m_Allowance{};

private: //---------------------------|Cross-actor dependencies|----------------------------
	std::map<Actor*, std::tuple<cpt::GridTransform*>> m_ActorComponentPtrs{};

}; // !MoveAlongGrid

} // !eng::cmd

