#ifndef ANDROID_HPP
#define ANDROID_HPP

#include "OE/Config.hpp"

#if OE_ANDROID
	#include <jni.h>
	#include <android/log.h>
	#include <android/configuration.h>
	#include <android/looper.h>
	#include <android/window.h>
	#include <android/input.h>
	#include <android/native_activity.h>
	#include <android_native_app_glue.h>
#endif

#endif
