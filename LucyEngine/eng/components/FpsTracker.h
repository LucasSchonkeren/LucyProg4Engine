#pragma once

#include "../abstract/AbstractComponent.h"
#include "TextRenderer.h"

class eng::Actor; 

namespace eng::cpt {

class FpsTracker final : public eng::AbstractComponent {
public: //---------------|Constructor/Destructor/copy/move|--------------

	FpsTracker(eng::Actor& owner) : AbstractComponent(owner) {};
	~FpsTracker() = default;

	FpsTracker(const FpsTracker&) = delete;
	FpsTracker& operator=	(const FpsTracker&) = delete;

	FpsTracker(FpsTracker&&) = delete;
	FpsTracker& operator=	(FpsTracker&&) = delete;

public: //---------------|Serialization Methods|--------------

	std::string TypeName() override { return "FpsTracker"; }
	static std::unique_ptr<FpsTracker> Deserialize(Actor& owner, const nlohmann::json& json);

public: //------------------|Gameloop Methods|--------------------------
	void Start() override;
	void Update() override;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Component references|----------------------------

	TextRenderer* m_TextRendererPtr{};

private: //---------------------------|Type registry|----------------------------

	static bool REGISTERED;

}; // !FpsTracker

} // !cpt