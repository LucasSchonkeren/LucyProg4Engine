#pragma once

#include <vector>
#include <optional>
#include <concepts>
#include <functional>
#include <forward_list>

template <typename T>
class WeakReference final {
	static_assert(std::is_base_of_v<WeaklyReferable<T>, T>, "WeakReference can only refer to objects that derive from WeaklyReferable");
public: //---------------------|Constructor/Destructor|-----------------------

	WeakReference(T& referred) : m_Reference(referred) {
		referred.m_References.push_front(*this);
	}

	~WeakReference() {
		if (m_Reference) m_Reference.value().get().m_References.remove_if( [&](auto ref) { return &ref.get() == this; } );
	}
	
public: //---------------------|Implicit conversion operators|-----------------------

	operator T&()	const	{ return Get(); }
	operator bool() const	{ return m_Reference.has_value(); }

public: //---------------------|General Methods|--------------------------

	T&		Get()	const	{ return m_Reference.value().get(); }
	void	Invalidate()	{ m_Reference.reset(); }

/*##################################|PRIVATE|##################################################*/

private: //-------------------------|Private fields|----------------------------
	std::optional<std::reference_wrapper<T>> m_Reference;
};

/// <summary>
/// Classes must inherit from this base class in order for weak references to bid to them.
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
class WeaklyReferable {
	friend class WeakReference<T>; // Allow WeakReference to add/remove itself from the reference list without having to expose it
public:
	/// <summary>
	/// Invalidates all weak references to this 
	/// </summary>
	virtual ~WeaklyReferable() {
		for (auto& ref : m_References) { ref.get().Invalidate(); }
	}
private:
	std::forward_list<std::reference_wrapper<WeakReference<T>>> m_References{};
};

