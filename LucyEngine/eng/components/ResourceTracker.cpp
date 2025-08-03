#include "ResourceTracker.h"
#include "../utils/SdbmHash.h"
#include "../engine/serialization.h"

#include <algorithm>

namespace eng::cpt {
REGISTER_COMPONENT(ResourceTracker)

nlohmann::ordered_json eng::cpt::ResourceTracker::Serialize() {
	nlohmann::ordered_json f_Json;

	for (auto& [name, value] : m_ResourceVals) {
		f_Json[name]["Value"] = value;
	}
	for (auto& [name, value] : m_MaxResourceVals) {
		f_Json[name]["Max"] = value;
	}

	return f_Json;
}

std::unique_ptr<ResourceTracker> eng::cpt::ResourceTracker::Deserialize(Actor& owner, const nlohmann::json& json) {
	auto f_Uptr{std::make_unique<ResourceTracker>(owner)};

	if (!json.is_object()) return f_Uptr;

	for (auto& [name, value] : json.items()) {
		f_Uptr->m_ResourceVals[name] = value.value("Value", 0);
		f_Uptr->m_MaxResourceVals[name] = value.value("Max", INT_MAX);
	}

	return std::move(f_Uptr);
}

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

} // !eng::cpt