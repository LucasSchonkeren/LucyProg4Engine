#pragma once

#include "../abstract/AbstractComponent.h"

#include <unordered_map>

namespace eng::cpt { class ResourceTracker; } //forward decleration


namespace eng::eventContext {

struct ResourceChanged {
	std::string resource;
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

public: //---------------|Serialization Methods|--------------

	std::string TypeName() override { return "ResourceTracker"; }
	nlohmann::ordered_json Serialize() override;
	static std::unique_ptr<ResourceTracker> Deserialize(Actor& owner, const nlohmann::json& json);


public: //--------------------|Meta methods|-----------------------

	void NewResource(std::string resource, unsigned int maxValue, bool startEmpty = false);

public: //--------------------|Setter methods|-----------------------

	/// <summary>
	/// increase or decrease a resource of this Actor, clamped by 0 and its max valaue. Sends an "ActorResourceChanged" event if the resource is altered in this way.
	/// </summary>
	void ModifyResource(std::string resource, int value);

	/// <summary>
	/// Sets the resource value. Sends an "ActorResourceChanged" event if the resource is altered in this way.
	/// </summary>
	void SetResource(std::string resource, int value);

	/// <summary>
	/// Sets the resource to its max value. Sends an "ActorResourceChanged" event if the resource is altered in this way.
	/// </summary>
	void FillResource(std::string resource);

	void SetMaxResource(std::string resource, unsigned int value);

public: //--------------------|Getter methods|-----------------------

	int GetResource(std::string resource) const;

	bool IsResourceFull(std::string resource) const;

	bool IsResourceEmpty(std::string resource) const;

public: //--------------------|Subject methods|-----------------------

	void AddObserver(IObserver& observer);
	void RemoveObserver(IObserver& observer);

/*##################################|PRIVATE|##################################################*/

private: //---------------------------|Private fields|----------------------------

	std::unordered_map<std::string, int> m_ResourceVals{};
	std::unordered_map<std::string, int>m_MaxResourceVals{};
	Subject m_ResourceSubject{};

}; // !HealthTracker

} // !cpt