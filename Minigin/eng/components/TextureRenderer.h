#pragma once

#include "../AbstractComponent.h"
#include "../../dae/Texture2D.h"

namespace cpt {

class TextureRenderer final : public eng::AbstractComponent {
public: //---------------|Constructor/Destructor/copy/move|--------------

	TextureRenderer() = default;
	~TextureRenderer() override = default;

	TextureRenderer(const TextureRenderer& other) {
		m_TextureUptr = other.m_TextureUptr ? std::make_unique<dae::Texture2D>(other.m_TextureUptr->GetSDLTexture()) : nullptr;
	}
	TextureRenderer& operator=	(const TextureRenderer&)	= default;

	TextureRenderer				(TextureRenderer&&)	= default;
	TextureRenderer& operator=	(TextureRenderer&&)	= default;

public: //------------------------|Texture Methods|-------------------------

	void LoadTexture(const std::string& file);

public: //------------------------|Gameloop Methods|-------------------------

	void Render() override;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Fields|----------------------------

	u_ptr<dae::Texture2D> m_TextureUptr{};

}; // !TextureRenderer

} // !cpt