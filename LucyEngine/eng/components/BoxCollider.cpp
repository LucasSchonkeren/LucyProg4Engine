

#include "BoxCollider.h"

#include "../utils/utils.h"
#include "../Actor.h"
#include "../engine/Services.h"
#include "../engine/Serialization.h"

namespace eng::cpt {

REGISTER_COMPONENT(BoxCollider);

nlohmann::ordered_json BoxCollider::Serialize() {
    nlohmann::ordered_json f_Json;
    f_Json["Size"] = {m_Size.x, m_Size.y};
    f_Json["Pivot"] = {m_Pivot.x, m_Pivot.y};
    return f_Json;
}

std::unique_ptr<BoxCollider> BoxCollider::Deserialize(Actor& owner, const nlohmann::json& json) {
    glm::vec2 f_Size{ json.value("Size", glm::vec2{}) };
    glm::vec2 f_Pivot{ json.value("Pivot", glm::vec2{}) };

    return std::make_unique<BoxCollider>(owner,f_Size, f_Pivot);
}

void eng::cpt::BoxCollider::Start() {
    Owner().GetTransform().AddObserver(*this);
    Owner().Game().Physics().RegisterCollider(*this);
}

void eng::cpt::BoxCollider::Render() {
#ifndef NDEBUG
    auto f_Renderer{ service::renderer.Get().GetSDLRenderer() };
    SDL_SetRenderDrawColor(f_Renderer, 255, 0, 0, 255); //red
    auto& f_Bounds{ Bounds() };
    SDL_FRect f_Rect{ f_Bounds.left, f_Bounds.top, f_Bounds.right - f_Bounds.left, f_Bounds.bottom - f_Bounds.top };
    SDL_RenderDrawRectF(f_Renderer, &f_Rect);
#endif // !NDEBUG

}

void eng::cpt::BoxCollider::OnEvent(Event event) {
    if (event.eventTypeHash != eventHash::PositionChanged) return;
    m_BoundsDirty = true;
}

const eng::physics::Boundsf& eng::cpt::BoxCollider::Bounds()
{
    if (m_BoundsDirty) {
        auto& f_TransformPos{ Owner().GetTransform().GetGlobal().position };

        m_Bounds.top = f_TransformPos.y - m_Pivot.y * m_Size.y;
        m_Bounds.bottom = f_TransformPos.y + m_Size.y - m_Pivot.y * m_Size.y;
        m_Bounds.right = f_TransformPos.x + m_Size.x - m_Pivot.x * m_Size.x;
        m_Bounds.left = f_TransformPos.x - m_Pivot.x * m_Size.x;

        m_BoundsDirty = false;
    }

    return m_Bounds;
}

void eng::cpt::BoxCollider::OnCollisionEnter(IAABBCollider* other) {
    m_Subject.DispatchEvent(Event{
        eventHash::OnCollisionEnter,
        std::make_any<eventContext::Collision>(*other)
        });
}

void eng::cpt::BoxCollider::OnCollision(IAABBCollider* other) {
    m_Subject.DispatchEvent(Event{
        eventHash::OnCollision,
        std::make_any<eventContext::Collision>(*other)
        });
}

void eng::cpt::BoxCollider::OnCollisionExit(IAABBCollider* other) {
    m_Subject.DispatchEvent(Event{
        eventHash::OnCollisionExit,
        std::make_any<eventContext::Collision>(*other)
        });
}

void eng::cpt::BoxCollider::AddObserver(IObserver& observer) {
    m_Subject.AddObserver(observer);
}

void eng::cpt::BoxCollider::RemoveObserver(IObserver&) {
}

} // !eng::cpt