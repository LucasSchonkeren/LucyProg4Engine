#pragma once

#include <concepts>
#include <optional>
#include <string>

#include "../utils/utils.h"

namespace eng {

class Actor; //forward declaration

/// <summary>
/// An abstract base class for Components
/// </summary>
class AbstractComponent {
public: //------------------------|Virtual Destructor|------------------------
	virtual ~AbstractComponent() = default;

public: //----------------------|Serialization methods|------------------------
	virtual std::string Serialize() { return ""; };
	virtual void Deserialize(std::string serializationString) {};

public: //--------------------|Gameloop methods|-----------------------------

	virtual void Update() {};
	virtual void LateUpdate() {};
	virtual void FixedUpdate() {};

	virtual void Render() {};

	virtual void OnDestroy() {};

public: //---------------------|Relationship Methods|-------------------------
	/// <summary>
	/// Sets the Actor this Component is a part of. Throws an exception if this object already has an owner.
	/// </summary>
	void SetOwner(Actor& newOwner);

	optional_ref<Actor> GetOwner();

	//template<std::derived_from<AbstractComponent> CompT>
	//optional_ref<CompT> GetSibling() {
	//	if (!m_OwnerPtr) return std::nullopt;
	//	return m_OwnerPtr->GetComponent<CompT>();
	//}

private:
	Actor* m_OwnerPtr{};
}; // !AbstractComponent

} // !cpt