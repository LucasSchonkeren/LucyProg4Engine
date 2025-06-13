#pragma once

#include <set>

namespace eng {

class Actor;

class IActorGraph {
public:
	virtual ~IActorGraph() = default;

	virtual void Cleanup() = 0;

	virtual void FlagActorDestroy(Actor* actor) = 0;
	virtual void UnFlagActorDestroy(Actor* actor) = 0;
	virtual void FlagActorNewParent(Actor* actor) = 0;
};

class ActorGraph final : public IActorGraph {
public: //---------------------|Scenegraph methods|-----------------------------------

	void Cleanup() override;

	void FlagActorDestroy(Actor* actor) override;
	void UnFlagActorDestroy(Actor* actor) override;
	void FlagActorNewParent(Actor* actor) override;

private: //---------------------------|Scenegraph fields|----------------------------

	std::set<Actor*> m_DestroyActors{};
	std::set<Actor*> m_NewParentActors{};
};

}