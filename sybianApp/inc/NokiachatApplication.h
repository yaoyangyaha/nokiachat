/*
========================================================================
 Name        : NokiachatApplication.h
 Author      : yaoyangyaha
 Copyright   : copyright yaoyangyaha 2026
 Description : 
========================================================================
*/
#ifndef NOKIACHATAPPLICATION_H
#define NOKIACHATAPPLICATION_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknapp.h>
// ]]] end generated region [Generated Includes]

// [[[ begin generated region: do not modify [Generated Constants]
const TUid KUidNokiachatApplication = { 0xE50A2973 };
// ]]] end generated region [Generated Constants]

/**
 *
 * @class	CNokiachatApplication NokiachatApplication.h
 * @brief	A CAknApplication-derived class is required by the S60 application 
 *          framework. It is subclassed to create the application's document 
 *          object.
 */
class CNokiachatApplication : public CAknApplication
	{
private:
	TUid AppDllUid() const;
	CApaDocument* CreateDocumentL();
	
	};
			
#endif // NOKIACHATAPPLICATION_H		
