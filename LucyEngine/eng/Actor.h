#pragma once

#include <set>
#include <memory>
#include <vector>
#include <bitset>
#include <cstddef>
#include <unordered_map>
#include <optional>
#include <ranges>
#include <algorithm>

#include "utils/utils.h"
#include "abstract/AbstractComponent.h"
#include "components/Transform.h"
#include "Game/AbstractGame.h"

namespace eng::eventHash {

/// <summary>
/// Event sent by an Actor when it becomes enabled. The EventContext is a pointer to the Actor.
/// </summary>
constexpr unsigned int ActorEnabled{ make_sdbm_hash("ActorEnabled") };
/// <summary>
/// Event sent by an Actor when it becomes disabled. The EventContext is a pointer to the Actor.
/// </summary>
constexpr unsigned int ActorDisabled{ make_sdbm_hash("ActorDisabled") };

}

namespace eng {

/// <summary>
/// The quintessential Game Object. An Actor manages the lifetime and ownership of Components and Child Actors.
/// </summary>
class Actor final {
public: //---------------|Helper struct|------------------------------
	struct MoveInfo {
		Actor* newParentPtr{};
		bool keepWorldTransform{};
	};

public: //---------------|Constructor/Destructor/copy/move|--------------
	
	Actor(AbstractGame& game);
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

	/// <returns>
	/// Shallow search, gets only the children of this object. Empty if this Actor has no children.
	/// </returns>
	ref_vec<Actor>	GetChildren()								const;

	/// <returns>
	/// Deep search, gets the children of this object, and their children, recursively. Empty if this Actor has no children.
	/// </returns>
	ref_vec<Actor>	GetAllChildren()							const;

	/// <summary>
	/// Remove a child actor. This destroys the child. May only be used during cleanup.
	/// </summary>
	void EraseChildActor(Actor* childPtr);

public: //---------------|Parent Actor Methods|-------------------------

	/// <returns>
	/// A pointer to this Actor's parent Actor. If it has no parent, returns nullptr.
	///	</returns>
	Actor*	GetParent();
	/// <summary>
	/// Move ownership of this Actor to a different parent Actor. Do not call this during the Update loop. Consider using eng::scenegraph::ChangeParent() instead
	/// </summary>
	void	SetParent(Actor& newParent, bool keepWorldTransform = true);
	/// <summary>
	/// Set the root Actor of this Actor's tree as this Actor's parent.
	/// </summary>
	void	SetParentToRoot();

	void	MoveToNewParent();

public: //--------------|Component Methods|-----------------------------

	/// <summary>
	/// Alternative AddComponent which calls the default constructor of the component type.
	/// </summary>
	/// <typeparam name="CompT">The Component type. Must derive from cpt::AbstractComponent. Must be have valid copy/move constructors and assignment operators in order for the Actor to be able to be cloned</typeparam>
	/// <returns>A reference to the newly added component</returns>
	template <std::derived_from<AbstractComponent> CompT, typename... ArgsT>
	CompT&				AddComponent(ArgsT... args);

	/// <returns>
	/// An (optional) reference to this Actor's component of type CompT. Return value is empty if no such component exists.
	/// </returns>
	template <std::derived_from<AbstractComponent> CompT>
	CompT* GetComponent();

	/// <returns>
	/// A list of all componenets attached to this Actor.
	/// </returns>
	ref_vec<AbstractComponent> GetAbstractComponents();

	///
	template <std::derived_from<AbstractComponent> CompT>
	void RemoveComponent();

