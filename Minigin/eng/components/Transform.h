#pragma once
#include <glm.hpp>

#include "../abstract/AbstractComponent.h"

namespace eng {

struct TransformData {
	glm::vec2 position{};
};

} // !eng

namespace cpt {

/// <summary>
/// A component that allows Actors to store their local transform and observe their global transform.
/// </summary>
class Transform final : public eng::AbstractComponent {
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

	void FlagForGlobalUpdate();

	eng::TransformData const& GetLocal() const;
	eng::TransformData const& GetGlobal();

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Fields|----------------------------
	eng::TransformData m_TransformData{};
	eng::TransformData m_GlobalTransformData{};
	bool m_GlobalNeedsUpdate{};

}; // !TransformComponent

} // !cpt


//-----------------------------------------|Operators|------------------------------------

eng::TransformData operator+(const eng::TransformData& lhs, const eng::TransformData& rhs);