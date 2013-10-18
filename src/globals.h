#if !defined(__GLOBALS_H_INCLUDED__)
#define __GLOBALS_H_INCLUDED__

#define PLATFORM_WINDOWS 1
#define PLATFORM_APPLE   2
#define PLATFORM_LINUX   3

#if !defined(MCSERVER_PLATFORM)

#if defined(__WIN32__) || defined(_WIN32)
#include <windows.h>
#define MCSERVER_PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE_CC__)
#define MCSERVER_PLATFORM PLATFORM_APPLE
#else
#define MCSERVER_PLATFORM PLATFORM_LINUX
#endif

#endif

#endif // __GLOBALS_H_INCLUDED__
