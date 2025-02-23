#pragma once

#include "../../dae/Texture2D.h"
#include "../../dae/Font.h"

namespace eng::resources {

void			Init();

dae::Texture2D* LoadTexture(const std::string& file);

dae::Font*		LoadFont(const std::string& file, uint8_t size);

void			ClearAllResources();

} // !eng::resources