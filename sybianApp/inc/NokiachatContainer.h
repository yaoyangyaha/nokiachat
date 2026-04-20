/*
========================================================================
 Name        : NokiachatContainer.h
 Author      : yaoyangyaha
 Copyright   : copyright yaoyangyaha 2026
 Description : 
========================================================================
*/
#ifndef NOKIACHATCONTAINER_H
#define NOKIACHATCONTAINER_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <coecntrl.h>		
// ]]] end generated region [Generated Includes]

#include <e32base.h>
#include <eiklbo.h>
#include <avkon.rsg>

#include "NokiachatHttpClient.h"

#ifndef R_AVKON_DIALOG_QUERY_VALUE
#define R_AVKON_DIALOG_QUERY_VALUE R_AVKON_DIALOG_QUERY_VALUE_TEXT
#endif

// [[[ begin [Event Handler Includes]
// ]]] end [Event Handler Includes]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class MEikCommandObserver;		
// ]]] end generated region [Generated Forward Declarations]

class CAknSingleStyleListBox;

/**
 * Container class for NokiachatContainer
 * 
 * @class	CNokiachatContainer NokiachatContainer.h
 */
class CNokiachatContainer : public CCoeControl, public MEikListBoxObserver, public MNokiachatHttpObserver
	{
public:
	// constructors and destructor
	CNokiachatContainer();
	static CNokiachatContainer* NewL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver );
	static CNokiachatContainer* NewLC( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver );
	void ConstructL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver );
	virtual ~CNokiachatContainer();

public:
	// from base class CCoeControl
	TInt CountComponentControls() const;
	CCoeControl* ComponentControl( TInt aIndex ) const;
	TKeyResponse OfferKeyEventL( 
			const TKeyEvent& aKeyEvent, 
			TEventCode aType );
	void HandleResourceChange( TInt aType );
	void HandleListBoxEventL( CEikListBox* aListBox, TListBoxEvent aEvent );

public: // MNokiachatHttpObserver
	void HttpCompletedL( TInt aErr, const TDesC8& aBody );
	
protected:
	// from base class CCoeControl
	void SizeChanged();

private:
	// from base class CCoeControl
	void Draw( const TRect& aRect ) const;

private:
	void InitializeControlsL();
	void LayoutControls();
	void SetMainMenuL();
	void FetchConversationsL();
	void FetchMessagesL( const TDesC& aConvId );
	void ShowErrorNoteL( TInt aErr );
	void SwitchToConversationListL();
	void SwitchToMessageListL( const TDesC& aConvId );
	void ParseConversationsL( const TDesC8& aBody );
	void ParseMessagesL( const TDesC8& aBody );
	void PromptSendTextL();
	void SendTextL( const TDesC& aText );
	HBufC* Utf8ToUnicodeL( const TDesC8& aUtf8 );
	HBufC* UrlEncodeConvIdL( const TDesC& aConvId );
	HBufC8* FormUrlEncodeUtf8L( const TDesC& aValue );

	CCoeControl* iFocusControl;
	MEikCommandObserver* iCommandObserver;
	// [[[ begin generated region: do not modify [Generated Methods]
public: 
	// ]]] end generated region [Generated Methods]
	
	// [[[ begin generated region: do not modify [Generated Type Declarations]
public: 
	// ]]] end generated region [Generated Type Declarations]
	
	// [[[ begin generated region: do not modify [Generated Instance Variables]
private: 
	// ]]] end generated region [Generated Instance Variables]

private:
	enum TUiMode
		{
		EModeMain = 0,
		EModeQqConversations,
		EModeQqMessages
		};
	enum TRequestKind
		{
		EReqNone = 0,
		EReqConversations,
		EReqMessages,
		EReqSendText
		};

private:
	CAknSingleStyleListBox* iListBox;
	CDesCArrayFlat* iListItems;
	CDesCArrayFlat* iConvIds;

	CNokiachatHttpClient* iHttp;
	HBufC* iServerBase;
	HBufC* iApiKey;

	TUiMode iMode;
	TRequestKind iReq;
	HBufC* iActiveConvId;
	
	
	// [[[ begin [Overridden Methods]
protected: 
	// ]]] end [Overridden Methods]
	
	
	// [[[ begin [User Handlers]
protected: 
	// ]]] end [User Handlers]
	
public: 
	enum TControls
		{
		// [[[ begin generated region: do not modify [Generated Contents]
		
		// ]]] end generated region [Generated Contents]
		
		// add any user-defined entries here...
		
		ELastControl
		};
	};
				
#endif // NOKIACHATCONTAINER_H
