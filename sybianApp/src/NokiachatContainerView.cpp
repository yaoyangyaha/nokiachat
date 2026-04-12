/*
========================================================================
 Name        : NokiachatContainerView.cpp
 Author      : yaoyangyaha
 Copyright   : copyright yaoyangyaha 2026
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated System Includes]
#include <aknviewappui.h>
#include <eikmenub.h>
#include <avkon.hrh>
#include <akncontext.h>
#include <akntitle.h>
#include <stringloader.h>
#include <barsread.h>
#include <eikbtgpc.h>
#include <Nokiachat.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]

#include "Nokiachat.hrh"
#include "NokiachatContainerView.h"
#include "NokiachatContainer.h"
// ]]] end generated region [Generated User Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

/**
 * First phase of Symbian two-phase construction. Should not contain any
 * code that could leave.
 */
CNokiachatContainerView::CNokiachatContainerView()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	iNokiachatContainer = NULL;
	// ]]] end generated region [Generated Contents]
	
	}

/** 
 * The view's destructor removes the container from the control
 * stack and destroys it.
 */
CNokiachatContainerView::~CNokiachatContainerView()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	delete iNokiachatContainer;
	iNokiachatContainer = NULL;
	// ]]] end generated region [Generated Contents]
	
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance then calls the second-phase constructor
 * without leaving the instance on the cleanup stack.
 * @return new instance of CNokiachatContainerView
 */
CNokiachatContainerView* CNokiachatContainerView::NewL()
	{
	CNokiachatContainerView* self = CNokiachatContainerView::NewLC();
	CleanupStack::Pop( self );
	return self;
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CNokiachatContainerView
 */
CNokiachatContainerView* CNokiachatContainerView::NewLC()
	{
	CNokiachatContainerView* self = new ( ELeave ) CNokiachatContainerView();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}


/**
 * Second-phase constructor for view.  
 * Initialize contents from resource.
 */ 
void CNokiachatContainerView::ConstructL()
	{
	// [[[ begin generated region: do not modify [Generated Code]
	BaseConstructL( R_NOKIACHAT_CONTAINER_NOKIACHAT_CONTAINER_VIEW );
				
	// ]]] end generated region [Generated Code]
	
	// add your own initialization code here
	
	}

/**
 * @return The UID for this view
 */
TUid CNokiachatContainerView::Id() const
	{
	return TUid::Uid( ENokiachatContainerViewId );
	}

/**
 * Handle a command for this view (override)
 * @param aCommand command id to be handled
 */
void CNokiachatContainerView::HandleCommandL( TInt aCommand )
	{
	// [[[ begin generated region: do not modify [Generated Code]
	TBool commandHandled = EFalse;
	switch ( aCommand )
		{	// code to dispatch to the AknView's menu and CBA commands is generated here
		default:
			break;
		}
	
		
	if ( !commandHandled ) 
		{
	
		if ( aCommand == EAknSoftkeyExit )
			{
			AppUi()->HandleCommandL( EEikCmdExit );
			}
	
		}
	// ]]] end generated region [Generated Code]
	
	}

/**
 *	Handles user actions during activation of the view, 
 *	such as initializing the content.
 */
void CNokiachatContainerView::DoActivateL( 
		const TVwsViewId& /*aPrevViewId*/,
		TUid /*aCustomMessageId*/,
		const TDesC8& /*aCustomMessage*/ )
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	SetupStatusPaneL();
	
				
	if ( iNokiachatContainer == NULL )
		{
		iNokiachatContainer = CreateContainerL();
		iNokiachatContainer->SetMopParent( this );
		AppUi()->AddToStackL( *this, iNokiachatContainer );
		} 
	// ]]] end generated region [Generated Contents]
	
	}

/**
 */
void CNokiachatContainerView::DoDeactivate()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	CleanupStatusPane();
	
	if ( iNokiachatContainer != NULL )
		{
		AppUi()->RemoveFromViewStack( *this, iNokiachatContainer );
		delete iNokiachatContainer;
		iNokiachatContainer = NULL;
		}
	// ]]] end generated region [Generated Contents]
	
	}

/** 
 * Handle status pane size change for this view (override)
 */
void CNokiachatContainerView::HandleStatusPaneSizeChange()
	{
	CAknView::HandleStatusPaneSizeChange();
	
	// this may fail, but we're not able to propagate exceptions here
	TVwsViewId view;
	AppUi()->GetActiveViewId( view );
	if ( view.iViewUid == Id() )
		{
		TInt result;
		TRAP( result, SetupStatusPaneL() );
		}
	
	// [[[ begin generated region: do not modify [Generated Code]
	// ]]] end generated region [Generated Code]
	
	}

// [[[ begin generated function: do not modify
void CNokiachatContainerView::SetupStatusPaneL()
	{
	// reset the context pane
	TUid contextPaneUid = TUid::Uid( EEikStatusPaneUidContext );
	CEikStatusPaneBase::TPaneCapabilities subPaneContext = 
		StatusPane()->PaneCapabilities( contextPaneUid );
	if ( subPaneContext.IsPresent() && subPaneContext.IsAppOwned() )
		{
		CAknContextPane* context = static_cast< CAknContextPane* > ( 
			StatusPane()->ControlL( contextPaneUid ) );
		context->SetPictureToDefaultL();
		}
	
	// setup the title pane
	TUid titlePaneUid = TUid::Uid( EEikStatusPaneUidTitle );
	CEikStatusPaneBase::TPaneCapabilities subPaneTitle = 
		StatusPane()->PaneCapabilities( titlePaneUid );
	if ( subPaneTitle.IsPresent() && subPaneTitle.IsAppOwned() )
		{
		CAknTitlePane* title = static_cast< CAknTitlePane* >( 
			StatusPane()->ControlL( titlePaneUid ) );
		TResourceReader reader;
		iEikonEnv->CreateResourceReaderLC( reader, R_NOKIACHAT_CONTAINER_TITLE_RESOURCE );
		title->SetFromResourceL( reader );
		CleanupStack::PopAndDestroy(); // reader internal state
		}
				
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
void CNokiachatContainerView::CleanupStatusPane()
	{
	}

// ]]] end generated function

/**
 *	Creates the top-level container for the view.  You may modify this method's
 *	contents and the CNokiachatContainer::NewL() signature as needed to initialize the
 *	container, but the signature for this method is fixed.
 *	@return new initialized instance of CNokiachatContainer
 */
CNokiachatContainer* CNokiachatContainerView::CreateContainerL()
	{
	return CNokiachatContainer::NewL( ClientRect(), NULL, this );
	}

