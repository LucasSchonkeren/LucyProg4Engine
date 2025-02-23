#include <stdexcept>
#include <SDL_ttf.h>
#include "Font.h"
#include <assert.h>


dae::Font::Font(const std::string& fullPath, unsigned int size) : m_font(nullptr)
{
	m_font = TTF_OpenFont(fullPath.c_str(), size);

	if (m_font == nullptr) throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
}

dae::Font::Font(_TTF_Font* font) : m_font{ font }
{
	assert(m_font != nullptr);
}

dae::Font::~Font()
{
	TTF_CloseFont(m_font);
}


_TTF_Font* dae::Font::GetFont() const {
	return m_font;
}