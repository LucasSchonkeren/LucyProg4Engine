#pragma once

#include <set>
#include <memory>
#include <vector>
#include <bitset>
#include <cstddef>
#include <unordered_map>
#include <optional>

#include "../utils/utils.h"
#include "AbstractComponent.h"

namespace eng {

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
	Actor(Actor&&)				= default;
	Actor& operator=(Actor&&)	= default;

	///<summary>
	/// Create an exact duplicate of this actor and return a reference to the duplicate. 
	/// </summary>
	/*Actor& Clone();*/ //TODO

public: //--------------|Child Actor Methods|------------------
	
	/// <summary>
	/// Construct an empty child actor
	/// </summary>
	/// <returns> a reference to the created child actor</returns>
	Actor&			AddChildActor();

	/// <summary>
	/// Move construct a child actor
	/// </summary>
	/// <returns> a reference to the created child actor</returns>
	Actor&			AddChildActor(Actor&& child);

	/// <returns>
	/// Shallow search, gets only the children of this object. Empty if this Actor has no children.
	/// </returns>
	ref_vec<Actor>	GetChildren()								const;
	/// <returns>
	/// Deep search, gets the children of this object, and their children, recursively. Empty if this Actor has no children.
	/// </returns>
	ref_vec<Actor>	GetAllChildren()							const;

public: //---------------|Parent Actor Methods|-------------------------

	/// <returns>
	/// An optional reference to this Actor's parent Actor. If it has no parent, the optional is empty.
	///	</returns>
	optional_ref<Actor>		GetParent();
	/// <summary>
	/// Move ownership of this Actor to a different parent Actor.
	/// </summary>
	void					SetParent(Actor&);

public: //--------------|Component Methods|-----------------------------

	/// <summary>
	/// Add a component of type CompT to this actor. If such a component already exists, instead copies the given component to it.
	/// </summary>
	/// <typeparam name="CompT">The Component type. Must derive from cpt::AbstractComponent. Must be have valid copy/move constructors and assignment operators in order for the Actor to be able to be cloned</typeparam>
	/// <returns>A reference to the newly added component</returns>
	template <std::derived_from<AbstractComponent> CompT> requires requires { std::copyable<CompT>; }
	CompT&				AddComponent(CompT&& compUptr);
	
	/// <summary>
	/// Alternative AddComponent which calls the default constructor of the component type.
	/// </summary>
	/// <typeparam name="CompT">The Component type. Must derive from cpt::AbstractComponent. Must be have valid copy/move constructors and assignment operators in order for the Actor to be able to be cloned</typeparam>
	/// <returns>A reference to the newly added component</returns>
	template <std::derived_from<AbstractComponent> CompT> requires requires { std::copyable<CompT> && std::default_initializable<CompT>; }
	CompT&				AddComponent();

	/// <returns>
	/// An (optional) reference to this Actor's component of type CompT. Return value is empty if no such component exists.
	/// </returns>
	template <std::derived_from<AbstractComponent> CompT>
	optional_ref<CompT> GetComponent();

	/// <returns>
	/// A list of all componenets attached to this Actor.
	/// </returns>
	ref_vec<AbstractComponent> GetAbstractComponents();

public: //---------------------|Flag Enum/Methods|-----------------------------
	enum class Flags {
		///Flag for destruction at the end of the frame
		Destroyed,
		///Skip this Actor's Update cycle. Disables Update(), LateUpdate(), and FixedUpdate()
		NoUpdate,
		///Skip this Actor's Render cycle. Disable Render().
		NoRender,

		SIZE_
	};

	bool GetFlag(Flags flag)	const;	

	/// <summary>
	/// Flag this actor for destruction and call OnDestroy() on its components
	/// </summary>
	void Destroy();

public: //--------------------|Gameloop Methods|--------------------------------

	void Update();
	void LateUpdate();
	void FixedUpdate();
	void Render();

/*##################################|PRIVATE|##################################################*/

private: //--------------------|Child/Parent Actor Fields|-----------------------------------

	Actor* m_ParentPtr{};

	u_ptr_vec<Actor> m_ChildUptrs{};

private: //-----------------------|Component Fields|-------------------------------------

	u_ptr_type_map_unordered<AbstractComponent> m_CompUptrMap;

private: //-----------------------|Flag Fields|-------------------------------------------

	std::bitset<(int)Flags::SIZE_> m_Flags{};

}; // !Actor



//----------------------------------------------------------------------------------------
//---------------------|Template implementation|------------------------------------------
//----------------------------------------------------------------------------------------

template <std::derived_from<AbstractComponent> CompT> requires requires { std::copyable<CompT>; }
CompT& Actor::AddComponent(CompT&& comp) {
	if (auto pairIt = m_CompUptrMap.find(std::type_index(typeid(CompT))); pairIt != m_CompUptrMap.end()) {
		*(*pairIt).second = comp;
		return static_cast<CompT&>(*(*pairIt).second);
	}

	m_CompUptrMap[std::type_index(typeid(CompT))] = std::make_unique<CompT>(comp);
	m_CompUptrMap[std::type_index(typeid(CompT))]->SetOwner(*this);

	return static_cast<CompT&>(*m_CompUptrMap[std::type_index(typeid(CompT))]);
}

template<std::derived_from<AbstractComponent> CompT> requires requires { std::copyable<CompT>&& std::default_initializable<CompT>; }
CompT& Actor::AddComponent()
{
	return AddComponent<CompT>(CompT{});

}

template<std::derived_from<AbstractComponent> CompT>
inline optional_ref<CompT> Actor::GetComponent()
{
	optional_ref<CompT> f_Result{};

	if (auto pairIt = m_CompUptrMap.find(std::type_index(typeid(CompT))); pairIt != m_CompUptrMap.end()) {
		f_Result = static_cast<CompT&>(*(*pairIt).second);
	}

	return f_Result;
}


} // !namespace eng