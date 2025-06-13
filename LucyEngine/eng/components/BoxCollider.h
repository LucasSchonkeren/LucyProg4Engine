#pragma once

#include "../abstract/AbstractComponent.h"
#include "../physics/Collisions.h"
#include <glm.hpp>

namespace eng::eventHash {

constexpr unsigned int OnCollisionEnter{ make_sdbm_hash("OnCollisionEnter") };

constexpr unsigned int OnCollision{ make_sdbm_hash("OnCollision") };

constexpr unsigned int OnCollisionExit{ make_sdbm_hash("OnCollisionExit") };

}

namespace eng::eventContext {

struct Collision {
	physics::IAABBCollider& other;
	bool solid;
};

}

class eng::Actor;

namespace eng::cpt {

class BoxCollider final : public eng::AbstractComponent, public eng::physics::IAABBCollider, public IObserver {
public: //---------------|Constructor/Destructor/copy/move|--------------

	BoxCollider(eng::Actor& owner, glm::vec2 size, glm::vec2 pivot = glm::vec2{}, bool solid = {false}) : AbstractComponent(owner), m_Size(size), m_Pivot(pivot), m_Solid(solid) {};
	~BoxCollider() = default;

	BoxCollider(const BoxCollider&) = delete;
	BoxCollider& operator=	(const BoxCollider&) = delete;

	BoxCollider(BoxCollider&&) = delete;
	BoxCollider& operator=	(BoxCollider&&) = delete;

public: //------------------|Gameloop Methods|--------------------------
	
	void Start() override;
	void Render() override;

public: //------------------|Observer Methods|--------------------------

	void OnEvent(Event event) override;

public: //------------------|Collider Methods|--------------------------

	 const physics::Boundsf& Bounds();

	 void			OnCollisionEnter(IAABBCollider* other);
	 void			OnCollision(IAABBCollider* other);
	 void			OnCollisionExit(IAABBCollider* other);

public: //------------------|Getter Methods|--------------------------

	const bool IsSolid() override;

public: //------------------|Observer Methods|--------------------------

	void AddObserver(IObserver& observer);
	void RemoveObserver(IObserver& observer);

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|collider fields|----------------------------

	physics::Boundsf m_Bounds{};
	bool m_BoundsDirty{ true };

	glm::vec2 m_Size; 
	glm::vec2 m_Pivot;

private: //---------------------------|Observer fields|----------------------------

	Subject m_Subject{};
	bool m_Solid{ false };

}; // !FpsTracker

} // !cpt