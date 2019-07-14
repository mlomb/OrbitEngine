#ifndef APPLICATION_EVENT_HANDLER_HPP
#define APPLICATION_EVENT_HANDLER_HPP

#include <vector>
#include <functional>
#include <memory>
#include <algorithm>

#include "OE/Application/Event.hpp"

namespace OrbitEngine { namespace Application {
	/* Should it be in the Misc namespace? */
	template<class TEvent = Event>
	class EventHandler {
		//static_assert(std::is_base_of<Event, TEvent>::value, "TEvent must inherit from Event");

	public:
		typedef typename std::function<void(TEvent&)> EventCallback;
		typedef typename std::shared_ptr<EventCallback> EventCallbackPtr;
		typedef typename std::vector<EventCallbackPtr> Callbacks;

		EventHandler() {};

		EventCallbackPtr AddListener(EventCallback callback) {
			EventCallbackPtr ptr(new EventCallback(callback));
			p_Callbacks.push_back(ptr);
			return ptr;
		}

		template<class T>
		EventCallbackPtr AddListener(T* instance, void(T::*callback)(TEvent&)) {
			return AddListener(std::bind(callback, instance, std::placeholders::_1));
		}

		void RemoveListener(EventCallbackPtr callbackPtr) {
			auto it = std::find(p_Callbacks.begin(), p_Callbacks.end(), callbackPtr);
			if (it != p_Callbacks.end())
				p_Callbacks.erase(it);
		}

		virtual void FireEvent(TEvent& event) {
			for (typename Callbacks::reverse_iterator callback = p_Callbacks.rbegin(); callback != p_Callbacks.rend(); ++callback) {
				(*(*callback))(event);
			}
		}
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