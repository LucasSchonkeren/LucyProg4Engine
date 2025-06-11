#include <algorithm>
#include <ranges>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <future>

#include "Collisions.h"

void eng::physics::AABBCollisionHandler::RegisterCollider(IAABBCollider& collider) {
    const Boundsf& f_Bounds{ collider.Bounds() };
    m_ColliderXBounds.emplace_back(&collider, &f_Bounds.left, true);
    m_ColliderXBounds.emplace_back(&collider, &f_Bounds.right, false);
    m_ColliderYBounds.emplace_back(&collider, &f_Bounds.top, true);
    m_ColliderYBounds.emplace_back(&collider, &f_Bounds.bottom, false);
}

void eng::physics::AABBCollisionHandler::UnRegisterCollider(IAABBCollider& collider) {
    std::erase_if(m_ColliderXBounds, [&collider](BoundData& bound) { return bound.colliderPtr == &collider; });
    std::erase_if(m_ColliderYBounds, [&collider](BoundData& bound) { return bound.colliderPtr == &collider; });
}

void eng::physics::AABBCollisionHandler::NotifyCollisions() {
    static auto f_BoundCmp{ [](const BoundData& lhs, const BoundData& rhs) -> bool {
        return *lhs.boundPtr < *rhs.boundPtr;
    } };

    static auto f_PairCmp = [](const auto& a, const auto& b) {
        return std::tie(a.first, a.second) < std::tie(b.first, b.second);
        };

    std::ranges::sort(m_ColliderXBounds, f_BoundCmp);
    std::ranges::sort(m_ColliderYBounds, f_BoundCmp);



    auto f_AxisCollisionLambda{ [](std::vector<BoundData>& input) {
        std::set<IAABBCollider*> f_ActiveColliders{};
        std::vector<std::pair<IAABBCollider*, IAABBCollider*>> f_Output{};
        for (auto& boundData : input) {
            if (boundData.first) {
                for (auto key : f_ActiveColliders) {
                    f_Output.emplace_back(boundData.colliderPtr, key);
                }
                f_ActiveColliders.emplace(boundData.colliderPtr);
            }
            else {
                f_ActiveColliders.erase(boundData.colliderPtr);
            }
        }
        std::ranges::sort(f_Output, f_PairCmp);
        return f_Output;
    } };

    auto f_AsyncX{ std::async(std::launch::async, f_AxisCollisionLambda, std::ref(m_ColliderXBounds)) };
    auto f_AsyncY{ std::async(std::launch::async, f_AxisCollisionLambda, std::ref(m_ColliderYBounds)) };

    std::vector<std::pair<IAABBCollider*, IAABBCollider*>> f_XCollisions{ f_AsyncX.get() };
    std::vector<std::pair<IAABBCollider*, IAABBCollider*>> f_YCollisions{ f_AsyncY.get() };

    std::vector<std::pair<IAABBCollider*, IAABBCollider*>> f_ActualCollisions;
    std::ranges::set_intersection(
        f_XCollisions,
        f_YCollisions,
        std::back_inserter(f_ActualCollisions),
        f_PairCmp
    );

    std::vector<std::pair<IAABBCollider*, IAABBCollider*>> f_EnteredCollisions;
    std::ranges::set_difference(
        f_ActualCollisions,
        m_PrevCollisions,
        std::back_inserter(f_EnteredCollisions),
        f_PairCmp
    );

    std::vector<std::pair<IAABBCollider*, IAABBCollider*>> f_ExitedCollisions;
    std::ranges::set_difference(
        m_PrevCollisions,
        f_ActualCollisions,
        std::back_inserter(f_ExitedCollisions),
        f_PairCmp
    );

    for (auto& pair : f_EnteredCollisions) {
        pair.first->OnCollisionEnter(pair.second);
        pair.second->OnCollisionEnter(pair.first);
    }

    for (auto& pair : f_ActualCollisions) {
        pair.first->OnCollision(pair.second);
        pair.second->OnCollision(pair.first);
    }

    for (auto& pair : f_ExitedCollisions) {
        pair.first->OnCollisionExit(pair.second);
        pair.second->OnCollisionExit(pair.first);
    }

    m_PrevCollisions = f_ActualCollisions;
}

