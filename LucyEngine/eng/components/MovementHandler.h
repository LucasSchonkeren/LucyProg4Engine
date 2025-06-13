#pragma once

#include "../abstract/AbstractComponent.h"
#include "BoxCollider.h"
#include "../abstract/AbstractObserver.h"

class eng::Actor;

namespace eng::cpt {

class MovementHandler final : public eng::AbstractComponent, public eng::physics::IAABBCollider {
public: //---------------|Constructor/Destructor/copy/move|--------------

	MovementHandler(eng::Actor& owner, float speed) : AbstractComponent(owner), m_Speed(speed) {};
	~MovementHandler() = default;

	MovementHandler(const MovementHandler&) = delete;
	MovementHandler& operator=	(const MovementHandler&) = delete;

	MovementHandler(MovementHandler&&) = delete;
	MovementHandler& operator=	(MovementHandler&&) = delete;

public: //------------------|Gameloop Methods|--------------------------

	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;

public: //------------------|Setter Methods|--------------------------

	void SetDirection(glm::vec2 direction);
	void SetSpeed(float speed);


public: //------------------|Collider Methods|--------------------------

	const physics::Boundsf& Bounds() override;
	void			OnCollisionEnter(IAABBCollider* other) override;
	void			OnCollision(IAABBCollider* other) override;
	void			OnCollisionExit(IAABBCollider* other) override;

	const bool IsSolid() override;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Component references|----------------------------

	BoxCollider* m_BoxColliderPtr{};

private: //---------------------------|fields|----------------------------

	glm::vec2 m_Direction{};
	glm::vec2 m_LastDirection{};
	float m_Speed{};

	physics::Boundsf m_Bounds{};

}; // !FpsTracker

} // !cpt