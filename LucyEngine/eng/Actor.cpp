#include "Actor.h"

#include <algorithm>

#include <iostream>

namespace eng {

Actor::Actor(AbstractGame& game) :
    m_Game(game) {   
    m_TransformPtr = &AddComponent<cpt::Transform>();
}

Actor& Actor::AddChildActor() {
    auto& f_NewChild{ m_ChildUptrs.emplace_back(std::make_unique<Actor>(m_Game)) };

    f_NewChild->m_ParentPtr = this;

    return *m_ChildUptrs.back();
}

ref_vec<Actor> Actor::GetChildren() const {
    ref_vec<Actor> f_Result{};
    for (const auto& childUptr : m_ChildUptrs) {
        f_Result.push_back(*childUptr);
    }
    return f_Result;
}

ref_vec<Actor> Actor::GetAllChildren() const {
    ref_vec<Actor> f_Result = GetChildren();

    for (size_t i{}; i < f_Result.size(); ++i) {
        auto f_ChildChildren{ f_Result[i].get().GetChildren() };
        f_Result.insert(f_Result.end(), f_ChildChildren.begin(), f_ChildChildren.end());
    }

    return f_Result;
}

void Actor::EraseChildActor(Actor* childPtr) {
    std::erase_if(m_ChildUptrs, [childPtr](const u_ptr<Actor>& child) {
        return child.get() == childPtr;
    });
}

void Actor::SetParent(Actor& newParent, bool keepWorldTransform) {
    // Assert that this call is valid
    assert(m_ParentPtr          and "Root Actors cannot be assigned the child of another actor.");
    assert(&newParent != this   and "An actor cannot be its own parent");

    auto f_Children{ GetAllChildren() };
    while (!f_Children.empty()) {
        assert(&f_Children.back().get() != this and "An actor cannot be a child of its children");
        f_Children.pop_back();
    }

    m_Game.ActorGraph().FlagActorNewParent(this);
    m_Flags.set(static_cast<int>(Flags::ParentChanged));
    m_MoveInfo.newParentPtr = &newParent;
    m_MoveInfo.keepWorldTransform = keepWorldTransform;
}

void Actor::SetParentToRoot() {
    assert(m_ParentPtr and "This actor is itself root actor");

    Actor* f_RootPtr{ GetParent() };

    while (f_RootPtr->GetParent()) {
        f_RootPtr = f_RootPtr->GetParent();
    }

    SetParent(*f_RootPtr);
}

void Actor::MoveToNewParent() {
    if (m_MoveInfo.newParentPtr == nullptr) return;

    // Allow world transform to be kept consistent
    if (m_MoveInfo.keepWorldTransform) {
        GetTransform().SetLocalPosition(GetTransform().GetGlobal().position - m_MoveInfo.newParentPtr->GetTransform().GetGlobal().position);
    }

    // Fetch the unique pointer that owns this actor and move it into the new parent
    std::unique_ptr<Actor>& f_OwningPointer = *std::ranges::find_if(m_ParentPtr->m_ChildUptrs, [this](const std::unique_ptr<Actor>& child) {
        return child.get() == this; });
    m_MoveInfo.newParentPtr->m_ChildUptrs.emplace_back(std::move(f_OwningPointer));

    // Erase the now empty unique pointer in our old parent
    std::erase_if(m_ParentPtr->m_ChildUptrs, [this](const std::unique_ptr<Actor>& child) {
        return child.get() == nullptr; });

    // Inform this actor of its new parent
    m_ParentPtr = m_MoveInfo.newParentPtr;

    // Flag this actor's transform as dirty
    GetTransform().FlagForGlobalUpdate();

    // Moving complete
    m_MoveInfo.newParentPtr = nullptr;
}

Actor* eng::Actor::GetParent()
{
    return m_ParentPtr;
}

bool Actor::IsFlagged(Flags flag) const {
    return m_Flags.test(static_cast<int>(flag));
}

void Actor::Destroy() {
    if (IsFlagged(Flags::Destroyed) or m_ParentPtr == nullptr) return;
    m_Flags.set(static_cast<int>(Flags::Destroyed));

    for (auto& child : m_ChildUptrs) {
        if (child->IsFlagged(Flags::Destroyed)) m_Game.ActorGraph().UnFlagActorDestroy(child.get());
        child->Destroy();
    }

    Disable();

    for (auto& compUptr : m_CompUptrs) {
        compUptr->OnDestroy();
    }
}

void Actor::Enable() {
    for (auto& child : m_ChildUptrs) {
        child->Enable();
    }

    if (not IsFlagged(Flags::Disabled) and not IsFlagged(Flags::Started)) return;

    m_Flags.reset(static_cast<int>(Flags::Disabled));
    m_Flags.reset(static_cast<int>(Flags::NoRender));
    m_Flags.reset(static_cast<int>(Flags::NoUpdate));

    for (auto& compUptr : m_CompUptrs) {
        compUptr->OnEnable();
    }

    m_ToggleSubject.DispatchEvent({
        eventHash::ActorEnabled,
        std::make_any<Actor*>(this)
    });
}

void Actor::Disable() {
    for (auto& child : m_ChildUptrs) {
        child->Disable();
    }

    if (IsFlagged(Flags::Disabled)) return;

    m_Flags.set(static_cast<int>(Flags::NoRender));
    m_Flags.set(static_cast<int>(Flags::Disabled));
    m_Flags.set(static_cast<int>(Flags::NoUpdate));

    for (auto& compUptr : m_CompUptrs) {
        compUptr->OnDisable();
    }

    m_ToggleSubject.DispatchEvent({
        eventHash::ActorDisabled,
        std::make_any<Actor*>(this)
        });
}

void Actor::EnableOnStart(bool enable) {
    m_Flags[static_cast<int>(Flags::DisableOnStart)] = !enable;
}

void Actor::AddToggleObserver(IObserver& observer) {
    m_ToggleSubject.AddObserver(observer);
}

void Actor::RemoveToggleObserver(IObserver& observer) {
    m_ToggleSubject.AddObserver(observer);
}

void Actor::Start() {
    for (auto& child : m_ChildUptrs) {
        child->Start();
    }

    if (IsFlagged(Flags::Started)) return;

    for (auto& compUptr : m_CompUptrs) {
        compUptr->Start();
    }

    Enable();
    if (IsFlagged(Flags::DisableOnStart)) Disable();

    m_Flags[static_cast<int>(Flags::Started)] = true;
}

void Actor::Update() {
    if (IsFlagged(Flags::NoUpdate)) return;

    for (auto& child : m_ChildUptrs) {
        child->Update();
    }

    for (auto& compUptr : m_CompUptrs) {
        compUptr->Update();
    }
}

void Actor::LateUpdate() {
    if (IsFlagged(Flags::NoUpdate)) return;

    for (auto& child : m_ChildUptrs) {
        child->LateUpdate();
    }

    for (auto& compUptr : m_CompUptrs) {
        compUptr->LateUpdate();
    }
}

void Actor::FixedUpdate() {
    if (IsFlagged(Flags::NoUpdate)) return;

    for (auto& child : m_ChildUptrs) {
        child->FixedUpdate();
    }

    for (auto& compUptr : m_CompUptrs) {
        compUptr->FixedUpdate();
    }
    
}

void Actor::Render() {
    if (IsFlagged(Flags::NoRender)) return;

    for (auto& compUptr : m_CompUptrs) {
        compUptr->Render();
    }

    for (auto& child : m_ChildUptrs) {
        child->Render();
    }
}   

void Actor::RenderImgui() {
    if (IsFlagged(Flags::NoRender)) return;

    for (auto& compUptr : m_CompUptrs) {
        compUptr->RenderImgui();
    }

    for (auto& childUptr : m_ChildUptrs) {
        childUptr->RenderImgui();
    }
}

AbstractGame& Actor::Game() {
    return m_Game;
}

float Actor::DeltaTime() {
    return static_cast<float>(m_Game.Time().DeltaTime());
}

ref_vec<AbstractComponent> Actor::GetAbstractComponents()
{
    ref_vec<AbstractComponent> f_Result{};

    for (auto& compUptr : m_CompUptrs) {
        f_Result.emplace_back(*compUptr);
    }

    return f_Result;
}

cpt::Transform& Actor::GetTransform()
{
    return *m_TransformPtr;
}

} // namespace eng

