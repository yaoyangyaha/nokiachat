/*
========================================================================
 Name        : NokiachatAppUi.h
 Author      : yaoyangyaha
 Copyright   : copyright yaoyangyaha 2026
 Description : 
========================================================================
*/
#ifndef NOKIACHATAPPUI_H
#define NOKIACHATAPPUI_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknviewappui.h>
// ]]] end generated region [Generated Includes]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class CNokiachatContainerView;
// ]]] end generated region [Generated Forward Declarations]

/**
 * @class	CNokiachatAppUi NokiachatAppUi.h
 * @brief The AppUi class handles application-wide aspects of the user interface, including
 *        view management and the default menu, control pane, and status pane.
 */
class CNokiachatAppUi : public CAknViewAppUi
	{
public: 
	// constructor and destructor
	CNokiachatAppUi();
	virtual ~CNokiachatAppUi();
	void ConstructL();

public:
	// from CCoeAppUi
	TKeyResponse HandleKeyEventL(
				const TKeyEvent& aKeyEvent,
				TEventCode aType );

	// from CEikAppUi
	void HandleCommandL( TInt aCommand );
	void HandleResourceChangeL( TInt aType );

	// from CAknAppUi
	void HandleViewDeactivation( 
			const TVwsViewId& aViewIdToBeDeactivated, 
			const TVwsViewId& aNewlyActivatedViewId );

private:
	void InitializeContainersL();
	// [[[ begin generated region: do not modify [Generated Methods]
public: 
	// ]]] end generated region [Generated Methods]
	
	// [[[ begin generated region: do not modify [Generated Instance Variables]
private: 
	CNokiachatContainerView* iNokiachatContainerView;
	// ]]] end generated region [Generated Instance Variables]
	
	
	// [[[ begin [User Handlers]
protected: 
	// ]]] end [User Handlers]
	
	};

#endif // NOKIACHATAPPUI_H			
