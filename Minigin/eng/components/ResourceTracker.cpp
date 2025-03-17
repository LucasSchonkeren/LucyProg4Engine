#include "ResourceTracker.h"
#include "../utils/SdbmHash.h"

#include <algorithm>

void eng::cpt::ResourceTracker::NewResource(std::string_view resource, unsigned int maxValue, bool startEmpty) {
	if (maxValue == 0) maxValue = INT_MAX;
	m_MaxResourceVals[resource] = maxValue;
	m_ResourceVals[resource] = startEmpty ? 0 : maxValue;
	DispatchEvent({
			*this,
			eng::eventHash::actorResourceChanged,
			std::make_any<eng::eventContext::ResourceChanged>(
				resource,
				0
			)
		});
}

void eng::cpt::ResourceTracker::ModifyResource(std::string_view resource, int value) {
	int f_OldVal{ m_ResourceVals.at(resource) };
	m_ResourceVals.at(resource) += value;
	m_ResourceVals.at(resource) = std::clamp(m_ResourceVals.at(resource), 0, m_MaxResourceVals.at(resource));
	if (f_OldVal != m_ResourceVals.at(resource)) {
		DispatchEvent({
			*this,
			eng::eventHash::actorResourceChanged,
			std::make_any<eng::eventContext::ResourceChanged>(
				resource,
				f_OldVal - m_ResourceVals.at(resource)
			)
		});
	}
}

void eng::cpt::ResourceTracker::SetResource(std::string_view resource, int value) {
	int f_OldVal{ m_ResourceVals.at(resource) };
	m_ResourceVals.at(resource) = value;
	if (f_OldVal != m_ResourceVals.at(resource)) {
		DispatchEvent({
			*this,
			eng::eventHash::actorResourceChanged,
			std::make_any<eng::eventContext::ResourceChanged>(
				resource,
				f_OldVal - m_ResourceVals.at(resource)
			)
		});
	}
}

void eng::cpt::ResourceTracker::FillResource(std::string_view resource) {
	int f_OldVal{ m_ResourceVals.at(resource) };
	m_ResourceVals.at(resource) = m_MaxResourceVals[resource];
	if (f_OldVal != m_ResourceVals.at(resource)) {
		DispatchEvent({
			*this,
			eng::eventHash::actorResourceChanged,
			std::make_any<eng::eventContext::ResourceChanged>(
				resource,
				f_OldVal - m_ResourceVals.at(resource)
			)
		});
	}
}

void eng::cpt::ResourceTracker::SetMaxResource(std::string_view resource, unsigned int value) {
	if (value == 0) value = INT_MAX;
	m_MaxResourceVals.at(resource) = value;
}

int eng::cpt::ResourceTracker::GetResource(std::string_view resource) const {
	if (m_ResourceVals.count(resource) == 0) return -1;
	return m_ResourceVals.at(resource);
}

bool eng::cpt::ResourceTracker::IsResourceFull(std::string_view resource) const {
	return m_ResourceVals.at(resource) == m_MaxResourceVals.at(resource);
}

bool eng::cpt::ResourceTracker::IsResourceEmpty(std::string_view resource) const {
	return m_ResourceVals.at(resource) == 0;
}

void eng::cpt::ResourceTracker::Start() {
	for (auto& pair : m_ResourceVals)
		DispatchEvent({
			*this,
			eng::eventHash::actorResourceChanged,
			std::make_any<eng::eventContext::ResourceChanged>(
				pair.first,
				0
			)
		});
}
