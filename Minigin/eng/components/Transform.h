#pragma once
#include <glm.hpp>

#include "../AbstractComponent.h"

namespace eng {

struct TransformData {
	glm::vec2 position{};
};

} // !eng

namespace cpt {

/// <summary>
/// A component that allows Actors to observe their global transform without storing a local transform.
/// </summary>
class TransformView : public eng::AbstractComponent {
	// TODO
};

/// <summary>
/// A component that allows Actors to store their local transform and observe their global transform.
/// </summary>
class Transform final : public TransformView {
public: //---------------|Constructor/Destructor/copy/move|--------------

	Transform() = default;
	~Transform() = default;

	//Transform				(const Transform&)	= default;
	//Transform& operator=	(const Transform&)	= default;

	//Transform				(const Transform&&)	= default;
	//Transform& operator=	(const Transform&&)	= default;

public: //---------------|General Methods|--------------
	void SetLocalPosition(float x, float y);

	eng::TransformData const& GetLocal() const;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Fields|----------------------------
	eng::TransformData m_TransformData;
}; // !TransformComponent

} // !cpt
