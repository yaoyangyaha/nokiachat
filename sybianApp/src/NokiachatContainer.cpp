/*
========================================================================
 Name        : NokiachatContainer.cpp
 Author      : yaoyangyaha
 Copyright   : copyright yaoyangyaha 2026
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated System Includes]
#include <aknviewappui.h>
#include <eikappui.h>
#include <Nokiachat.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]
#include "NokiachatContainer.h"
#include "NokiachatContainerView.h"
#include "Nokiachat.hrh"
// ]]] end generated region [Generated User Includes]

#include <aknlists.h>
#include <eikclbd.h>
#include <eiktxlbm.h>
#include <utf.h>
#include <aknnotewrappers.h>
#include <aknquerydialog.h>
#include <avkon.rsg>

#include "NokiachatProto.h"

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

static TInt FindChar8( const TDesC8& aDes, const TChar aCh, TInt aFrom )
	{
	const TInt len = aDes.Length();
	for ( TInt i = aFrom; i < len; ++i )
		{
		if ( aDes[i] == aCh )
			{
			return i;
			}
		}
	return KErrNotFound;
	}

static TPtrC8 Field8( const TDesC8& aLine, TInt aFieldIndex )
	{
	TInt start = 0;
	TInt end = aLine.Length();
	TInt curField = 0;
	for ( TInt i = 0; i <= aLine.Length(); ++i )
		{
		const TBool atEnd = ( i == aLine.Length() );
		const TBool isSep = ( !atEnd && aLine[i] == '\t' );
		if ( atEnd || isSep )
			{
			if ( curField == aFieldIndex )
				{
				end = i;
				break;
				}
			++curField;
			start = i + 1;
			}
		}
	if ( start > end )
		{
		return TPtrC8( KNullDesC8 );
		}
	return aLine.Mid( start, end - start );
	}
/**
 * First phase of Symbian two-phase construction. Should not 
 * contain any code that could leave.
 */
CNokiachatContainer::CNokiachatContainer()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	iListBox = NULL;
	iListItems = NULL;
	iConvIds = NULL;
	iHttp = NULL;
	iServerBase = NULL;
	iApiKey = NULL;
	iMode = EModeMain;
	iReq = EReqNone;
	iActiveConvId = NULL;
	
	}
/** 
 * Destroy child controls.
 */
CNokiachatContainer::~CNokiachatContainer()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	delete iHttp;
	delete iListBox;
	delete iListItems;
	delete iConvIds;
	delete iServerBase;
	delete iApiKey;
	delete iActiveConvId;
	
	}
				
/**
 * Construct the control (first phase).
 *  Creates an instance and initializes it.
 *  Instance is not left on cleanup stack.
 * @param aRect bounding rectangle
 * @param aParent owning parent, or NULL
 * @param aCommandObserver command observer
 * @return initialized instance of CNokiachatContainer
 */
CNokiachatContainer* CNokiachatContainer::NewL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver )
	{
	CNokiachatContainer* self = CNokiachatContainer::NewLC( 
			aRect, 
			aParent, 
			aCommandObserver );
	CleanupStack::Pop( self );
	return self;
	}

/**
 * Construct the control (first phase).
 *  Creates an instance and initializes it.
 *  Instance is left on cleanup stack.
 * @param aRect The rectangle for this window
 * @param aParent owning parent, or NULL
 * @param aCommandObserver command observer
 * @return new instance of CNokiachatContainer
 */
CNokiachatContainer* CNokiachatContainer::NewLC( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver )
	{
	CNokiachatContainer* self = new ( ELeave ) CNokiachatContainer();
	CleanupStack::PushL( self );
	self->ConstructL( aRect, aParent, aCommandObserver );
	return self;
	}
			
/**
 * Construct the control (second phase).
 *  Creates a window to contain the controls and activates it.
 * @param aRect bounding rectangle
 * @param aCommandObserver command observer
 * @param aParent owning parent, or NULL
 */ 
void CNokiachatContainer::ConstructL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver )
	{
	if ( aParent == NULL )
	    {
		CreateWindowL();
	    }
	else
	    {
	    SetContainerWindowL( *aParent );
	    }
	iFocusControl = NULL;
	iCommandObserver = aCommandObserver;
	InitializeControlsL();
	SetRect( aRect );
	ActivateL();
	// [[[ begin generated region: do not modify [Post-ActivateL initializations]
	// ]]] end generated region [Post-ActivateL initializations]
	
	}
			
