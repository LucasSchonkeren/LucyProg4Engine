#include "AbstractComponent.h"
#include "../Actor.h"

#include <algorithm>

namespace eng {

AbstractComponent::AbstractComponent(Actor& owner) : 
    m_Owner(owner) {
}

eng::Actor& AbstractComponent::Owner() {
    return m_Owner;
}

} // !eng
