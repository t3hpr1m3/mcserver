#if !defined(__DYNAMICLIB_H_INCLUDED__)
#define __DYNAMICLIB_H_INCLUDED__

/* System Headers */
#include <string>

/* Local Headers */
#include "globals.h"

#if MCSERVER_PLATFORM == PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN

#define DYNLIB_HANDLE			HMODULE
#define DYNLIB_LOAD( a )		LoadLibrary( a )
#define DYNLIB_GETSYM( a, b )	GetProcAddress( a, b )
#define DYNLIB_UNLOAD( a )		FreeLibrary( a )
#define DYNLIB_ERROR( )			strerror(errno)
#elif MCSERVER_PLATFORM == PLATFORM_LINUX
#include <dlfcn.h>
#define DYNLIB_HANDLE			void*
#define DYNLIB_LOAD( a )		dlopen( a, RTLD_NOW)
#define DYNLIB_GETSYM( a, b )	dlsym( a, b )
#define DYNLIB_UNLOAD( a )		dlclose( a )
#define DYNLIB_ERROR( )			dlerror( )
#endif

namespace MCServer {

/**
 *	@class		DynamicLib
 *
 *	@brief		Represents a dynamically loaded library (.dll/.so)
 *	@author		Josh Williams
 *	@remarks	Allows a dynamic library to be loaded and function pointers
 *				obtained.  Handles cross-platform logic internally.
 */
class DynamicLib {
public:
	/*==================================*
	 *	   CONSTRUCTION/DESTRUCTION		*
	 *==================================*/
	DynamicLib(void);
	~DynamicLib(void);

	/*==================================*
	 *			  ATTRIBUTES			*
	 *==================================*/

	/*==================================*
	 *			  OPERATIONS			*
	 *==================================*/
	bool			Load(const std::string& pPath, const std::string& pName);
	void			UnLoad(void);
	void*			GetSymbol(const std::string& pName) const;
	const char*		Error(void) const;

protected:
	/*==================================*
	 *             CALLBACKS			*
	 *==================================*/

private:
	/*==================================*
	 *             INTERNALS            *
	 *==================================*/
	void			BuildLibName(const std::string &pPath, 
											const std::string &pName);

private:
	/*==================================*
	 *             VARIABLES            *
	 *==================================*/
	DYNLIB_HANDLE   mInst;      /**< OS specific library handle */
	std::string     mName;      /**< full name of the library to load */
	std::string     mError;     /**< Last error encountered */
};

} // End Namespace

#endif // __DYNAMICLIB_H_INCLUDED__
