/*
========================================================================
 Name        : NokiachatHttpClient.cpp
 Author      : yaoyangyaha
 Description :
========================================================================
*/
#include "NokiachatHttpClient.h"

#include <es_sock.h>
#include <in_sock.h>
#include <utf.h>
#include <httpstringconstants.h>
#include <uri8.h>

// Simple fixed-size response buffer append helper
static const TInt KMaxResponseBytes = 32 * 1024;

class CNokiachatFormDataSupplier : public CBase, public MHTTPDataSupplier
	{
public:
	static CNokiachatFormDataSupplier* NewL( const TDesC8& aBody )
		{
		CNokiachatFormDataSupplier* self = new ( ELeave ) CNokiachatFormDataSupplier();
		CleanupStack::PushL( self );
		self->iBody = aBody.AllocL();
		CleanupStack::Pop( self );
		return self;
		}

	~CNokiachatFormDataSupplier()
		{
		delete iBody;
		}

public: // MHTTPDataSupplier
	TBool GetNextDataPart( TPtrC8& aDataPart )
		{
		if ( !iBody )
			{
			aDataPart.Set( KNullDesC8 );
			return ETrue;
			}
		aDataPart.Set( iBody->Des() );
		return ETrue;
		}

	void ReleaseData()
		{
		}

	TInt OverallDataSize()
		{
		return iBody ? iBody->Length() : 0;
		}

	TInt Reset()
		{
		return KErrNone;
		}

private:
	CNokiachatFormDataSupplier() : iBody( NULL ) {}

private:
	HBufC8* iBody;
	};

