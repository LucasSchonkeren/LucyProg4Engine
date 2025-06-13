#pragma once

#include "../abstract/AbstractComponent.h"
#include "../../dae/Texture2D.h"
#include <SDL.h>

namespace eng::cpt {

class TextureRenderer final : public eng::AbstractComponent {
public: //---------------|Constructor/Destructor/copy/move|--------------

	TextureRenderer(eng::Actor& owner, const std::string& texturePath, glm::ivec2 size = {-1, -1}, SDL_Rect sourceRect = 
		{-1, -1, -1, -1});
	~TextureRenderer() override = default;

	TextureRenderer(const TextureRenderer& other) = delete;
	TextureRenderer& operator=	(const TextureRenderer&) = delete;

	TextureRenderer				(TextureRenderer&&) = delete;
	TextureRenderer& operator=	(TextureRenderer&&) = delete;

public: //------------------------|Texture Methods|-------------------------

	void LoadTexture(const std::string& file);
	void SetSourceRect(SDL_Rect rect);

public: //------------------------|Gameloop Methods|-------------------------

	void Render() override;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Fields|----------------------------

	dae::Texture2D* m_TexturePtr{};
	glm::ivec2 m_Size{};
	SDL_Rect m_SourceRect{};

}; // !TextureRenderer

} // !cpt