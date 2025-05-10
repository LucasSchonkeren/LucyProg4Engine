#pragma once
#include "../abstract/AbstractComponent.h"
#include "TextRenderer.h"
#include "ResourceTracker.h"
#include "../abstract/AbstractObserver.h"

namespace eng::cpt {

/// <summary>
/// A component to keep track of an arbitrary number of in-game 'resources', such as health or stamina. Resources are stored as integers and have are clamped between 0 and a max value. If the max value is set to 0, there is no max value for that resource.
/// </summary>
class ResourceDisplayLogic final : public eng::AbstractComponent, public eng::IObserver {
public: //---------------|Constructor/Destructor/copy/move|--------------

	ResourceDisplayLogic(eng::Actor& owner, std::string_view formatString) : AbstractComponent(owner), m_FormatString(formatString) {};
	~ResourceDisplayLogic()	= default;

	ResourceDisplayLogic			(const ResourceDisplayLogic&)  = delete;
	ResourceDisplayLogic& operator=	(const ResourceDisplayLogic&)  = delete;

	ResourceDisplayLogic			(const ResourceDisplayLogic&&) = delete;
	ResourceDisplayLogic& operator=	(const ResourceDisplayLogic&&) = delete;

public: //--------------------|Setter methods|-----------------------------

	void SetText(std::string_view formatString);

public: //--------------------|Gameloop methods|-----------------------------

	void Start() override;

public: //--------------------|Observer methods|-----------------------------

	void OnEvent(Event event);

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|methods|----------------------------

	void UpdateTextComponent(ResourceTracker* const resourceTrackerPtr);

private: //---------------------------|fields|----------------------------
	
	std::string						m_FormatString;
	TextRenderer*					m_TextRendererPtr{};

}; // !HealthTracker

} // !cpt