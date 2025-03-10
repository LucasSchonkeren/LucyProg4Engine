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
	AbstractComponent(Actor& owner);
	virtual ~AbstractComponent() = default;

public: //----------------------|Serialization methods|------------------------
	virtual std::string Serialize() { return ""; };
	virtual void Deserialize(std::string serializationString) {};

public: //--------------------|Gameloop methods|-----------------------------

	virtual void Start() {};

	virtual void Update() {};
	virtual void LateUpdate() {};
	virtual void FixedUpdate() {};

	virtual void Render() {};
	virtual void RenderImgui() {};

	virtual void OnDestroy() {};

public: //---------------------|Relationship Methods|-------------------------
	Actor& GetOwner();

private:
	Actor& m_Owner;
}; // !AbstractComponent

} // !cpt