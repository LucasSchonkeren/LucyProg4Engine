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

	BoxCollider(eng::Actor& owner, glm::vec2 size, glm::vec2 pivot = glm::vec2{}) : AbstractComponent(owner), m_Size(size), m_Pivot(pivot) {};
	~BoxCollider() = default;

	BoxCollider(const BoxCollider&) = delete;
	BoxCollider& operator=	(const BoxCollider&) = delete;

	BoxCollider(BoxCollider&&) = delete;
	BoxCollider& operator=	(BoxCollider&&) = delete;

public: //---------------|Serialization|--------------
	nlohmann::ordered_json Serialize() override;
	static std::unique_ptr<BoxCollider> Deserialize(Actor& owner, const nlohmann::json& json);

	std::string TypeName() override { return "BoxCollider"; }

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

}; // !FpsTracker

} // !cpt