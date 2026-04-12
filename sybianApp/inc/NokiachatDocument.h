/*
========================================================================
 Name        : NokiachatDocument.h
 Author      : yaoyangyaha
 Copyright   : copyright yaoyangyaha 2026
 Description : 
========================================================================
*/
#ifndef NOKIACHATDOCUMENT_H
#define NOKIACHATDOCUMENT_H

#include <akndoc.h>
		
class CEikAppUi;

/**
* @class	CNokiachatDocument NokiachatDocument.h
* @brief	A CAknDocument-derived class is required by the S60 application 
*           framework. It is responsible for creating the AppUi object. 
*/
class CNokiachatDocument : public CAknDocument
	{
public: 
	// constructor
	static CNokiachatDocument* NewL( CEikApplication& aApp );

private: 
	// constructors
	CNokiachatDocument( CEikApplication& aApp );
	void ConstructL();
	
public: 
	// from base class CEikDocument
	CEikAppUi* CreateAppUiL();
	};
#endif // NOKIACHATDOCUMENT_H
