#if !defined(__GLOBALS_H_INCLUDED__)
#define __GLOBALS_H_INCLUDED__

#define PLATFORM_WINDOWS 1
#define PLATFORM_APPLE   2
#define PLATFORM_LINUX   3

#define ARCH_32 1
#define ARCH_64 2

/*
 * Determine the platform
 */
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

/*
 * Determine the architecture
 */
#if !defined(MCSERVER_ARCH)

#if defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) || defined(__M_IX86)
#define MCSERVER_ARCH ARCH_32
#elif defined(__amd64__) || defined(__x86_64__)
#define MCSERVER_ARCH ARCH_64
#endif

#endif

#endif // __GLOBALS_H_INCLUDED__
