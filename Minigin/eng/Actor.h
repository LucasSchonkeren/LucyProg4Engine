#pragma once

#include <set>
#include <memory>

#include "../utils/utils.h"

namespace eng {

class AbstractComponent;

/// <summary>
/// The quintessential Game Object. An Actor manages the lifetime and ownership of Components and Child Actors.
/// </summary>
class Actor final {
public: //---------------|Constructor/Destructor/copy/move|--------------
	
	Actor()		= default;
	~Actor()	= default;

	// Actors cannot be copied
	Actor(const Actor&)				= delete;
	Actor& operator=(const Actor&)	= delete;

	// Actors can be moved
	Actor(Actor&&)			= default;
	Actor& operator=(Actor&&) = default;

	///<summary>
	/// Create an exact duplicate of this actor and return a reference to the duplicate. 
	/// </summary>
	Actor& Clone();

public: //--------------|Child Actor Methods|------------------
	
	void				AddChildActor(stdUptr<Actor> childUptr);
	/// <returns>
	/// Shallow search, gets only the children of this object. Empty if this Actor has no children.
	/// </returns>
	std::vector<Actor&>	GetChildren()								const;
	/// <returns>
	/// Deep search, gets the children of this object, and their children, recursively. Empty if this Actor has no children.
	/// </returns>
	std::vector<Actor&>	GetAllChildren()							const;

public: //-------------|Parent Actor Methods|--------------------

	/// <returns>
	/// Whether of not this child has a parent. If false, this is the root object of an Actor tree.
	/// </returns>
	bool		HasParent() const;
	/// <returns>
	/// A reference to this Actor's parent Actor. If it has no parent, returns a reference to itself instead.
	///	</returns>
	Actor&		GetParent() const;
	/// <summary>
	/// Move ownership of this Actor to a different parent Actor.
	/// </summary>
	void		SetParent(Actor&);

public: //--------------|Component Management Methods|------------------

	/// <typeparam name="CompT">The Component type. Must derive from AbstractComponent. Must be have valid copy/move constructors and assignment operators in order for the Actor to be able to be cloned</typeparam>
	/// <returns>A reference to the newly added component</returns>
	template <std::derived_from<AbstractComponent> CompT> requires requires { std::copyable<CompT>; }
	CompT& AddComponent(stdUptr<CompT> compUptr);
	
	/// <summary>
	/// Alternative AddComponent which calls the default constructor of the component type.
	/// </summary>
	/// <typeparam name="CompT">The Component type. Must derive from AbstractComponent. Must be have valid copy/move constructors and assignment operators in order for the Actor to be able to be cloned</typeparam>
	/// <returns>A reference to the newly added component</returns>
	template <std::derived_from<AbstractComponent> CompT> requires requires { std::copyable<CompT> && std::default_initializable<CompT>; }
	CompT& AddComponent();

	/// <returns>
	/// Get a vector of references to all components of type CompT. These references are guaranteed to be valid as long as the owning Actor exists.
	/// Components can safely store references to other components on the same actor, as components are only deleted when the entire Actor is destroyed.
	/// </returns>
	template <std::derived_from<AbstractComponent> CompT>
	std::vector<CompT&> GetComponent();

public: //---------------------|Flag Enum/Methods|-----------------------------
	enum class Flags {
		//Flag for destruction at the end of the frame
		Destroyed,
		//Skip this Actor's Update cycle. Disables Update(), LateUpdate(), and FixedUpdate()
		NoUpdate,
		//Skip this Actor's Render cycle. Disable Render().
		NoRender,

		SIZE_
	};

	void SetFlag(Flags flag);

	void UnsetFlag(Flags flag);

	bool GetFlag(Flags flag)	const;	

	//Helper methods
	/// <summary>
	/// Flag this actor for destruction
	/// </summary>
	void Destroy();

}; // !Actor



//----------------------------------------------------------------------------------------
//---------------------|Template implementation|------------------------------------------
//----------------------------------------------------------------------------------------


template <std::derived_from<AbstractComponent> CompT> requires requires { std::copyable<CompT>; }
CompT& Actor::AddComponent(stdUptr<CompT> compUptr) {

}


} // !namespace eng




