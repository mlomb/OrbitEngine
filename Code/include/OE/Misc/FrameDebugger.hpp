#ifndef MISC_FRAME_DEBUGGER_HPP
#define MISC_FRAME_DEBUGGER_HPP

#include <string>
#include <vector>

#include "OE/Memory/Ptrs.hpp"

namespace OrbitEngine { namespace Misc {

	class FrameDebugger {
	public:
		struct Event {
			int index;
			int parent_index;
			std::string name;
			std::string description;
		};
		struct Capture {
			std::string title;
			std::vector<Event> events;
			int next_parent = -1;
		};

		static bool IsCapturing();
		static void StartCapture(const std::string& title);
		static void StopCapture();

		static void RegisterEvent(Event& event, bool push = false);
		static void Pop();

		static StrongPtr<Capture> GetLastCapture();

	private:
		static StrongPtr<Capture> s_CurrentCapture;
		static StrongPtr<Capture> s_LastCapture;
	};

} }

#endif