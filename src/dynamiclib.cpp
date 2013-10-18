#include <config.h>
#include <globals.h>

#include <dynamiclib.h>

/* System Headers */

/* Local Headers */

namespace MCServer {

/********************************************************************
 *          C O N S T R U C T I O N / D E S T R U C T I O N         *
 ********************************************************************/
DynamicLib::DynamicLib(void)
{
	mInst = NULL;
	mName = "";
	mError = "";

}

DynamicLib::~DynamicLib(void)
{
	if (mInst != NULL)
		UnLoad();
}

/********************************************************************
 *                        A T T R I B U T E S                       *
 ********************************************************************/

/********************************************************************
 *                        O P E R A T I O N S                       *
 ********************************************************************/

/*------------------------------------------------------------------*
 *								 Load()								*
 *------------------------------------------------------------------*/
/**
 *	@brief		Loads the library into memory.
 *	@author		Josh Williams
 *	@date		25-Sep-2003
 *
 *	@param		path	Path (relative or absolute) to the library.
 *	@param		name	Base name of the library to be loaded.
 *
 *	@returns	bool	Library load success/failure
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
bool DynamicLib::Load(const std::string& pPath, const std::string& pName)
{
	BuildLibName(pPath, pName);

	mInst = (DYNLIB_HANDLE)DYNLIB_LOAD( mName.c_str() );
	if (mInst == NULL)
	{
		mError = DYNLIB_ERROR();
		return false;
	}

	return true;
}

/*------------------------------------------------------------------*
 *								UnLoad()							*
 *------------------------------------------------------------------*/
/**
 *	@brief		Frees memory allocated for this library object.
 *	@author		Josh Williams
 *	@date		25-Sep-2003
 *
 *	@returns	void
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
void DynamicLib::UnLoad()
{
	if (mInst != NULL)
		DYNLIB_UNLOAD( mInst );

	mInst = NULL;
}

/*------------------------------------------------------------------*
 *							  GetSymbol()							*
 *------------------------------------------------------------------*/
/**
 *	@brief		Obtains a pointer to a symbol exported from the library.
 *	@author		Josh Williams
 *	@date		25-Sep-2003
 *
 *	@param		strName	Function to search for in the library.
 *
 *	@returns	What value this function returns, or void
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
void* DynamicLib::GetSymbol(const std::string& pName) const
{
	if (mInst == NULL)
	{
		return NULL;
	}

	return DYNLIB_GETSYM( mInst, pName.c_str());
}

/*------------------------------------------------------------------*
 *							  GetSymbol()							*
 *------------------------------------------------------------------*/
/**
 *	@brief		Retrieves the text for the last error encountered.
 *	@author		Josh Williams
 *	@date		25-Sep-2004
 *
 *	@returns	std::string containing the error text
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
const char* DynamicLib::Error(void) const
{
	return mError.c_str();
}

/********************************************************************
 *                         C A L L B A C K S                        *
 ********************************************************************/

/********************************************************************
 *                         I N T E R N A L S                        *
 ********************************************************************/

/*------------------------------------------------------------------*
 *							 BuildLibName()							*
 *------------------------------------------------------------------*/
/**
 *	@brief		Builds the platform specific library name
 *	@author		Josh Williams
 *	@date		02-Sep-2004
 *
 *	@param		path	Path to the location of the library
 *	@param		name	Basename of the library.
 *
 *	@returns	void
 */
/*------------------------------------------------------------------*
 * MODIFICATIONS													*
 *	Date		Description							Author			*
 * ===========	==================================	===============	*
 *																	*
 *------------------------------------------------------------------*/
void DynamicLib::BuildLibName(const std::string& pPath, const std::string& pName)
{
	mName = pPath;
#if MCSERVER_PLATFORM == PLATFORM_WINDOWS
	if (mName[mName.length()-1] != '\\')
		mName += "\\";
	mName += pName + ".dll";
#elif MCSERVER_PLATFORM == PLATFORM_LINUX
	if (mName[mName.length()-1] != '/')
		mName += "/";
	mName += "lib";
	mName += pName + ".so";
#endif
}

} // End Namespace
