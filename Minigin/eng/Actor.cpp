#include "Actor.h"
#include "AbstractComponent.h"

#include <algorithm>

#include <iostream>

namespace eng {

Actor::Actor() {   
    m_TransformPtr = &AddComponent<cpt::Transform>();
}

Actor& Actor::AddChildActor() {
    m_ChildUptrs.emplace_back(std::make_unique<Actor>());

    m_ChildUptrs.back()->m_ParentPtr = this;

    return *m_ChildUptrs.back();
}

Actor& Actor::AddChildActor(Actor&& child) {
    m_ChildUptrs.push_back(std::make_unique<Actor>(std::move(child)));
    
    m_ChildUptrs.back()->m_ParentPtr = this;

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

void Actor::RemoveChildActor(Actor* childPtr) {
    assert(time::stage == time::Stages::Cleanup or time::stage == time::Stages::None and "Do not call RemoveChildActor outside of cleanup");

    std::erase_if(m_ChildUptrs, [childPtr](const u_ptr<Actor>& child) {
        return child.get() == childPtr;
    });
}

void Actor::SetParent(Actor& newParent) {
    if (!m_ParentPtr) {
        newParent.AddChildActor(std::move(*this));
        return;
    }

    std::unique_ptr<Actor>& f_OwningPointerPtr = *std::ranges::find_if(m_ParentPtr->m_ChildUptrs, [this](const std::unique_ptr<Actor>& child) {
        return child.get() == this; });

    newParent.m_ChildUptrs.push_back(std::move(f_OwningPointerPtr));

    std::erase_if(m_ParentPtr->m_ChildUptrs,  [this](const std::unique_ptr<Actor>& child) {
        return child.get() == nullptr; });
}

void Actor::SetParentToRoot() {
    if (!GetParent()) return;

    Actor* f_RootPtr{ GetParent() };

    while (f_RootPtr->GetParent()) {
        f_RootPtr = f_RootPtr->GetParent();
    }

    SetParent(*f_RootPtr);
}

Actor* eng::Actor::GetParent()
{
    return m_ParentPtr;
}

bool Actor::IsFlagged(Flags flag) const {
    return m_Flags[(int)flag];
}

void Actor::Destroy() {
    m_Flags.set(static_cast<int>(Flags::Destroyed));

    for (auto& child : m_ChildUptrs) {
        child->Destroy();
    }

    for (auto& compUptr : m_CompUptrs) {
        compUptr->OnDestroy();
    }
}

void Actor::Start() {
    for (auto& child : m_ChildUptrs) {
        child->Start();
    }

    if (IsFlagged(Flags::Started)) return;

    for (auto& compUptr : m_CompUptrs) {
        compUptr->Start();
    }

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
