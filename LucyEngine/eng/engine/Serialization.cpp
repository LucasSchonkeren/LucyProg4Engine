#include "Serialization.h"
#include "../Actor.h"

namespace eng::serialization {

std::unordered_map<std::string, CompDeserializeFunc>& serialization::DeserializeMethods() {
	static std::unordered_map<std::string, CompDeserializeFunc> deserializeMethodsByName{};
	return deserializeMethodsByName;
}

std::unique_ptr<AbstractComponent> DeserializeComponent(Actor& owner, const std::string& name, const nlohmann::json& json) {
	return DeserializeMethods().at(name)(owner, json);
}

bool serialization::IsComponentRegistered(const std::string& name) {
	return DeserializeMethods().contains(name);
}

} // !serialization