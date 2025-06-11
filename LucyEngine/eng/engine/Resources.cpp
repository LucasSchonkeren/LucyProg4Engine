#include "Resources.h"
#include <SDL_ttf.h>
#include <string>
#include <stdexcept>
#include <filesystem>
#include <map>
#include "../utils/utils.h"
#include "../../dae/Texture2D.h"
#include "../../dae/Font.h"

namespace eng {

eng::SdlResourceLoader::SdlResourceLoader() {
	if (TTF_Init() != 0) {
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

eng::SdlResourceLoader::~SdlResourceLoader() {
	m_TextureUptrs.clear();
	m_FontUptrs.clear();
}

dae::Texture2D* eng::SdlResourceLoader::LoadTexture(const std::string& file)
{
	const auto fullPath = "../Data/" + file;

	const auto filename = std::filesystem::path(fullPath).filename().string();

	if (m_TextureUptrs.find(filename) == m_TextureUptrs.end()) m_TextureUptrs.insert(std::pair(filename, std::make_unique<dae::Texture2D>(fullPath)));

	return m_TextureUptrs.at(filename).get();
}

dae::Font* eng::SdlResourceLoader::LoadFont(const std::string& file, uint8_t size)
{
	const auto fullPath = "../Data/" + file;

	const auto filename = std::filesystem::path(fullPath).filename().string();

	const auto key = std::pair<std::string, uint8_t>(filename, size);

	if (m_FontUptrs.find(key) == m_FontUptrs.end()) m_FontUptrs.insert(std::make_pair(key, std::make_unique<dae::Font>(fullPath, size)));

	return m_FontUptrs.at(key).get();
}

}