/**
* Return the number of controls in the container (override)
* @return count
*/
TInt CNokiachatContainer::CountComponentControls() const
	{
	return 1;
	}
				
/**
* Get the control with the given index (override)
* @param aIndex Control index [0...n) (limited by #CountComponentControls)
* @return Pointer to control
*/
CCoeControl* CNokiachatContainer::ComponentControl( TInt aIndex ) const
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	switch ( aIndex )
		{
		case 0:
			return iListBox;
		}
	// ]]] end generated region [Generated Contents]
	
	// handle any user controls here...
	
	return NULL;
	}
				
/**
 *	Handle resizing of the container. This implementation will lay out
 *  full-sized controls like list boxes for any screen size, and will layout
 *  labels, editors, etc. to the size they were given in the UI designer.
 *  This code will need to be modified to adjust arbitrary controls to
 *  any screen size.
 */				
void CNokiachatContainer::SizeChanged()
	{
	CCoeControl::SizeChanged();
	LayoutControls();
	// [[[ begin generated region: do not modify [Generated Contents]
			
	// ]]] end generated region [Generated Contents]
	
	}
				
// [[[ begin generated function: do not modify
/**
 * Layout components as specified in the UI Designer
 */
void CNokiachatContainer::LayoutControls()
	{
	if ( iListBox )
		{
		iListBox->SetRect( Rect() );
		}
	}
// ]]] end generated function

/**
 *	Handle key events.
 */				
TKeyResponse CNokiachatContainer::OfferKeyEventL( 
		const TKeyEvent& aKeyEvent, 
		TEventCode aType )
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	
	// ]]] end generated region [Generated Contents]
	
	if ( aType == EEventKey && aKeyEvent.iCode == EKeyBackspace )
		{
		if ( iMode == EModeQqMessages )
			{
			SwitchToConversationListL();
			iListBox->DrawNow();
			return EKeyWasConsumed;
			}
		if ( iMode == EModeQqConversations )
			{
			SetMainMenuL();
			return EKeyWasConsumed;
			}
		}

	if ( aType == EEventKey && iMode == EModeQqMessages && aKeyEvent.iCode == '5' )
		{
		PromptSendTextL();
		return EKeyWasConsumed;
		}

	if ( iListBox != NULL
		&& iListBox->OfferKeyEventL( aKeyEvent, aType ) == EKeyWasConsumed )
		{
		return EKeyWasConsumed;
		}
	return CCoeControl::OfferKeyEventL( aKeyEvent, aType );
	}
				
// [[[ begin generated function: do not modify
/**
 *	Initialize each control upon creation.
 */				
void CNokiachatContainer::InitializeControlsL()
	{
	iListItems = new ( ELeave ) CDesCArrayFlat( 8 );
	iConvIds = new ( ELeave ) CDesCArrayFlat( 8 );

	iListBox = new ( ELeave ) CAknSingleStyleListBox();
	iListBox->SetContainerWindowL( *this );
	iListBox->ConstructL( this, EAknListBoxSelectionList );
	iListBox->SetListBoxObserver( this );

	CTextListBoxModel* model = iListBox->Model();
	model->SetItemTextArray( iListItems );
	model->SetOwnershipType( ELbmDoesNotOwnItemArray );

	iHttp = CNokiachatHttpClient::NewL( *this );
	iServerBase = KDefaultServerBase().AllocL();
	iApiKey = KDefaultApiKey().AllocL();

	SetMainMenuL();
	}
// ]]] end generated function

/** 
 * Handle global resource changes, such as scalable UI or skin events (override)
 */
void CNokiachatContainer::HandleResourceChange( TInt aType )
	{
	CCoeControl::HandleResourceChange( aType );
	SetRect( iAvkonViewAppUi->View( TUid::Uid( ENokiachatContainerViewId ) )->ClientRect() );
	// [[[ begin generated region: do not modify [Generated Contents]
	// ]]] end generated region [Generated Contents]
	
	}
				
/**
 *	Draw container contents.
 */				
void CNokiachatContainer::Draw( const TRect& aRect ) const
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	CWindowGc& gc = SystemGc();
	gc.Clear( aRect );
	
	// ]]] end generated region [Generated Contents]
	
	}
				
