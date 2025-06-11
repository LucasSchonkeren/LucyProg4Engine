#include "ModifyActorResource.h"

bool eng::cmd::ModifyActorResource::Execute(Actor& target) {
    if (!m_ResourceTrackerPtr or m_ResourceTrackerPtr->Owner() != target) m_ResourceTrackerPtr = target.GetComponent<cpt::ResourceTracker>();
    if (!m_ResourceTrackerPtr) return false; // Command fails to execute

    m_ResourceTrackerPtr->ModifyResource(m_Resource, m_Value);
    return true;
}
