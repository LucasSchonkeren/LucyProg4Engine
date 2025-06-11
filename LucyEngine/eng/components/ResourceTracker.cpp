#include "ResourceTracker.h"
#include "../utils/SdbmHash.h"

#include <algorithm>

void eng::cpt::ResourceTracker::NewResource(std::string resource, unsigned int maxValue, bool startEmpty) {
	if (maxValue == 0) maxValue = INT_MAX;
	m_MaxResourceVals[resource] = maxValue;
	m_ResourceVals[resource] = startEmpty ? 0 : maxValue;
	m_ResourceSubject.DispatchEvent({
			eng::eventHash::actorResourceChanged,
			std::make_any<eng::eventContext::ResourceChanged>(
				resource,
				this,
				0
			)
		});
}

void eng::cpt::ResourceTracker::ModifyResource(std::string resource, int value) {
	int f_OldVal{ m_ResourceVals.at(resource) };
	m_ResourceVals.at(resource) += value;
	m_ResourceVals.at(resource) = std::clamp(m_ResourceVals.at(resource), 0, m_MaxResourceVals.at(resource));
	if (f_OldVal != m_ResourceVals.at(resource)) {
		m_ResourceSubject.DispatchEvent({
			eng::eventHash::actorResourceChanged,
			std::make_any<eng::eventContext::ResourceChanged>(
				resource,
				this,
				f_OldVal - m_ResourceVals.at(resource)
			)
		});
	}
}

void eng::cpt::ResourceTracker::SetResource(std::string resource, int value) {
	int f_OldVal{ m_ResourceVals.at(resource) };
	m_ResourceVals.at(resource) = value;
	if (f_OldVal != m_ResourceVals.at(resource)) {
		m_ResourceSubject.DispatchEvent({
			eng::eventHash::actorResourceChanged,
			std::make_any<eng::eventContext::ResourceChanged>(
				resource,
				this, 
				f_OldVal - m_ResourceVals.at(resource)
			)
		});
	}
}

void eng::cpt::ResourceTracker::FillResource(std::string resource) {
	int f_OldVal{ m_ResourceVals.at(resource) };
	m_ResourceVals.at(resource) = m_MaxResourceVals[resource];
	if (f_OldVal != m_ResourceVals.at(resource)) {
		m_ResourceSubject.DispatchEvent({
			eng::eventHash::actorResourceChanged,
			std::make_any<eng::eventContext::ResourceChanged>(
				resource,
				this,
				f_OldVal - m_ResourceVals.at(resource)
			)
		});
	}
}

void eng::cpt::ResourceTracker::SetMaxResource(std::string resource, unsigned int value) {
	if (value == 0) value = INT_MAX;
	m_MaxResourceVals.at(resource) = value;
}

int eng::cpt::ResourceTracker::GetResource(std::string resource) const {
	if (m_ResourceVals.count(resource) == 0) return -1;
	return m_ResourceVals.at(resource);
}

bool eng::cpt::ResourceTracker::IsResourceFull(std::string resource) const {
	return m_ResourceVals.at(resource) == m_MaxResourceVals.at(resource);
}

bool eng::cpt::ResourceTracker::IsResourceEmpty(std::string resource) const {
	return m_ResourceVals.at(resource) == 0;
}

void eng::cpt::ResourceTracker::AddObserver(IObserver& observer) {
	m_ResourceSubject.AddObserver(observer);
}

void eng::cpt::ResourceTracker::RemoveObserver(IObserver& observer) {
	m_ResourceSubject.RemoveObserver(observer);
}

void eng::cpt::ResourceTracker::Start() {
	for (auto& pair : m_ResourceVals)
		m_ResourceSubject.DispatchEvent({
			eng::eventHash::actorResourceChanged,
			std::make_any<eng::eventContext::ResourceChanged>(
				pair.first,
				this,
				0
			)
		});
}
