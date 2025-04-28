#include "AbstractObserver.h"

void eng::Subject::AddObserver(IObserver& observer) {
	m_ObserverPtrs.emplace(&observer);
}

void eng::Subject::RemoveObserver(IObserver& observer) {
	m_ObserverPtrs.erase(&observer);
}

void eng::Subject::DispatchEvent(Event event) {
    for (auto* observer : m_ObserverPtrs) {
        if (observer) observer->OnEvent(event);
    }
}