bool operator==(const eng::Actor& lhs, const eng::Actor& rhs) {
    return &lhs == &rhs;
}


//---------------------------------------------------------------------------
//-----------------------------|Unit Tests|----------------------------------
//---------------------------------------------------------------------------
#ifdef NOSDL

#include "../Libs/catch.hpp"

class TestComponent1 : public eng::AbstractComponent {
	std::string Serialize() override { return ""; }
	void Deserialize(std::string) override {}
};
class TestComponent2 : public eng::AbstractComponent {
	std::string Serialize() override { return ""; }
	void Deserialize(std::string) override {}
};

SCENARIO("Components can be added and references to them retrieved from Actors", "[Actor][Component]") {
	GIVEN("An empty actor") {
		eng::Actor actor{};

		WHEN("Asking for any components") {
			optional_ref<TestComponent1> testComp1{ actor.GetComponent<TestComponent1>() };
			optional_ref<TestComponent2> testComp2{ actor.GetComponent<TestComponent2>() };


			THEN("None should be found") {
				REQUIRE(not testComp1);
				REQUIRE(not testComp2);
				REQUIRE(actor.GetAbstractComponents().size() == 0);

			}
		}

		WHEN("Adding an arbitrary number of components") {
			actor.AddComponent<TestComponent1>();

			actor.AddComponent<TestComponent2>();
			actor.AddComponent(TestComponent2{});
			actor.AddComponent(TestComponent2());

			AND_WHEN("Asking for any components") {
				optional_ref<TestComponent1> testComp1{ actor.GetComponent<TestComponent1>() };
				optional_ref<TestComponent2> testComp2{ actor.GetComponent<TestComponent2>() };

				THEN("Exactly one of each type should be found") {
					REQUIRE(testComp1);
					REQUIRE(testComp2);
					REQUIRE(actor.GetAbstractComponents().size() == 2);
				}
			}
		}
	}
}