void CNokiachatContainer::HandleListBoxEventL( CEikListBox* /*aListBox*/, TListBoxEvent aEvent )
	{
	if ( aEvent != EEventEnterKeyPressed && aEvent != EEventItemClicked )
		{
		return;
		}

	const TInt idx = iListBox ? iListBox->CurrentItemIndex() : -1;
	if ( idx < 0 )
		{
		return;
		}

	if ( iMode == EModeMain )
		{
		if ( idx == 1 ) // QQ
			{
			FetchConversationsL();
			}
		else
			{
			CAknInformationNote* note = new ( ELeave ) CAknInformationNote();
			note->ExecuteLD( _L( "微信暂未实现" ) );
			}
		}
	else if ( iMode == EModeQqConversations )
		{
		if ( idx < iConvIds->MdcaCount() )
			{
			const TPtrC convId = iConvIds->MdcaPoint( idx );
			SwitchToMessageListL( convId );
			FetchMessagesL( convId );
			}
		}
	}

void CNokiachatContainer::SetMainMenuL()
	{
	iMode = EModeMain;
	iReq = EReqNone;
	iConvIds->Reset();
	iListItems->Reset();
	iListItems->AppendL( _L( "微信消息" ) );
	iListItems->AppendL( _L( "QQ消息" ) );
	iListBox->HandleItemAdditionL();
	iListBox->SetCurrentItemIndex( 0 );
	iListBox->DrawNow();
	}

void CNokiachatContainer::FetchConversationsL()
	{
	iReq = EReqConversations;
	TBuf<256> url;
	url.Append( *iServerBase );
	url.Append( _L( "/v1/qq/conversations" ) );
	iHttp->GetL( url, *iApiKey );
	}

HBufC* CNokiachatContainer::UrlEncodeConvIdL( const TDesC& aConvId )
	{
	HBufC* out = HBufC::NewL( aConvId.Length() + 16 );
	TPtr p = out->Des();
	for ( TInt i = 0; i < aConvId.Length(); ++i )
		{
		const TChar ch = aConvId[i];
		if ( ch == ':' )
			{
			p.Append( _L( "%3A" ) );
			}
		else if ( ch == ' ' )
			{
			p.Append( _L( "%20" ) );
			}
		else
			{
			p.Append( ch );
			}
		}
	return out;
	}

void CNokiachatContainer::FetchMessagesL( const TDesC& aConvId )
	{
	iReq = EReqMessages;
	HBufC* enc = UrlEncodeConvIdL( aConvId );
	CleanupStack::PushL( enc );

	HBufC* url = HBufC::NewLC( iServerBase->Length() + enc->Length() + 64 );
	TPtr p = url->Des();
	p.Append( *iServerBase );
	p.Append( _L( "/v1/qq/messages?conv_id=" ) );
	p.Append( *enc );
	p.Append( _L( "&after_id=0&limit=50" ) );

	iHttp->GetL( p, *iApiKey );

	CleanupStack::PopAndDestroy( url );
	CleanupStack::PopAndDestroy( enc );
	}

HBufC8* CNokiachatContainer::FormUrlEncodeUtf8L( const TDesC& aValue )
	{
	HBufC8* utf8 = HBufC8::NewLC( aValue.Length() * 3 + 8 );
	TPtr8 utf8Ptr( utf8->Des() );
	utf8Ptr.Zero();
	CnvUtfConverter::ConvertFromUnicodeToUtf8( utf8Ptr, aValue );

	// Percent-encode bytes; space -> '+'.
	HBufC8* out = HBufC8::NewL( utf8Ptr.Length() * 3 + 8 );
	TPtr8 p = out->Des();
	p.Zero();
	for ( TInt i = 0; i < utf8Ptr.Length(); ++i )
		{
		const TUint8 b = utf8Ptr[i];
		const TBool safe =
			( ( b >= 'a' && b <= 'z' ) ||
			  ( b >= 'A' && b <= 'Z' ) ||
			  ( b >= '0' && b <= '9' ) ||
			  b == '-' || b == '_' || b == '.' || b == '~' );
		if ( b == ' ' )
			{
			p.Append( (TChar)'+' );
			}
		else if ( safe )
			{
			p.Append( (TChar)b );
			}
		else
			{
			const TUint8 hi = (TUint8)( ( b >> 4 ) & 0x0F );
			const TUint8 lo = (TUint8)( b & 0x0F );
			p.Append( (TChar)'%' );
			p.Append( (TChar)( hi < 10 ? ( '0' + hi ) : ( 'A' + ( hi - 10 ) ) ) );
			p.Append( (TChar)( lo < 10 ? ( '0' + lo ) : ( 'A' + ( lo - 10 ) ) ) );
			}
		}

	CleanupStack::PopAndDestroy( utf8 );
	return out;
	}

