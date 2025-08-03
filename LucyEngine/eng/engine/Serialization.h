#pragma once
#include <memory>
#include <unordered_map>
#include "../abstract/AbstractComponent.h"
#include <functional>
#include "../../../3rdParty/json.hpp"
#include <concepts>
#include <glm.hpp>
#include <SDL.h>

namespace eng {
class Actor;
}

namespace eng::serialization {

// Macros

#define REGISTER_COMPONENT(x) namespace { static bool registered__ = [] { \
	eng::serialization::RegisterComponentType<x>(#x); \
	return true; \
}();};

// Typedefs

using CompDeserializeFunc = std::function<std::unique_ptr<AbstractComponent>(Actor&, const nlohmann::json&)>;

// Concepts

template <typename T>
concept RegistrableComponent = std::derived_from<T, AbstractComponent>&&
	requires (T a, Actor& actor, const nlohmann::json& json) {
	{ T::Deserialize(actor, json) } -> std::convertible_to<std::unique_ptr<T>>;
};

// 'singleton' like function to ensure correct order of initialization 
std::unordered_map<std::string, CompDeserializeFunc>& DeserializeMethods();

// Function templates

template <RegistrableComponent T>
void RegisterComponentType(const std::string& typeName) {
	DeserializeMethods()[typeName] = &T::Deserialize;
}

template <typename T>
T JsonArrayGet(const nlohmann::json& json, size_t index, T defaultValue) {
	return (json.is_array() and json.size() > index) ? json[index].get<T>() : defaultValue;
}

// Free functions

std::unique_ptr<AbstractComponent> DeserializeComponent(Actor& owner, const std::string& name, const nlohmann::json& json);

bool IsComponentRegistered(const std::string& typeName);

} // !eng::serialization

// Json ADL serialization specializations

template <>
struct nlohmann::adl_serializer<glm::vec2> {
	static void from_json(const json& j, glm::vec2& vec) {
		if (j.is_array() && j.size() >= 2) {
			vec.x = eng::serialization::JsonArrayGet(j, 0, 0.f);
			vec.y = eng::serialization::JsonArrayGet(j, 1, 0.f);
		}
	}

	static void to_json(json& j, const glm::vec2& vec) {
		j = json::array({ vec.x, vec.y });
	}

	static void from_json(const ordered_json& j, glm::vec2& vec) {
		if (j.is_array() && j.size() >= 2) {
			vec.x = eng::serialization::JsonArrayGet(j, 0, 0.f);
			vec.y = eng::serialization::JsonArrayGet(j, 1, 0.f);
		}
	}

	static void to_json(ordered_json& j, const glm::vec2& vec) {
		j = json::array({ vec.x, vec.y });
	}
};

template <>
struct nlohmann::adl_serializer<glm::ivec2> {
	static void from_json(const json& j, glm::ivec2& vec) {
		if (j.is_array() && j.size() >= 2) {
			vec.x = eng::serialization::JsonArrayGet(j, 0, 0);
			vec.y = eng::serialization::JsonArrayGet(j, 1, 0);
		}
	}

	static void to_json(json& j, const glm::ivec2& vec) {
		j = json::array({ vec.x, vec.y });
	}

	static void from_json(const ordered_json& j, glm::ivec2& vec) {
		if (j.is_array() && j.size() >= 2) {
			vec.x = eng::serialization::JsonArrayGet(j, 0, 0);
			vec.y = eng::serialization::JsonArrayGet(j, 1, 0);
		}
	}

	static void to_json(ordered_json& j, const glm::ivec2& vec) {
		j = json::array({ vec.x, vec.y });
	}
};

template <>
struct nlohmann::adl_serializer<SDL_Color> {
	static void from_json(const json& j, SDL_Color& color) {
		if (j.is_array() && j.size() >= 2) {
			color.r = eng::serialization::JsonArrayGet(j, 0, static_cast<Uint8>(0));
			color.g = eng::serialization::JsonArrayGet(j, 1, static_cast<Uint8>(0));
			color.b = eng::serialization::JsonArrayGet(j, 0, static_cast<Uint8>(0));
			color.a = eng::serialization::JsonArrayGet(j, 1, static_cast<Uint8>(0));
		}
	}

	static void to_json(json& j, const SDL_Color& color) {
		j = json::array({ color.r, color.g, color.b, color.a });
	}

	static void from_json(const ordered_json& j, SDL_Color& color) {
		if (j.is_array() && j.size() >= 2) {
			color.r = eng::serialization::JsonArrayGet(j, 0, static_cast<Uint8>(0));
			color.g = eng::serialization::JsonArrayGet(j, 1, static_cast<Uint8>(0));
			color.b = eng::serialization::JsonArrayGet(j, 0, static_cast<Uint8>(0));
			color.a = eng::serialization::JsonArrayGet(j, 1, static_cast<Uint8>(0));
		}
	}

	static void to_json(ordered_json& j, const SDL_Color& color) {
		j = json::array({ color.r, color.g, color.b, color.a });
	}
};

template <>
struct nlohmann::adl_serializer<SDL_Rect> {
	static void from_json(const json& j, SDL_Rect& rect) {
		if (j.is_array() && j.size() >= 2) {
			rect.x = eng::serialization::JsonArrayGet(j, 0, static_cast<Uint8>(0));
			rect.y = eng::serialization::JsonArrayGet(j, 1, static_cast<Uint8>(0));
			rect.w = eng::serialization::JsonArrayGet(j, 0, static_cast<Uint8>(0));
			rect.h = eng::serialization::JsonArrayGet(j, 1, static_cast<Uint8>(0));
		}
	}

	static void to_json(json& j, const SDL_Rect& rect) {
		j = json::array({ rect.x, rect.y, rect.w, rect.h });
	}

	static void from_json(const ordered_json& j, SDL_Rect& rect) {
		if (j.is_array() && j.size() >= 2) {
			rect.x = eng::serialization::JsonArrayGet(j, 0, static_cast<Uint8>(0));
			rect.y = eng::serialization::JsonArrayGet(j, 1, static_cast<Uint8>(0));
			rect.w = eng::serialization::JsonArrayGet(j, 0, static_cast<Uint8>(0));
			rect.h = eng::serialization::JsonArrayGet(j, 1, static_cast<Uint8>(0));
		}
	}

	static void to_json(ordered_json& j, const SDL_Rect& rect) {
		j = json::array({ rect.x, rect.y, rect.w, rect.h });
	}

};