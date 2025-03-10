#include "Transform.h"
#include "../Actor.h"

namespace cpt {
void Transform::SetLocalPosition(float x, float y) {
	m_TransformData.position.x = x;
	m_TransformData.position.y = y;

	m_GlobalNeedsUpdate = true;
	for (auto child : GetOwner().GetAllChildren()) {
		child.get().GetTransform().FlagForGlobalUpdate();
	}
}
void Transform::SetLocalPosition(glm::vec2 newPosition) {
	SetLocalPosition(newPosition.x, newPosition.y);
}
void Transform::TranslatePosition(glm::vec2 translateVector) {
	m_TransformData.position += translateVector;
}
void Transform::FlagForGlobalUpdate() {
	m_GlobalNeedsUpdate = true;
}
eng::TransformData const& Transform::GetLocal() const
{
	return m_TransformData;
}

eng::TransformData const& Transform::GetGlobal() {
	if (!GetOwner().GetParent()) return m_TransformData;
	if (!m_GlobalNeedsUpdate) return m_GlobalTransformData;

	m_GlobalTransformData = GetLocal() + GetOwner().GetParent()->GetTransform().GetGlobal();

	return m_GlobalTransformData;
}

} // !cpt

eng::TransformData operator+(const eng::TransformData& lhs, const eng::TransformData& rhs) {
	return eng::TransformData{ lhs.position + rhs.position };
}