void CNokiachatContainer::PromptSendTextL()
	{
	if ( !iActiveConvId )
		{
		return;
		}

	TBuf<256> text;
	CAknTextQueryDialog* dlg = new ( ELeave ) CAknTextQueryDialog( text );
	dlg->SetPromptL( _L( "发送到QQ(按5):" ) );
	if ( dlg->ExecuteLD( R_AVKON_DIALOG_QUERY_VALUE ) )
		{
		if ( text.Length() > 0 )
			{
			SendTextL( text );
			}
		}
	}

void CNokiachatContainer::SendTextL( const TDesC& aText )
	{
	iReq = EReqSendText;

	HBufC8* convEnc = FormUrlEncodeUtf8L( *iActiveConvId );
	CleanupStack::PushL( convEnc );
	HBufC8* textEnc = FormUrlEncodeUtf8L( aText );
	CleanupStack::PushL( textEnc );

	HBufC8* body = HBufC8::NewLC( convEnc->Length() + textEnc->Length() + 32 );
	TPtr8 bp = body->Des();
	bp.Append( _L8( "conv_id=" ) );
	bp.Append( *convEnc );
	bp.Append( _L8( "&text=" ) );
	bp.Append( *textEnc );

	TBuf<256> url;
	url.Append( *iServerBase );
	url.Append( _L( "/v1/qq/send_text" ) );
	iHttp->PostFormL( url, *iApiKey, bp );

	CleanupStack::PopAndDestroy( body );
	CleanupStack::PopAndDestroy( textEnc );
	CleanupStack::PopAndDestroy( convEnc );
	}

void CNokiachatContainer::SwitchToConversationListL()
	{
	iMode = EModeQqConversations;
	delete iActiveConvId;
	iActiveConvId = NULL;
	}

void CNokiachatContainer::SwitchToMessageListL( const TDesC& aConvId )
	{
	iMode = EModeQqMessages;
	delete iActiveConvId;
	iActiveConvId = aConvId.AllocL();
	iListItems->Reset();
	iListBox->HandleItemAdditionL();
	iListBox->DrawNow();
	}

HBufC* CNokiachatContainer::Utf8ToUnicodeL( const TDesC8& aUtf8 )
	{
	HBufC* out = HBufC::NewL( aUtf8.Length() + 8 );
	TPtr p = out->Des();
	p.Zero();
	CnvUtfConverter::ConvertToUnicodeFromUtf8( p, aUtf8 );
	return out;
	}

void CNokiachatContainer::ParseConversationsL( const TDesC8& aBody )
	{
	iConvIds->Reset();
	iListItems->Reset();

	TInt pos = 0;
	while ( pos < aBody.Length() )
		{
		TInt nl = FindChar8( aBody, '\n', pos );
		if ( nl == KErrNotFound )
			{
			nl = aBody.Length();
			}
		if ( nl > pos )
			{
			const TPtrC8 line = aBody.Mid( pos, nl - pos );
			const TPtrC8 convId8 = Field8( line, 0 );
			const TPtrC8 name8 = Field8( line, 1 );
			const TPtrC8 preview8 = Field8( line, 3 );
			const TPtrC8 unread8 = Field8( line, 4 );

			HBufC* convId = Utf8ToUnicodeL( convId8 );
			CleanupStack::PushL( convId );
			HBufC* name = Utf8ToUnicodeL( name8 );
			CleanupStack::PushL( name );
			HBufC* preview = Utf8ToUnicodeL( preview8 );
			CleanupStack::PushL( preview );
			HBufC* unread = Utf8ToUnicodeL( unread8 );
			CleanupStack::PushL( unread );

			iConvIds->AppendL( *convId );

			HBufC* item = HBufC::NewLC( name->Length() + preview->Length() + unread->Length() + 16 );
			TPtr ip = item->Des();
			ip.Append( *name );
			if ( unread->Length() > 0 && unread->Des() != _L( "0" ) )
				{
				ip.Append( _L( " (" ) );
				ip.Append( *unread );
				ip.Append( _L( ")" ) );
				}
			if ( preview->Length() > 0 )
				{
				ip.Append( _L( " - " ) );
				ip.Append( *preview );
				}
			iListItems->AppendL( ip );

			CleanupStack::PopAndDestroy( item );
			CleanupStack::PopAndDestroy( unread );
			CleanupStack::PopAndDestroy( preview );
			CleanupStack::PopAndDestroy( name );
			CleanupStack::PopAndDestroy( convId );
			}
		pos = nl + 1;
		}

	iMode = EModeQqConversations;
	if ( iListItems->MdcaCount() == 0 )
		{
		iListItems->AppendL( _L( "(无会话)" ) );
		}
	iListBox->HandleItemAdditionL();
	iListBox->SetCurrentItemIndex( 0 );
	iListBox->DrawNow();
	}

