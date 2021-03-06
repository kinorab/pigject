#include "eventQueue.h"
#include "../definition/utility.h"

namespace game {
	bool EventQueue::empty() {
		mutex.lock();
		bool empty = queue.empty();
		mutex.unlock();
		return empty;
	}

	void EventQueue::push(const sf::Event & event) {
		mutex.lock();
		queue.push(event);
		mutex.unlock();
	}

	sf::Event EventQueue::pop() {
		mutex.lock();
		sf::Event event = queue.front();
		queue.pop();
		mutex.unlock();
		return event;
	}
}