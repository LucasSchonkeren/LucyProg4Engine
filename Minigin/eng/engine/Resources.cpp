#include "Resources.h"
#include <SDL_ttf.h>
#include <string>
#include <stdexcept>
#include <filesystem>
#include <map>
#include "../utils/utils.h"
#include "../../dae/Texture2D.h"
#include "../../dae/Font.h"

namespace eng::resources {

//-------------------|Namespace variables|--------------------


std::map<std::string, u_ptr<dae::Texture2D>> textureUptrs{};
std::map<std::pair<std::string, uint8_t>, u_ptr<dae::Font>> fontUptrs{};

//---------------------|Namespace methods|--------------------

void eng::resources::Init() {
	if (TTF_Init() != 0) {
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

dae::Texture2D* LoadTexture(const std::string& file) {
	const auto fullPath = "../Data/" + file;

	const auto filename = std::filesystem::path(fullPath).filename().string();

	if (textureUptrs.find(filename) == textureUptrs.end()) textureUptrs.insert(std::pair(filename, std::make_unique<dae::Texture2D>(fullPath)));

	return textureUptrs.at(filename).get() ;
}

dae::Font* LoadFont(const std::string& file, uint8_t size) {
	const auto fullPath = "../Data/" + file;

	const auto filename = std::filesystem::path(fullPath).filename().string();

	const auto key = std::pair<std::string, uint8_t>(filename, size);

	if (fontUptrs.find(key) == fontUptrs.end()) fontUptrs.insert(std::pair(key, std::make_unique<dae::Font>(fullPath, size)));

	return fontUptrs.at(key).get();
}

void ClearAllResources() {
	textureUptrs.clear();
	fontUptrs.clear();
}

}