void CNokiachatContainer::ParseMessagesL( const TDesC8& aBody )
	{
	iListItems->Reset();

	TInt pos = 0;
	while ( pos < aBody.Length() )
		{
		TInt nl = FindChar8( aBody, '\n', pos );
		if ( nl == KErrNotFound )
			{
			nl = aBody.Length();
			}
		if ( nl > pos )
			{
			const TPtrC8 line = aBody.Mid( pos, nl - pos );
			const TPtrC8 sender8 = Field8( line, 2 );
			const TPtrC8 type8 = Field8( line, 3 );
			const TPtrC8 content8 = Field8( line, 4 );

			HBufC* sender = Utf8ToUnicodeL( sender8 );
			CleanupStack::PushL( sender );
			HBufC* type = Utf8ToUnicodeL( type8 );
			CleanupStack::PushL( type );
			HBufC* content = Utf8ToUnicodeL( content8 );
			CleanupStack::PushL( content );

			HBufC* item = HBufC::NewLC( sender->Length() + type->Length() + content->Length() + 8 );
			TPtr ip = item->Des();
			ip.Append( *sender );
			ip.Append( _L( ": " ) );
			if ( type->Des() != _L( "text" ) )
				{
				ip.Append( _L( "[" ) );
				ip.Append( *type );
				ip.Append( _L( "] " ) );
				}
			ip.Append( *content );
			iListItems->AppendL( ip );

			CleanupStack::PopAndDestroy( item );
			CleanupStack::PopAndDestroy( content );
			CleanupStack::PopAndDestroy( type );
			CleanupStack::PopAndDestroy( sender );
			}
		pos = nl + 1;
		}

	if ( iListItems->MdcaCount() == 0 )
		{
		iListItems->AppendL( _L( "(无消息)" ) );
		}
	iListBox->HandleItemAdditionL();
	iListBox->SetCurrentItemIndex( 0 );
	iListBox->DrawNow();
	}

void CNokiachatContainer::ShowErrorNoteL( TInt aErr )
	{
	TBuf<64> msg;
	msg.Append( _L( "网络错误: " ) );
	msg.AppendNum( aErr );
	CAknErrorNote* note = new ( ELeave ) CAknErrorNote();
	note->ExecuteLD( msg );
	}

void CNokiachatContainer::HttpCompletedL( TInt aErr, const TDesC8& aBody )
	{
	if ( aErr != KErrNone )
		{
		ShowErrorNoteL( aErr );
		iReq = EReqNone;
		return;
		}

	if ( iReq == EReqConversations )
		{
		TRAPD( err, ParseConversationsL( aBody ) );
		if ( err != KErrNone )
			{
			ShowErrorNoteL( err );
			}
		}
	else if ( iReq == EReqMessages )
		{
		TRAPD( err, ParseMessagesL( aBody ) );
		if ( err != KErrNone )
			{
			ShowErrorNoteL( err );
			}
		}
	else if ( iReq == EReqSendText )
		{
		// Refresh messages after sending
		if ( iActiveConvId )
			{
			TRAPD( err, FetchMessagesL( *iActiveConvId ) );
			if ( err != KErrNone )
				{
				ShowErrorNoteL( err );
				}
			else
				{
				// FetchMessagesL sets iReq=EReqMessages for the follow-up request.
				return;
				}
			}
		}
	iReq = EReqNone;
	}
