#include "SystemWindows.hpp"

#include <Windows.h>
#include <chrono>

namespace OrbitEngine { namespace System { namespace priv {

	static BOOL CALLBACK ListMonitors_EnumFunc(HMONITOR monitor, HDC, LPRECT, LPARAM ptr)
	{
		MONITORINFO info = { 0 };
		info.cbSize = sizeof(MONITORINFO);
		if (!::GetMonitorInfo(monitor, &info))
			return TRUE;

		std::vector<MonitorDesc>* vec = reinterpret_cast<std::vector<MonitorDesc>*>(ptr);

		MonitorDesc desc;
		desc.mainArea = Math::Vec4i(
			info.rcMonitor.left,
			info.rcMonitor.top,
			info.rcMonitor.right - info.rcMonitor.left,
			info.rcMonitor.bottom - info.rcMonitor.top
		);
		desc.workArea = Math::Vec4i(
			info.rcWork.left,
			info.rcWork.top,
			info.rcWork.right - info.rcWork.left,
			info.rcWork.bottom - info.rcWork.top
		);
		desc.dpi = 1.0; // TODO: pf..

		if (info.dwFlags & MONITORINFOF_PRIMARY)
			vec->insert(vec->begin(), desc);
		else
			vec->push_back(desc);
		return TRUE;
	}

	std::vector<MonitorDesc> SystemWindows::getMonitors()
	{
		std::vector<MonitorDesc> vec;
		::EnumDisplayMonitors(NULL, NULL, ListMonitors_EnumFunc, reinterpret_cast<LPARAM>(&vec));
		return vec;
	}

	const char* SystemWindows::getName()
	{
		return "Windows";
	}

	long long SystemWindows::currentNano()
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
} } }
