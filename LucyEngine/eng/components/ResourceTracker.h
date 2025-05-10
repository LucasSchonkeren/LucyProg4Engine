#pragma once

#include "../abstract/AbstractComponent.h"

#include <unordered_map>

namespace eng::cpt { class ResourceTracker; } //forward decleration


namespace eng::eventContext {

struct ResourceChanged {
	std::string_view resource;
	cpt::ResourceTracker* const sender;
	int delta;
};

}

namespace eng::cpt {

/// <summary>
/// A component to keep track of an arbitrary number of in-game 'resources', such as health or stamina. Resources are stored as integers and are clamped between 0 and a max value. If the max value is set to 0, there is no max value for that resource.
/// </summary>
class ResourceTracker final : public AbstractComponent {
public: //---------------|Constructor/Destructor/copy/move|--------------
	
	ResourceTracker(eng::Actor& owner) : AbstractComponent(owner) {};
	~ResourceTracker()	= default;

	ResourceTracker(const ResourceTracker&)	= delete;
	ResourceTracker& operator=	(const ResourceTracker&)	= delete;

	ResourceTracker(const ResourceTracker&&)	= delete;
	ResourceTracker& operator=	(const ResourceTracker&&)	= delete;

public: //--------------------|Meta methods|-----------------------

	void NewResource(std::string_view resource, unsigned int maxValue, bool startEmpty = false);

public: //--------------------|Setter methods|-----------------------

	/// <summary>
	/// increase or decrease a resource of this Actor, clamped by 0 and its max valaue. Sends an "ActorResourceChanged" event if the resource is altered in this way.
	/// </summary>
	void ModifyResource(std::string_view resource, int value);

	/// <summary>
	/// Sets the resource value. Sends an "ActorResourceChanged" event if the resource is altered in this way.
	/// </summary>
	void SetResource(std::string_view resource, int value);

	/// <summary>
	/// Sets the resource to its max value. Sends an "ActorResourceChanged" event if the resource is altered in this way.
	/// </summary>
	void FillResource(std::string_view resource);

	void SetMaxResource(std::string_view resource, unsigned int value);

public: //--------------------|Getter methods|-----------------------

	int GetResource(std::string_view resource) const;

	bool IsResourceFull(std::string_view resource) const;

	bool IsResourceEmpty(std::string_view resource) const;

public: //--------------------|Subject methods|-----------------------

	void AddObserver(IObserver& observer);
	void RemoveObserver(IObserver& observer);

public: //--------------------|Gameloop methods|-----------------------

	void Start() override;

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Private fields|----------------------------

	std::unordered_map<std::string_view, int> m_ResourceVals{}, m_MaxResourceVals{};
	Subject m_ResourceSubject{};

}; // !HealthTracker

} // !cpt