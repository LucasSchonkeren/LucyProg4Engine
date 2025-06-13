#pragma once

#include "../abstract/AbstractComponent.h"
#include "TextureRenderer.h"

class eng::Actor;

namespace eng::cpt {

class SpriteController final : public eng::AbstractComponent {
public: //---------------|Constructor/Destructor/copy/move|--------------

	SpriteController(eng::Actor& owner, glm::ivec2 size, std::unordered_map<std::string, std::vector<glm::ivec2>> sourceData, const std::string& startSprite, float secondsPerFrame = 0) : AbstractComponent(owner), m_Size(size), m_SpriteSourcePositions(sourceData), m_SecPerFrame(secondsPerFrame), m_SecLeftThisFrame(secondsPerFrame), m_CurrentSprite(startSprite) {};
	~SpriteController() = default;

	SpriteController(const SpriteController&) = delete;
	SpriteController& operator=	(const SpriteController&) = delete;

	SpriteController(SpriteController&&) = delete;
	SpriteController& operator=	(SpriteController&&) = delete;

public: //------------------|Gameloop Methods|--------------------------
	void Start() override;
	void Update() override;

public: //------------------|Sprite Methods|--------------------------
	void SetSprite(const std::string& spriteName);

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Component references|----------------------------

	TextureRenderer* m_TextureRendererPtr{};

private: //---------------------------|Sprite fields|----------------------------

	std::string m_CurrentSprite;
	size_t m_CurrentFrame{};
	glm::ivec2 m_Size;
	std::unordered_map<std::string, std::vector<glm::ivec2>> m_SpriteSourcePositions;
	const float m_SecPerFrame;
	float m_SecLeftThisFrame;

}; // !FpsTracker

} // !cpt