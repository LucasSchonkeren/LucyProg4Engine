#pragma once

#include <concepts>
#include <optional>
#include <string>
#include <set>

#include "../utils/utils.h"
#include "AbstractObserver.h"

namespace eng {

class Actor; //forward declaration

/// <summary>
/// An abstract base class for Components
/// </summary>
class AbstractComponent : public AbstractSubject {
public: //------------------------|Virtual Destructor|------------------------
	AbstractComponent(Actor& owner);
	virtual ~AbstractComponent() = default;

public: //----------------------|Serialization methods|------------------------
	virtual std::string Serialize() { return ""; };
	virtual void Deserialize(std::string serializationString) {};

public: //--------------------|Subject methods|-----------------------------

	void AddObserver(AbstractObserver& observer) override;
	void RemoveObserver(AbstractObserver& observer) override;

protected:
	void DispatchEvent(Event event) override;

public: //--------------------|Gameloop methods|-----------------------------

	virtual void Init() {};
	virtual void Start() {};

	virtual void Update() {};
	virtual void LateUpdate() {};
	virtual void FixedUpdate() {};

	virtual void Render() {};
	virtual void RenderImgui() {};

	virtual void OnDestroy() {};

public: //---------------------|Relationship Methods|-------------------------
	Actor& GetOwner();

private: //--------------------|Fields|--------------------------
	Actor& m_Owner;
	std::vector<AbstractObserver*> m_ObserverPtrs{};
}; // !AbstractComponent

} // !cpt