	/// <summary>
	/// A helper method to easily get the transform of an actor
	/// </summary>
	cpt::Transform& GetTransform();

public: //---------------------|Flag Enum/Methods|-----------------------------
	enum class Flags {
		/// If this flag is set, the actor will be destroyed at end of frame
		Destroyed,
		/// If this flag is set, the actor will be moved to another parent at end of frame
		ParentChanged,
		/// Skip this Actor's Update cycle. Disables Update(), LateUpdate(), and FixedUpdate()
		NoUpdate,
		/// Skip this Actor's Render cycle. Disable Render().
		NoRender,
		/// If not set, runs start methods on its components and sets it at the start of the next frame.
		Started,
		/// If set, the actor is considered inactive, Controls Enable() and Disable().
		Disabled,
		/// If set, the actor will start disabled. Disable() will be called in Start(). Setting this flag has no effect if the Started flag has already been set.
		DisableOnStart,

		SIZE_
	};

	bool IsFlagged(Flags flag)	const;	

	/// <summary>
	/// Flag this actor for destruction and call OnDestroy() on its components
	/// </summary>
	void Destroy();

	/// <summary>
	/// Enable the actor and its children, if disabled. Unsets NoUpdate and NoRender and calls OnEnable().
	/// </summary>
	void Enable();

	/// <summary>
	/// Disable the actor and its children, if enabled. Sets NoUpdate and NoRender and calls OnDisable()
	/// </summary> 
	void Disable();

	/// <summary>
	/// Set this Actor to start enabled or disabled. By default, actors are enabled on start.
	/// </summary>
	void EnableOnStart(bool enable);

public: //--------------------|Observer Methods|--------------------------------

	void AddToggleObserver(IObserver& observer);
	void RemoveToggleObserver(IObserver& observer);

public: //--------------------|Gameloop Methods|--------------------------------

	void Start();
	void Update();
	void LateUpdate();
	void FixedUpdate();
	void Render(); 
	void RenderImgui();

public: //--------------------|Game Methods|--------------------------------

	AbstractGame& Game();
	float DeltaTime();

/*##################################|PRIVATE|##################################################*/

private: //--------------------|Child/Parent Actor Fields|-----------------------------------

	Actor* m_ParentPtr{};

	u_ptr_vec<Actor> m_ChildUptrs{};

	MoveInfo m_MoveInfo{};

private: //-----------------------|Component Fields|-------------------------------------

	u_ptr_vec<AbstractComponent> m_CompUptrs;

	cpt::Transform* m_TransformPtr;

private: //-----------------------|Flag Fields|-------------------------------------------

	std::bitset<static_cast<int>(Flags::SIZE_)> m_Flags{};

private: //-----------------------|Game Fields|-------------------------------------------

	AbstractGame& m_Game;

private: //-----------------------|Subject Fields|-------------------------------------------

	Subject m_ToggleSubject{};

}; // !Actor



//----------------------------------------------------------------------------------------
//---------------------|Template implementation|------------------------------------------
//----------------------------------------------------------------------------------------
#pragma region Template implementation

template<std::derived_from<AbstractComponent> CompT, typename... ArgsT>
CompT& Actor::AddComponent(ArgsT... args)
{
	// If a component of type CompT already exists, just return it
	for (auto& compUptr : m_CompUptrs) {
		if (CompT* castResult = dynamic_cast<CompT*>(compUptr.get()); castResult) {
			return *castResult;
		}
	}

	m_CompUptrs.emplace_back(std::make_unique<CompT>(*this, args...));

	return *static_cast<CompT*>(m_CompUptrs.back().get());

}

template<std::derived_from<AbstractComponent> CompT>
inline CompT* Actor::GetComponent()
{
	CompT* f_Result{};

	for (auto& compUptr : m_CompUptrs) {
		if (CompT* castResult = dynamic_cast<CompT*>(compUptr.get()); castResult) {
			f_Result = castResult;
			break;
		}
	}

	return f_Result;
}

template<std::derived_from<AbstractComponent> CompT>
inline void Actor::RemoveComponent() {
	std::ranges::remove_if(m_CompUptrs, [](u_ptr<AbstractComponent>& compUptr) {
		dynamic_cast<CompT>(compUptr);
		});
}

#pragma endregion

} // !namespace eng

//#############################################|Comparison Operator overloads|#################################

bool operator==(const eng::Actor& lhs, const eng::Actor& rhs);