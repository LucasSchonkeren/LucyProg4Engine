#include "TextureRenderer.h"
#include "../Actor.h"
#include "Transform.h"
#include "../../eng/engine/Resources.h"

#include <filesystem>

namespace eng::cpt {
void TextureRenderer::LoadTexture(const std::string& file) {
	m_TexturePtr = eng::service::resources.Get().LoadTexture(file);
}
void TextureRenderer::SetSourceRect(SDL_Rect rect) {
	m_SourceRect = rect;
}
TextureRenderer::TextureRenderer(eng::Actor& owner, const std::string& texturePath, glm::ivec2 size, SDL_Rect sourceRect) : AbstractComponent(owner), m_Size(size), m_SourceRect(sourceRect) {
	LoadTexture(texturePath);
	auto f_TextureSize{ m_TexturePtr->GetSize() };
	if (m_Size.x == -1)  m_Size = f_TextureSize;
	if (m_SourceRect.x == -1) m_SourceRect = { 0,0, f_TextureSize.x, f_TextureSize.y };
}
void TextureRenderer::Render() {
	const auto& f_Pos = Owner().GetTransform().GetGlobal().position;

	eng::service::renderer.Get().RenderTexture(*m_TexturePtr, SDL_Rect{static_cast<int>(f_Pos.x), static_cast<int>(f_Pos.y), m_Size.x, m_Size.y}, m_SourceRect);
}

} // !cpt