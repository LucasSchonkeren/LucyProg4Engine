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
class AbstractComponent {
public: //------------------------|Virtual Destructor|------------------------
	AbstractComponent(Actor& owner);
	virtual ~AbstractComponent() = default;

public: //----------------------|Serialization methods|------------------------
	virtual std::string Serialize() { return ""; };
	virtual void Deserialize(std::string serializationString) {};

public: //--------------------|Gameloop methods|-----------------------------

	/// <summary>
	/// Called once before the first frame of the owning object. This method should be used to initialize the component internally with its Actor.
	/// </summary>
	virtual void Start() {};
	/// <summary>
	/// Called when the object becomes enabled from a disabled state, including during Start() (even if the object starts in a disabled state) and when the owning Actor is finished moving across the scenegraph to a new parent. Any initializing that depends on the scenegraph should happen here to ensure a valid state for the component.
	/// </summary>
	virtual void OnEnable() {};

	/// <summary>
	/// Called once per frame.
	/// </summary>
	virtual void Update() {}; 
	/// <summary>
	/// Called once per frame, after Update() has been called on every other Actor/Component.
	/// </summary>
	virtual void LateUpdate() {}; 
	/// <summary>
	/// Called once per 'physics tick', should be used for any physics dependent code. May be called 0 or more times per update. NOT IMPLEMENTED
	/// </summary>
	virtual void FixedUpdate() {}; 

	/// <summary>
	/// Called once per frame by the renderer. Any rendering should go here.
	/// </summary>
	virtual void Render() {};
	/// <summary>
	/// Called once per frame by the renderer, after all other rendering. Any Imgui rendering should go here. NOT IMPLEMENTED
	/// </summary>
	virtual void RenderImgui() {};

	/// <summary>
	/// Called when the object becomes disabled from a enabled state, including during Start() if the objects starts disabled and before the owning Actor begins moves across the scenegraph to a new parent. Any initializing done in OnEnable() should be undone here to keep the component in a valid state.
	/// </summary>
	virtual void OnDisable() {};
	virtual void OnDestroy() {};

public: //---------------------|Relationship Methods|-------------------------

	Actor& Owner();

private: //--------------------|Fields|--------------------------
	Actor& m_Owner;
}; // !AbstractComponent

class AbstractState {
public:
	virtual ~AbstractState() = default;

	virtual void Update() {};
	virtual void LateUpdate() {};
	virtual void FixedUpdate() {};
	virtual void Render() {};
};

} // !eng