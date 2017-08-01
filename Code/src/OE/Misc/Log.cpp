#include "OE/Misc/Log.hpp"

#include <iostream>

#if OE_WINDOWS
	#include "OE/Platform/Windows/Windows.hpp"
#endif

namespace OrbitEngine {

#if OE_WINDOWS
	HANDLE s_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#elif OE_ANDROID
	android_LogPriority LogLevelToAndroid(LogLevel level) {
		switch (level) {
		case LOG_DEBUG:
			return ANDROID_LOG_DEBUG;
		case LOG_INFO:
			return ANDROID_LOG_INFO;
		case LOG_WARNING:
			return ANDROID_LOG_WARN;
		case LOG_ERROR:
			return ANDROID_LOG_ERROR;
		case LOG_FATAL:
			return ANDROID_LOG_FATAL;
		default:
			return ANDROID_LOG_DEFAULT;
		}
	}
#endif

	void Log::log(const char* file, int line, const char* message, LogLevel level)
	{
		std::ostringstream oss;

		if (level >= LogLevel::LOG_WARNING)
			oss << "(..." << &file[strlen(file) - 15] << ":" << line << ")";
		oss << ": " << message;

#if OE_ANDROID
		__android_log_print(LogLevelToAndroid(level), OE_APPNAME, "%s", oss.str().c_str());
#else

		char win_color;
		std::string type;

		switch (level) {
		case LOG_INFO:
			win_color = 11;
			type = "INFO";
			break;
		case LOG_WARNING:
			win_color = 14;
			type = "WARN";
			break;
		case LOG_ERROR:
			win_color = 12;
			type = "ERROR";
			break;
		case LOG_FATAL:
			win_color = 76;
			type = "FATAL";
			break;
		case LOG_DEBUG:
		default:
			win_color = 15;
			type = "DEBUG";
		}

	#if OE_WINDOWS
		SetConsoleTextAttribute(s_hConsole, win_color);
	#endif

		std::cout << type << oss.str() << std::endl;

	#if OE_WINDOWS
		SetConsoleTextAttribute(s_hConsole, 15);
	#endif
#endif
	}
}