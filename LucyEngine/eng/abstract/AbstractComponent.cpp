#include "AbstractComponent.h"
#include "../Actor.h"

#include <algorithm>

namespace eng {

AbstractComponent::AbstractComponent(Actor& owner) : 
    m_Owner(owner) {
}

void AbstractComponent::AddObserver(AbstractObserver& observer) {
    m_ObserverPtrs.emplace_back(&observer);
}

void AbstractComponent::RemoveObserver(AbstractObserver& observer) {
    std::erase(m_ObserverPtrs, &observer);
}

void AbstractComponent::DispatchEvent(Event event) {
    for (auto observerPtr : m_ObserverPtrs) {
        observerPtr->OnEvent(event);
    }
}

eng::Actor& AbstractComponent::GetOwner() {
    return m_Owner;
}

} // !eng
