#include "TextRenderer.h"
#include <stdexcept>
#include <SDL_ttf.h>
#include "../../dae/Renderer.h"
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
		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
				//	
				// Font and Size currently hardcoded, font is broken and I do not know why
				//
		const auto f_Surf = TTF_RenderText_Blended(m_FontPtr->GetFont(), m_Text.c_str(), color);

		if (f_Surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}

		auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), f_Surf);

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

	const auto& pos = Owner().GetTransform().GetLocal().position;
	dae::Renderer::GetInstance().RenderTexture(*m_TextTextureUptr, pos.x, pos.y);

}
