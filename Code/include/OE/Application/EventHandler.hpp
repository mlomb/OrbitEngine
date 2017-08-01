#ifndef APPLICATION_EVENT_HANDLER_HPP
#define APPLICATION_EVENT_HANDLER_HPP

#include <vector>
#include <functional>

#include "OE/Application/Event.hpp"

namespace OrbitEngine { namespace Application {
	/* Should it be in the Misc namespace? */
	template<class TEvent = Event>
	class EventHandler {
		static_assert(std::is_base_of<Event, TEvent>::value, "TEvent must inherit from Event");

	public:
		typedef typename std::function<void(const TEvent&)> EventCallback;
		typedef typename std::vector<EventCallback> Callbacks;

		EventHandler() {};

		void AddListener(EventCallback callback) {
			p_Callbacks.push_back(callback);
		}

		template<class T>
		void AddListener(T* instance, void(T::*callback)(const TEvent&)) {
			AddListener(std::bind(callback, instance, std::placeholders::_1));
		}

		void RemoveListener(EventCallback callback) { // TODO Check if this works
			//p_Callbacks.erase(std::remove(p_Callbacks.begin(), p_Callbacks.end(), callback), p_Callbacks.end());
		}

		virtual void FireEvent(const TEvent& event) {
			for (typename Callbacks::reverse_iterator callback = p_Callbacks.rbegin(); callback != p_Callbacks.rend(); ++callback) {
				(*callback)(event);
			}
		};
	protected:
		Callbacks p_Callbacks;
	};
	
	template<class TPropagableEventEvent = PropagableEvent>
	class PropagableEventHandler : public EventHandler<TPropagableEventEvent> {
		static_assert(std::is_base_of<PropagableEvent, TPropagableEventEvent>::value, "TPropagableEventEvent must inherit from PropagableEvent");
	public:
		void FireEvent(TPropagableEventEvent& event) override {
			for (typename EventHandler<TPropagableEventEvent>::Callbacks::reverse_iterator callback = this->p_Callbacks.rbegin(); callback != this->p_Callbacks.rend(); ++callback) {
				(*callback)(event);
				if (!((PropagableEvent&)event).propagating) // Stop propagation
					break;
			}
		};
	};
} }

#endif