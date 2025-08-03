#include "MovementHandler.h"
#include "../Actor.h"
#include <iostream>
#include "../engine/Services.h"

namespace eng::cpt {
REGISTER_COMPONENT(MovementHandler)

nlohmann::ordered_json MovementHandler::Serialize() {
	nlohmann::ordered_json f_Json;
	f_Json["Speed"] = m_Speed;
	return f_Json;
}

std::unique_ptr<MovementHandler> MovementHandler::Deserialize(Actor& owner, const nlohmann::json& json) {
	float f_Speed{ json.value("Speed", 0.f)};
	return std::make_unique<MovementHandler>(owner, f_Speed);
}

void eng::cpt::MovementHandler::Start() {
	m_BoxColliderPtr = Owner().GetComponent<BoxCollider>();
	Owner().Game().Physics().RegisterCollider(*this);
}

void eng::cpt::MovementHandler::Update() {
	m_LastDirection = m_Direction;
}

void eng::cpt::MovementHandler::LateUpdate() {

	if (m_Direction == glm::vec2{} or m_Speed == 0) return;
	Owner().GetTransform().TranslatePosition(glm::normalize(m_Direction) * m_Speed);
	m_Direction = glm::vec2{ 0,0 };
}

void eng::cpt::MovementHandler::Render() {
#ifndef NDEBUG
	if (m_LastDirection == glm::vec2{}) return;
	auto f_Renderer{ service::renderer.Get().GetSDLRenderer() };
	SDL_SetRenderDrawColor(f_Renderer, 0, 0, 255, 255); //blue
	auto& f_Bounds{ Bounds() };
	SDL_FRect f_Rect{ f_Bounds.left, f_Bounds.top, f_Bounds.right - f_Bounds.left, f_Bounds.bottom - f_Bounds.top };
	SDL_RenderDrawRectF(f_Renderer, &f_Rect);
#endif
}

void eng::cpt::MovementHandler::SetDirection(glm::vec2 direction) {
	m_Direction = direction;
}

void eng::cpt::MovementHandler::SetSpeed(float speed) {
	m_Speed = speed;
}

const eng::physics::Boundsf& eng::cpt::MovementHandler::Bounds() {
	if (m_LastDirection == glm::vec2{}) return m_Bounds;
	m_Bounds = m_BoxColliderPtr->Bounds();
	auto f_Velocity = glm::normalize(m_LastDirection) * m_Speed;

	m_Bounds.top += f_Velocity.y + 1;
	m_Bounds.bottom += f_Velocity.y - 1;
	m_Bounds.left += f_Velocity.x + 1;
	m_Bounds.right += f_Velocity.x - 1;
	return m_Bounds;
}

void eng::cpt::MovementHandler::OnCollisionEnter(IAABBCollider*) {
}

void eng::cpt::MovementHandler::OnCollision(IAABBCollider* other) {
	auto f_BoxCollider{ dynamic_cast<BoxCollider*>(other) };
	if (f_BoxCollider and f_BoxCollider->Owner() == Owner()) return;
	m_Direction = glm::vec2{};
}

void eng::cpt::MovementHandler::OnCollisionExit(IAABBCollider*) {
}


} // !eng::cpt