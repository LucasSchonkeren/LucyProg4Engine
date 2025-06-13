#include "Renderer.h"

static int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}


eng::Renderer::Renderer(SDL_Window* window) {
	m_window = window;
	m_renderer.reset(SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED));
	if (m_renderer == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void eng::Renderer::Render(eng::Actor& root) const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer.get());

	root.Render();

	SDL_RenderPresent(m_renderer.get());
}

void eng::Renderer::RenderTexture(const dae::Texture2D& texture, const SDL_Rect& screenRect, const SDL_Rect& TextureRect) const {
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &TextureRect, &screenRect);
}

SDL_Renderer* eng::Renderer::GetSDLRenderer() const { return m_renderer.get(); }
