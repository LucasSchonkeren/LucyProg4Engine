#include "TextureRenderer.h"
#include "../../dae/Renderer.h"
#include "../Actor.h"
#include "Transform.h"

#include <filesystem>

namespace cpt {
void TextureRenderer::LoadTexture(const std::string& file) {
	const auto fullPath = std::filesystem::path("../Data/")/ file;
	m_TextureUptr = std::make_unique<dae::Texture2D>(fullPath.string());
}
void TextureRenderer::Render() {
	optional_ref<Transform> f_Tfv{ GetOwner() ? GetOwner().val_get().GetComponent<Transform>() : std::nullopt};

	const auto& f_Pos = f_Tfv ? f_Tfv.val_get().GetLocal().position : glm::vec3{};

	dae::Renderer::GetInstance().RenderTexture(*m_TextureUptr, f_Pos.x, f_Pos.y);
}

} // !cpt