CNokiachatHttpClient* CNokiachatHttpClient::NewL( MNokiachatHttpObserver& aObserver )
	{
	CNokiachatHttpClient* self = new ( ELeave ) CNokiachatHttpClient( aObserver );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

CNokiachatHttpClient::CNokiachatHttpClient( MNokiachatHttpObserver& aObserver )
	: iObserver( aObserver ),
	  iDataSupplier( NULL ),
	  iResp( NULL ),
	  iConnected( EFalse ),
	  iInFlight( EFalse )
	{
	}

void CNokiachatHttpClient::ConstructL()
	{
	User::LeaveIfError( iSocketServ.Connect() );
	User::LeaveIfError( iConnection.Open( iSocketServ ) );

	iSession.OpenL();

	OpenConnectionL();
	}

CNokiachatHttpClient::~CNokiachatHttpClient()
	{
	Cancel();
	CloseConnection();
	iSession.Close();
	iConnection.Close();
	iSocketServ.Close();
	delete iDataSupplier;
	delete iResp;
	}

void CNokiachatHttpClient::OpenConnectionL()
	{
	if ( iConnected )
		{
		return;
		}

	TRequestStatus status;
	iConnection.Start( status );
	User::WaitForRequest( status );
	User::LeaveIfError( status.Int() );

	RStringPool strPool = iSession.StringPool();
	RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
	connInfo.SetPropertyL(
		strPool.StringF( HTTP::EHttpSocketServ, RHTTPSession::GetTable() ),
		THTTPHdrVal( iSocketServ.Handle() ) );
	connInfo.SetPropertyL(
		strPool.StringF( HTTP::EHttpSocketConnection, RHTTPSession::GetTable() ),
		THTTPHdrVal( iConnection.SubSessionHandle() ) );

	iConnected = ETrue;
	}

void CNokiachatHttpClient::CloseConnection()
	{
	if ( iConnected )
		{
		iConnection.Stop();
		iConnected = EFalse;
		}
	}

void CNokiachatHttpClient::EnsureRespBufferL()
	{
	if ( !iResp )
		{
		iResp = HBufC8::NewL( 1024 );
		iResp->Des().Zero();
		}
	else
		{
		iResp->Des().Zero();
		}
	}

void CNokiachatHttpClient::SetCommonHeadersL( RHTTPHeaders aHdr, const TDesC& aApiKey )
	{
	RStringPool strPool = iSession.StringPool();

	RStringF ua = strPool.StringF( HTTP::EUserAgent, RHTTPSession::GetTable() );
	RStringF uaVal = strPool.OpenFStringL( _L8( "nokiachat-s60" ) );
	CleanupClosePushL( uaVal );
	aHdr.SetFieldL( ua, THTTPHdrVal( uaVal ) );
	CleanupStack::PopAndDestroy(); // uaVal

	RStringF accept = strPool.StringF( HTTP::EAccept, RHTTPSession::GetTable() );
	RStringF acceptVal = strPool.OpenFStringL( _L8( "text/plain" ) );
	CleanupClosePushL( acceptVal );
	aHdr.SetFieldL( accept, THTTPHdrVal( acceptVal ) );
	CleanupStack::PopAndDestroy(); // acceptVal

	// Custom header: X-Api-Key
	RStringF headerName = strPool.OpenFStringL( _L8( "X-Api-Key" ) );
	CleanupClosePushL( headerName );
	HBufC8* apiKey8 = HBufC8::NewLC( aApiKey.Length() * 3 + 8 );
	TPtr8 apiKeyPtr( apiKey8->Des() );
	apiKeyPtr.Zero();
	CnvUtfConverter::ConvertFromUnicodeToUtf8( apiKeyPtr, aApiKey );
	RStringF apiKeyVal = strPool.OpenFStringL( apiKeyPtr );
	CleanupClosePushL( apiKeyVal );
	aHdr.SetFieldL( headerName, THTTPHdrVal( apiKeyVal ) );
	CleanupStack::PopAndDestroy(); // apiKeyVal
	CleanupStack::PopAndDestroy( apiKey8 );
	CleanupStack::PopAndDestroy(); // headerName
	}

void CNokiachatHttpClient::StartTransactionL(
		const TDesC8& aUrl8,
		RStringF aMethod,
		const TDesC& aApiKey )
	{
	if ( iInFlight )
		{
		User::Leave( KErrInUse );
		}

	EnsureRespBufferL();

	TUriParser8 uri;
	User::LeaveIfError( uri.Parse( aUrl8 ) );
	iTransaction = iSession.OpenTransactionL( uri, *this, aMethod );
	SetCommonHeadersL( iTransaction.Request().GetHeaderCollection(), aApiKey );
	iInFlight = ETrue;
	}

void CNokiachatHttpClient::GetL( const TDesC& aUrl, const TDesC& aApiKey )
	{
	OpenConnectionL();

	HBufC8* url8 = HBufC8::NewLC( aUrl.Length() * 3 + 8 );
	TPtr8 urlPtr( url8->Des() );
	urlPtr.Zero();
	CnvUtfConverter::ConvertFromUnicodeToUtf8( urlPtr, aUrl );

	RStringPool strPool = iSession.StringPool();
	StartTransactionL( urlPtr, strPool.StringF( HTTP::EGET, RHTTPSession::GetTable() ), aApiKey );
	iTransaction.SubmitL();
	CleanupStack::PopAndDestroy( url8 );
	}

void CNokiachatHttpClient::PostFormL( const TDesC& aUrl, const TDesC& aApiKey, const TDesC8& aBody )
	{
	OpenConnectionL();

	delete iDataSupplier;
	iDataSupplier = NULL;
	iDataSupplier = CNokiachatFormDataSupplier::NewL( aBody );

	HBufC8* url8 = HBufC8::NewLC( aUrl.Length() * 3 + 8 );
	TPtr8 urlPtr( url8->Des() );
	urlPtr.Zero();
	CnvUtfConverter::ConvertFromUnicodeToUtf8( urlPtr, aUrl );

	RStringPool strPool = iSession.StringPool();
	StartTransactionL( urlPtr, strPool.StringF( HTTP::EPOST, RHTTPSession::GetTable() ), aApiKey );

	iTransaction.Request().SetBody( *iDataSupplier );
	RHTTPHeaders hdr = iTransaction.Request().GetHeaderCollection();
	RStringF ct = strPool.StringF( HTTP::EContentType, RHTTPSession::GetTable() );
	RStringF ctVal = strPool.OpenFStringL( _L8( "application/x-www-form-urlencoded" ) );
	CleanupClosePushL( ctVal );
	hdr.SetFieldL( ct, THTTPHdrVal( ctVal ) );
	CleanupStack::PopAndDestroy(); // ctVal

	iTransaction.SubmitL();

	CleanupStack::PopAndDestroy( url8 );
	}

void CNokiachatHttpClient::Cancel()
	{
	if ( iInFlight )
		{
		iTransaction.Cancel();
		iTransaction.Close();
		iInFlight = EFalse;
		}
	}

void CNokiachatHttpClient::Finish( TInt aErr )
	{
	if ( iInFlight )
		{
		iTransaction.Close();
		iInFlight = EFalse;
		}
	if ( iResp )
		{
		TRAP_IGNORE( iObserver.HttpCompletedL( aErr, iResp->Des() ) );
		}
	else
		{
		TRAP_IGNORE( iObserver.HttpCompletedL( aErr, KNullDesC8 ) );
		}
	}

void CNokiachatHttpClient::MHFRunL( RHTTPTransaction aTransaction, const THTTPEvent& aEvent )
	{
	switch ( aEvent.iStatus )
		{
		case THTTPEvent::EGotResponseBodyData:
			{
			MHTTPDataSupplier* body = aTransaction.Response().Body();
			TPtrC8 dataPart;
			body->GetNextDataPart( dataPart );
			if ( iResp && dataPart.Length() > 0 )
				{
				TInt newLen = iResp->Length() + dataPart.Length();
				if ( newLen > KMaxResponseBytes )
					{
					body->ReleaseData();
					Finish( KErrOverflow );
					return;
					}
				iResp = iResp->ReAllocL( newLen );
				iResp->Des().Append( dataPart );
				}
			body->ReleaseData();
			}
			break;
		case THTTPEvent::EResponseComplete:
			break;
		case THTTPEvent::ESucceeded:
			Finish( KErrNone );
			break;
		case THTTPEvent::EFailed:
			Finish( KErrGeneral );
			break;
		default:
			break;
		}
	}

TInt CNokiachatHttpClient::MHFRunError( TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/ )
	{
	Finish( aError );
	return KErrNone;
	}
