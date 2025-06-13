#pragma once

#pragma once

#include <concepts>
#include <vector>

namespace eng::physics {

struct Boundsf final {
	float left;
	float top;
	float right;
	float bottom;
};

class AABBCollisionHandler;

class IAABBCollider {
public:
	virtual ~IAABBCollider() = default;

	virtual const Boundsf&	Bounds()								= 0;
	virtual void			OnCollisionEnter(IAABBCollider* other)	= 0;
	virtual void			OnCollision(IAABBCollider* other)		= 0;
	virtual void			OnCollisionExit(IAABBCollider* other)	= 0;
	virtual const bool		IsSolid()								= 0;
};

class AABBCollisionHandler final {
	struct BoundData {
		IAABBCollider* colliderPtr;
		const float* boundPtr;
		bool first;
	};
public:
	void RegisterCollider(IAABBCollider& collider);
	void UnRegisterCollider(IAABBCollider& collider);
	void NotifyCollisions();
private:
	std::vector<BoundData> m_ColliderXBounds{};
	std::vector<BoundData> m_ColliderYBounds{};
	std::vector<std::pair<IAABBCollider*, IAABBCollider*>> m_PrevCollisions;
};

}