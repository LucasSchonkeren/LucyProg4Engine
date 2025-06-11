#pragma once
#include <glm.hpp>

#include "../abstract/AbstractComponent.h"

#include "../abstract/AbstractObserver.h"

namespace eng {

struct TransformData {
	glm::vec2 position{};
};

} // !eng



namespace eng::cpt {

/// <summary>
/// A component that allows Actors to store their local transform and observe their global transform.
/// </summary>
class Transform final : public AbstractComponent {
public: //---------------|Constructor/Destructor/copy/move|--------------

	Transform(eng::Actor& owner) : AbstractComponent(owner) {};
	~Transform() = default;

	Transform(const Transform&) = delete;
	Transform& operator=	(const Transform&) = delete;

	Transform(const Transform&&) = delete;
	Transform& operator=	(const Transform&&) = delete;

public: //---------------|Transform Methods|--------------

	void SetLocalPosition(float x, float y);
	void SetLocalPosition(glm::vec2 newPosition);

	void TranslatePosition(glm::vec2 translateVector);

	void SetGlobalPosition(float x, float y);
	void SetGlobalPosition(glm::vec2 newPosition);

	void FlagForGlobalUpdate();

	TransformData const& GetLocal() const;
	TransformData const& GetGlobal();

public: //---------------|Subject Methods|--------------

	void AddObserver(IObserver& observer);
	void RemoveObserver(IObserver& observer);

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Transform Fields|----------------------------
	TransformData m_TransformData{};
	TransformData m_GlobalTransformData{};
	bool m_GlobalNeedsUpdate{};

private: //---------------------------|Subject|----------------------------

	Subject m_Subject{};

}; // !TransformComponent

} // !eng::cpt


//-----------------------------------------|Operators|------------------------------------

eng::TransformData operator+(const eng::TransformData& lhs, const eng::TransformData& rhs);

namespace eng::eventContext {

struct PositionChanged {
	eng::cpt::Transform* transformPtr;
	glm::vec2 oldPosition;
	glm::vec2 NewPosition;
};

}