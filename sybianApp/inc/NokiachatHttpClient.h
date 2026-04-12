/*
========================================================================
 Name        : NokiachatHttpClient.h
 Author      : yaoyangyaha
 Description :
========================================================================
*/
#ifndef NOKIACHAT_HTTP_CLIENT_H
#define NOKIACHAT_HTTP_CLIENT_H

#include <e32base.h>
#include <es_sock.h>
#include <http.h>

class MNokiachatHttpObserver
	{
public:
	virtual void HttpCompletedL( TInt aErr, const TDesC8& aBody ) = 0;
	};

class CNokiachatFormDataSupplier;

class CNokiachatHttpClient : public CBase, public MHTTPTransactionCallback
	{
public:
	static CNokiachatHttpClient* NewL( MNokiachatHttpObserver& aObserver );
	~CNokiachatHttpClient();

public:
	void GetL( const TDesC& aUrl, const TDesC& aApiKey );
	void PostFormL( const TDesC& aUrl, const TDesC& aApiKey, const TDesC8& aBody );
	void Cancel();

private:
	CNokiachatHttpClient( MNokiachatHttpObserver& aObserver );
	void ConstructL();

private: // MHTTPTransactionCallback
	void MHFRunL( RHTTPTransaction aTransaction, const THTTPEvent& aEvent );
	TInt MHFRunError( TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent );

private:
	void OpenConnectionL();
	void CloseConnection();
	void StartTransactionL( const TDesC8& aUrl8, RStringF aMethod, const TDesC& aApiKey );
	void SetCommonHeadersL( RHTTPHeaders aHdr, const TDesC& aApiKey );
	void EnsureRespBufferL();
	void Finish( TInt aErr );

private:
	MNokiachatHttpObserver& iObserver;

private:
	RSocketServ iSocketServ;
	RConnection iConnection;
	RHTTPSession iSession;
	RHTTPTransaction iTransaction;

	CNokiachatFormDataSupplier* iDataSupplier;
	HBufC8* iResp;
	TBool iConnected;
	TBool iInFlight;
	};

#endif // NOKIACHAT_HTTP_CLIENT_H
