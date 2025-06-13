#include "TextRenderer.h"
#include <stdexcept>
#include <SDL_ttf.h>
#include "../../dae/Font.h"
#include "../../dae/Texture2D.h"
#include "../components/Transform.h"
#include <cstdint>
#include "../engine/Resources.h"


#include <filesystem>

eng::cpt::TextRenderer::TextRenderer(eng::Actor& owner, const std::string& text, const std::string& fontPath, unsigned int size) :
	AbstractComponent(owner),
	m_Text(text), m_NeedsUpdate(true) {
	m_FontPtr = eng::service::resources.Get().LoadFont(fontPath, static_cast<uint8_t>(size));
}

void eng::cpt::TextRenderer::SetText(const std::string& text) {
	m_Text = text;
	m_NeedsUpdate = true;
}

void eng::cpt::TextRenderer::Update() {
	if (m_NeedsUpdate) {
		const auto f_Surf = TTF_RenderText_Blended(m_FontPtr->GetFont(), m_Text.c_str(), m_Color);

		if (f_Surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}

		auto texture = SDL_CreateTextureFromSurface(eng::service::renderer.Get().GetSDLRenderer(), f_Surf);

		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}

		SDL_FreeSurface(f_Surf);
		m_TextTextureUptr = std::make_unique<dae::Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void eng::cpt::TextRenderer::Render() {
	if (m_TextTextureUptr == nullptr) return;

	const glm::ivec2 f_Pos{ Owner().GetTransform().GetLocal().position };
	const auto f_Size{ m_TextTextureUptr->GetSize() };
	eng::service::renderer.Get().RenderTexture(*m_TextTextureUptr, SDL_Rect{ f_Pos.x, f_Pos.y, f_Size.x, f_Size.y }, SDL_Rect{0, 0, f_Size.x, f_Size.y});

}
