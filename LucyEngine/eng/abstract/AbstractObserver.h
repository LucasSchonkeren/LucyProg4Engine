#pragma once

#include <any>
#include <set>

namespace eng {

class IObserver;

struct Event {
	unsigned int eventTypeHash;
	std::any context;
};

class Subject final {
public:
	void AddObserver(IObserver& observer);
	void RemoveObserver(IObserver& observer);
	void DispatchEvent(Event event);

private:
	std::set<IObserver*> m_ObserverPtrs;
};

class IObserver {
public:
	virtual ~IObserver() = default;
	virtual void OnEvent(Event event) = 0;
};

}