#pragma once

#include "../physics/Collisions.h"

namespace eng {

class IGamePhysics {
public:
	virtual ~IGamePhysics() = default;

	virtual void RegisterCollider(physics::IAABBCollider& collider) = 0;
	virtual void UnRegisterCollider(physics::IAABBCollider& collider) = 0;
	virtual void NotifyCollisions() = 0;
};

class GamePhysics final : public IGamePhysics {
public: 
	void RegisterCollider(physics::IAABBCollider& collider);
	void UnRegisterCollider(physics::IAABBCollider& collider);
	void NotifyCollisions();

private: //---------------------------|Fields|----------------------------

	physics::AABBCollisionHandler m_AABBCollisionHandler{};

}; // !GamePhysics

} // !eng::physics