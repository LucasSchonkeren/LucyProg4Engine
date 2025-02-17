#include "TextRenderer.h"
#include <stdexcept>
#include <SDL_ttf.h>
#include "../../dae/TextObject.h"
#include "../../dae/Renderer.h"
#include "../../dae/Font.h"
#include "../../dae/Texture2D.h"
#include "../components/Transform.h"

#include <filesystem>

cpt::TextRenderer::TextRenderer(const std::string& text, const std::string& fontPath, unsigned int size) :
	m_Text(text), m_NeedsUpdate(true) {
	const auto fullPath = std::filesystem::path("../Data/")/fontPath;
	m_FontUptr = std::make_unique<dae::Font>(fullPath.string(), size);
}

void cpt::TextRenderer::SetText(const std::string& text) {
	m_Text = text;
	m_NeedsUpdate = true;
}

void cpt::TextRenderer::Update() {
	if (m_NeedsUpdate) {
		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
				//	
				// Font and Size currently hardcoded, font is broken and I do not know why
				//
		const auto f_Surf = TTF_RenderText_Blended(TTF_OpenFont(std::string("../Data/Lingua.otf").c_str(), 36), m_Text.c_str(), color);

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

void cpt::TextRenderer::Render() {
	if (m_TextTextureUptr == nullptr) return;

	const auto& pos = GetOwner() && GetOwner().val_get().GetComponent<Transform>() ? GetOwner().val_get().GetComponent<Transform>().val_get().GetLocal().position : glm::vec2{};
	dae::Renderer::GetInstance().RenderTexture(*m_TextTextureUptr, pos.x, pos.y);

}
