#include "GamePhysics.h"

void eng::GamePhysics::RegisterCollider(physics::IAABBCollider& collider) {
	m_AABBCollisionHandler.RegisterCollider(collider);
}

void eng::GamePhysics::UnRegisterCollider(physics::IAABBCollider& collider) {
	m_AABBCollisionHandler.UnRegisterCollider(collider);
}

void eng::GamePhysics::NotifyCollisions() {
	m_AABBCollisionHandler.NotifyCollisions();
}
