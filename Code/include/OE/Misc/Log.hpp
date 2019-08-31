#ifndef LOG_HPP
#define LOG_HPP

#include "OE/Config.hpp"

#if OE_ANDROID
	#include "OE/Platform/Android/Android.hpp"
#endif
#if OE_WINDOWS
	#include "OE/Platform/Windows/Windows.hpp"
#endif

#include <sstream>
#include <cstring>
#include <string>

namespace OrbitEngine {
	enum LogLevel {
		LOG_DEBUG = 0,
		LOG_INFO,
		LOG_WARNING,
		LOG_ERROR,
		LOG_FATAL
	};

	class Log {
	public:
		static void log(const char* file, int line, const char* message, LogLevel level);
	};
}

#define OE_LOG(message, level) \
{ \
	std::ostringstream _os_; \
	_os_ << message; \
	OrbitEngine::Log::log(__FILE__, __LINE__, _os_.str().c_str(), level); \
}

#if OE_DEBUG && OE_WINDOWS
#define OE_BREAK __debugbreak();
#else
#define OE_BREAK {};
#endif

#define OE_LOG_DEBUG(message) OE_LOG(message, OrbitEngine::LOG_DEBUG)
#define OE_LOG_INFO(message) OE_LOG(message, OrbitEngine::LOG_INFO)
#define OE_LOG_WARNING(message) OE_LOG(message, OrbitEngine::LOG_WARNING)
#define OE_LOG_ERROR(message) OE_LOG(message, OrbitEngine::LOG_ERROR)
#define OE_LOG_FATAL(message) OE_LOG(message, OrbitEngine::LOG_FATAL)

// disable debug messages
#if !OE_DEBUG
	#define OE_LOG_DEBUG(message) {};
#endif

#if OE_WINDOWS
	#define OE_LOG_FATAL_SHOWBOX_AND_EXIT(message) \
	{ \
		OE_LOG_FATAL(message) \
		std::ostringstream os_; \
		os_ << message << "\n\nThe application may now exit."; \
		MessageBox(NULL, \
				   os_.str().c_str(), \
				   "OrbitEngine - FATAL", \
				   MB_ICONERROR | MB_OK); \
		OE_BREAK \
	}
#else
	#define OE_LOG_FATAL_SHOWBOX_AND_EXIT(message) OE_LOG_FATAL(message)
#endif

#if OE_DEBUG
#define OE_ASSERT_MSG(condition, msg) \
{ \
	if(!(condition)){ \
		OE_LOG_FATAL("---------------------") \
		OE_LOG_FATAL("ASSERTION FAILED!") \
		OE_LOG_FATAL("File: " << __FILE__) \
		OE_LOG_FATAL("Line: " << __LINE__) \
		OE_LOG_FATAL("Function: " << __FUNCTION__) \
		OE_LOG_FATAL("Condition: (" << #condition << ")") \
		OE_LOG_FATAL("Details: " << #msg) \
		OE_LOG_FATAL("---------------------") \
		OE_BREAK \
	} \
};
#else
	#define OE_ASSERT_MSG(condition, msg) {};
#endif

#define NOARG
#define OE_ASSERT(condition) OE_ASSERT_MSG(condition, NOARG)

#endif