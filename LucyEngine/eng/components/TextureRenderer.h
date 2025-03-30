#pragma once

#include "../abstract/AbstractComponent.h"
#include "../../dae/Texture2D.h"

namespace eng::cpt {

class TextureRenderer final : public eng::AbstractComponent {
public: //---------------|Constructor/Destructor/copy/move|--------------

	TextureRenderer(eng::Actor& owner) : AbstractComponent(owner) {};
	TextureRenderer(eng::Actor& owner, std::string texturePath) : AbstractComponent(owner) { LoadTexture(texturePath); };
	~TextureRenderer() override = default;

	TextureRenderer(const TextureRenderer& other) = delete;
	TextureRenderer& operator=	(const TextureRenderer&) = delete;

	TextureRenderer				(TextureRenderer&&) = delete;
	TextureRenderer& operator=	(TextureRenderer&&) = delete;

public: //------------------------|Texture Methods|-------------------------

	void LoadTexture(const std::string& file);

public: //------------------------|Gameloop Methods|-------------------------

	void Render() override;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Fields|----------------------------

	dae::Texture2D* m_TexturePtr{};

}; // !TextureRenderer

} // !cpt