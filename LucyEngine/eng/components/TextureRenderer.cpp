#include "TextureRenderer.h"
#include "../../dae/Renderer.h"
#include "../Actor.h"
#include "Transform.h"
#include "../../eng/engine/Resources.h"

#include <filesystem>

namespace eng::cpt {
void TextureRenderer::LoadTexture(const std::string& file) {
	m_TexturePtr = eng::service::resources.Get().LoadTexture(file);
}
void TextureRenderer::Render() {
	const auto& f_Pos = Owner().GetTransform().GetGlobal().position;

	dae::Renderer::GetInstance().RenderTexture(*m_TexturePtr, f_Pos.x, f_Pos.y);
}

} // !cpt