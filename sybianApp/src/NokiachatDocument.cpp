/*
========================================================================
 Name        : NokiachatDocument.cpp
 Author      : yaoyangyaha
 Copyright   : copyright yaoyangyaha 2026
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated User Includes]
#include "NokiachatDocument.h"
#include "NokiachatAppUi.h"
// ]]] end generated region [Generated User Includes]

/**
 * @brief Constructs the document class for the application.
 * @param anApplication the application instance
 */
CNokiachatDocument::CNokiachatDocument( CEikApplication& anApplication )
	: CAknDocument( anApplication )
	{
	}

/**
 * @brief Completes the second phase of Symbian object construction. 
 * Put initialization code that could leave here.  
 */ 
void CNokiachatDocument::ConstructL()
	{
	}
	
/**
 * Symbian OS two-phase constructor.
 *
 * Creates an instance of CNokiachatDocument, constructs it, and
 * returns it.
 *
 * @param aApp the application instance
 * @return the new CNokiachatDocument
 */
CNokiachatDocument* CNokiachatDocument::NewL( CEikApplication& aApp )
	{
	CNokiachatDocument* self = new ( ELeave ) CNokiachatDocument( aApp );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

/**
 * @brief Creates the application UI object for this document.
 * @return the new instance
 */	
CEikAppUi* CNokiachatDocument::CreateAppUiL()
	{
	return new ( ELeave ) CNokiachatAppUi;
	}
				
