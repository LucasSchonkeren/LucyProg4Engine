#include "Transform.h"
#include "../Actor.h"
#include "../engine/Serialization.h"
#include <iostream>

namespace eng::cpt {

REGISTER_COMPONENT(Transform)

nlohmann::ordered_json Transform::Serialize()
{
	nlohmann::ordered_json f_Json{};
	
	f_Json["Position"] = m_TransformData.position;
	
	return f_Json;
}

std::unique_ptr<Transform> Transform::Deserialize(Actor& owner, const nlohmann::json& json) {
	std::unique_ptr<Transform> f_Uptr{ std::make_unique<Transform>(owner) };

	f_Uptr->SetLocalPosition(json.value("Position", glm::vec2{}));

	return std::move(f_Uptr);
}

void Transform::SetLocalPosition(float x, float y) {
	glm::vec2 f_OldPos{ m_TransformData.position };

	m_TransformData.position.x = x;
	m_TransformData.position.y = y;

	m_GlobalNeedsUpdate = true;
	for (auto child : Owner().GetAllChildren()) {
		child->GetTransform().FlagForGlobalUpdate();
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