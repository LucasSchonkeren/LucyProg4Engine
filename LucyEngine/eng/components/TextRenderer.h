#pragma once

#include "../abstract/AbstractComponent.h"
#include "../../dae/Texture2D.h"
#include "../../dae/Font.h"
#include <SDL.h>

#include <string>

namespace eng::cpt {

class TextRenderer final : public eng::AbstractComponent {
public: //---------------|Constructor/Destructor/copy/move|--------------
	TextRenderer(eng::Actor& owner, const std::string& text = "", const std::string& fontPath = "Lingua.otf", unsigned int size = 36, SDL_Color color = {255,255,255,255});
	~TextRenderer()	= default;

	TextRenderer(const TextRenderer&) = delete;
	TextRenderer& operator=	(const TextRenderer&)	= delete;

	TextRenderer				( TextRenderer&&)	= delete;
	TextRenderer& operator=		( TextRenderer&&)	= delete;

public: //---------------|Serialization Methods|--------------

	std::string TypeName() override { return "TextRenderer"; }
	nlohmann::ordered_json Serialize() override;
	static std::unique_ptr<TextRenderer> Deserialize(Actor& owner, const nlohmann::json& json);

public: //------------------|General methods|--------------------------

	void SetText(const std::string& text);

public: //------------------|Gameloop methods|--------------------------

	void Update() override;
	void Render() override;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Fields|----------------------------
	SDL_Color m_Color{ 255,255,255,255 }; // white
	std::string m_Text;
	std::string m_FontPath;
	unsigned int m_Size;
	bool m_NeedsUpdate{};
	dae::Font* m_FontPtr;
	u_ptr<dae::Texture2D> m_TextTextureUptr{};

}; // !TextRenderer

} // !cpt