SCENARIO("Child Actors can be added and references to them retrieved from parent Actors") {
	GIVEN("An empty root Actor") {
		eng::Actor actor{};

        REQUIRE(not actor.GetParent());
        REQUIRE(actor.GetChildren().size() == 0);

        WHEN("An arbitrary number of actors are added to the actor tree") {
            actor.AddChildActor(); //Add a single child actor
            THEN("The amount of child actors should match") { REQUIRE(actor.GetAllChildren().size() == 1); }

            actor.AddChildActor().AddChildActor().AddChildActor(); // add 3 empty child actors in a branch (total 4)
            THEN("The amount of child actors should match") { REQUIRE(actor.GetAllChildren().size() == 4); }

            eng::Actor& tempActor = actor.AddChildActor(); 
            tempActor.AddChildActor();
            tempActor.AddChildActor().AddChildActor(); //Add a child that branches off into more (total 8)
            THEN("The amount of child actors should match") { REQUIRE(actor.GetAllChildren().size() == 8); }

            AND_WHEN("Creating a seperate actor with its own children") {
                eng::Actor actor2{};
          
                actor2.AddChildActor().AddChildActor();

                eng::Actor& newActor = actor.AddChildActor(std::move(actor2)); // Move actor2 and its 2 children into actor (total 11)

                THEN("The amount of child actors should match") { REQUIRE(actor.GetAllChildren().size() == 11); }
                THEN("That actor should be a child of the first") { 
                    REQUIRE(newActor.GetParent().has_value());
                    REQUIRE(&newActor.GetParent().value().get() == &actor);

                }

            }

        }
	}
}

#endif // !NOSDL

