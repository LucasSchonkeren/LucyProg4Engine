#include "AbstractComponent.h"
#include "Actor.h"

namespace eng {

void AbstractComponent::SetOwner(Actor& newOwner) {
    if (m_OwnerPtr) throw std::exception("This component is already owned.");
    m_OwnerPtr = &newOwner;
}

optional_ref<eng::Actor> AbstractComponent::GetOwner() {
    if (!m_OwnerPtr) return std::nullopt;
    return optional_ref<Actor>(*m_OwnerPtr);
}

} // !eng
