#include "SpriteController.h"
#include "../Actor.h"

namespace eng::cpt {
REGISTER_COMPONENT(SpriteController)

nlohmann::ordered_json SpriteController::Serialize() {
	nlohmann::ordered_json f_Json{};

	f_Json["Sprite"] = m_CurrentSprite;
	f_Json["Frame"] = m_CurrentFrame;
	f_Json["Size"] = { m_Size.x, m_Size.y };
	f_Json["SecPerFrame"] = m_SecPerFrame;
	
	for (auto& [sprite, sourcePositions] : m_SpriteSourcePositions) {
		for (auto& vec : sourcePositions)
			f_Json["SourcePositions"][sprite].emplace_back(nlohmann::json::array({ vec.x,vec.y }));
	}

	return f_Json;
}

std::unique_ptr<SpriteController> SpriteController::Deserialize(Actor& owner, const nlohmann::json& json)
{
	std::unordered_map<std::string, std::vector<glm::ivec2>> f_SourcePositions = [](const nlohmann::json& j) {
		std::unordered_map<std::string, std::vector<glm::ivec2>> result;

		if (!j.contains("SourcePositions") || !j["SourcePositions"].is_object())
			return result;

		for (const auto& [sprite, positions] : j["SourcePositions"].items()) {
			if (!positions.is_array()) continue;

			for (const auto& pos : positions) {
				if (pos.is_array() && pos.size() == 2 &&
					pos[0].is_number_integer() && pos[1].is_number_integer()) {
					result[sprite].emplace_back(pos[0].get<int>(), pos[1].get<int>());
				}
			}
		}
		return result;
		}(json);

	auto f_Uptr{ std::make_unique<SpriteController>(owner, json.value("Size", glm::ivec2{}), f_SourcePositions, json.value("Sprite", ""), json.value("SecPerFrame", 0.f)) };

	f_Uptr->m_CurrentFrame = json.value("Frame", 0);

	return std::move(f_Uptr);
}

void eng::cpt::SpriteController::Start() {
	m_TextureRendererPtr = Owner().GetComponent<TextureRenderer>();
}

void eng::cpt::SpriteController::Update() {
	m_SecLeftThisFrame -= Owner().DeltaTime();
	if (m_SecLeftThisFrame > 0) return;

	m_SecLeftThisFrame = m_SecPerFrame;
	++m_CurrentFrame;
	if (m_CurrentFrame >= m_SpriteSourcePositions[m_CurrentSprite].size()) m_CurrentFrame = 0;

	auto& f_Pos{ m_SpriteSourcePositions[m_CurrentSprite][m_CurrentFrame] };
	m_TextureRendererPtr->SetSourceRect({ f_Pos.x, f_Pos.y, m_Size.x, m_Size.y });
}

void eng::cpt::SpriteController::SetSprite(const std::string& spriteName) {
	m_CurrentSprite = spriteName;
	m_SecLeftThisFrame = m_SecPerFrame;
}

} // !eng::cpt