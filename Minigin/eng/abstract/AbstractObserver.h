#pragma once

#include <any>

namespace eng {

class AbstractObserver;
class AbstractSubject;

struct Event {
	AbstractSubject& sender;
	unsigned int eventTypeHash;
	std::any context;
};

class AbstractSubject {
public:
	virtual ~AbstractSubject() = default;

	virtual void AddObserver(AbstractObserver& observer) = 0;
	virtual void RemoveObserver(AbstractObserver& observer) = 0;

protected:
	virtual void DispatchEvent(Event event) = 0;
};

class AbstractObserver {
public:
	virtual ~AbstractObserver() = default;
	virtual void OnEvent(Event event) = 0;
};

}