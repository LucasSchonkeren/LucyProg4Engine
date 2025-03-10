#pragma once

#include "../abstract/AbstractComponent.h"
#include "../../dae/Texture2D.h"
#include "../../dae/Font.h"

#include <string>

namespace cpt {

class TextRenderer final : public eng::AbstractComponent {
public: //---------------|Constructor/Destructor/copy/move|--------------
	TextRenderer(eng::Actor& owner, const std::string& text = "", const std::string& fontPath = "Lingua.otf", unsigned int size = 36);
	~TextRenderer()	= default;

	TextRenderer(const TextRenderer&) = delete;
	TextRenderer& operator=	(const TextRenderer&)	= delete;

	TextRenderer				( TextRenderer&&)	= delete;
	TextRenderer& operator=		( TextRenderer&&)	= delete;

public: //------------------|General methods|--------------------------

	void SetText(const std::string& text);

public: //------------------|Gameloop methods|--------------------------

	void Update() override;
	void Render() override;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Fields|----------------------------
	std::string m_Text;
	bool m_NeedsUpdate{};
	dae::Font* m_FontPtr;
	u_ptr<dae::Texture2D> m_TextTextureUptr{};
}; // !TextRenderer

} // !cpt