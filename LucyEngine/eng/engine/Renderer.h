#pragma once
#include <SDL.h>
#include "../Actor.h"
#include "glm.hpp"
#include "Services.h"

struct SdlRendererDeleter {
	void operator()(SDL_Renderer* p) const {
		SDL_DestroyRenderer(p);
	}
};

namespace dae {
class Texture2D;
}

namespace eng {

class Renderer final : public service::IRenderer {
public:
	Renderer(SDL_Window* window);

	void Render(eng::Actor&) const;

	void RenderTexture(const dae::Texture2D& texture, const SDL_Rect& screenRect, const SDL_Rect& TextureRect) const;

	SDL_Renderer* GetSDLRenderer() const;

	const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
	void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }

private:
	std::unique_ptr<SDL_Renderer, SdlRendererDeleter> m_renderer{};
	SDL_Window* m_window{};
	SDL_Color m_clearColor{};
};
}

