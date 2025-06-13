#include "Transform.h"
#include "../Actor.h"

namespace eng::cpt {

void Transform::SetLocalPosition(float x, float y) {
	glm::vec2 f_OldPos{ m_TransformData.position };

	m_TransformData.position.x = x;
	m_TransformData.position.y = y;

	m_GlobalNeedsUpdate = true;
	for (auto child : Owner().GetAllChildren()) {
		child.get().GetTransform().FlagForGlobalUpdate();
	}

	m_Subject.DispatchEvent(Event{
		eng::eventHash::PositionChanged,
		std::make_any<eng::eventContext::PositionChanged>(
			this,
			f_OldPos,
			m_TransformData.position
		)
	});
}

void Transform::SetLocalPosition(glm::vec2 newPosition) {
	SetLocalPosition(newPosition.x, newPosition.y);
}

void Transform::TranslatePosition(glm::vec2 translateVector) {
	if (translateVector == glm::vec2{}) return;
	SetLocalPosition(m_TransformData.position + translateVector);
}

void Transform::SetGlobalPosition(float x, float y){
	SetGlobalPosition(glm::vec2{x, y});
}

void Transform::SetGlobalPosition(glm::vec2 newPosition) {
	auto f_TranslatePosition{newPosition - GetGlobal().position};
	TranslatePosition(newPosition);
}

void Transform::FlagForGlobalUpdate() {
	m_GlobalNeedsUpdate = true;
}
eng::TransformData const& Transform::GetLocal() const
{
	return m_TransformData;
}

eng::TransformData const& Transform::GetGlobal() {
	if (!Owner().GetParent()) return m_TransformData;
	if (!m_GlobalNeedsUpdate) return m_GlobalTransformData;

	m_GlobalTransformData = GetLocal() + Owner().GetParent()->GetTransform().GetGlobal();

	return m_GlobalTransformData;
}

void Transform::AddObserver(IObserver& observer) {
	m_Subject.AddObserver(observer);
}

void Transform::RemoveObserver(IObserver& observer) {
	m_Subject.RemoveObserver(observer);
}

} // !cpt

eng::TransformData operator+(const eng::TransformData& lhs, const eng::TransformData& rhs) {
	return eng::TransformData{ lhs.position + rhs.position };
}