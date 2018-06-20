#include "OE/Misc/FrameDebugger.hpp"

namespace OrbitEngine { namespace Misc {

	StrongPtr<FrameDebugger::Capture> FrameDebugger::s_CurrentCapture = StrongPtr<FrameDebugger::Capture>();
	StrongPtr<FrameDebugger::Capture> FrameDebugger::s_LastCapture = StrongPtr<FrameDebugger::Capture>();

	bool FrameDebugger::IsCapturing()
	{
		return s_CurrentCapture;
	}

	void FrameDebugger::StartCapture(const std::string& title)
	{
		FrameDebugger::Capture* cap = new FrameDebugger::Capture();
		cap->title = title;
		s_CurrentCapture = StrongPtr<FrameDebugger::Capture>(cap);
	}

	void FrameDebugger::StopCapture()
	{
		s_LastCapture = s_CurrentCapture;
		s_CurrentCapture.Clear();
	}

	StrongPtr<FrameDebugger::Capture> FrameDebugger::GetLastCapture()
	{
		return s_LastCapture;
	}

	void FrameDebugger::RegisterEvent(Event& event, bool push) {
		event.index = s_CurrentCapture->events.size();
		if (push) {
			event.parent_index = event.index - 1;
			s_CurrentCapture->next_parent = event.index;
		}
		else
			event.parent_index = s_CurrentCapture->next_parent;
		s_CurrentCapture->events.push_back(event);
	}

	void FrameDebugger::Pop() {
		if(IsCapturing())
			s_CurrentCapture->next_parent = s_CurrentCapture->events[s_CurrentCapture->events.size() - 1].parent_index;
	}
} }