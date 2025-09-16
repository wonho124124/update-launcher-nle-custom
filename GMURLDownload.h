//---------------------------------------------------------------------------

#ifndef GMURLDownloadH
#define GMURLDownloadH

#include <Winapi.Windows.hpp>	// Pascal unit
#include <Winapi.Urlmon.hpp>	// Pascal unit
#include <Winapi.ActiveX.hpp>	// Pascal unit
#include <System.SysUtils.hpp>	// Pascal unit
#include <System.Classes.hpp>	// Pascal unit
#include <Vcl.Forms.hpp>	// Pascal unit

#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>

#include <urlmon.h>
//---------------------------------------------------------------------------

typedef void __fastcall (__closure *TOnProgressEvent)(System::TObject* Sender, DWORD Max, DWORD Position, DWORD StatusCode, String StatusText, Boolean& Abort);

class TGMURLDownload: public System::TInterfacedObject, public IBindStatusCallback
{
	typedef System::TInterfacedObject inherited;

  private:
	String FURL;
	Boolean FAbort;
	TNotifyEvent FOnStart;
	TNotifyEvent FOnStop;
	TOnProgressEvent FOnProgress;
	String FLocalFileName;
	String FErrorMsg;
	Boolean FAfterOpen;

	String __fastcall GetLocation(String URL);
	void __fastcall HTTPRedirect(System::TObject* Sender, String& Dest, int& NumRedirect, Boolean& Handled, TIdHTTPMethod& VMethod);
	void __fastcall DoStart();
	void __fastcall DoStop();
	void __fastcall DoProgress(DWORD Max, DWORD Position, DWORD StatusCode, String StatusText, Boolean& Abort);
  protected:
	// IBindStatusCallback
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject)
	{ return E_NOTIMPL; }

	virtual ULONG STDMETHODCALLTYPE AddRef( void)
	{ return 0; }

	virtual ULONG STDMETHODCALLTYPE Release( void)
	{ return 0; }

	HRESULT STDMETHODCALLTYPE OnStartBinding(DWORD dwReserved, __RPC__in_opt ::IBinding* pib);
	HRESULT STDMETHODCALLTYPE GetPriority(__RPC__out LONG *pnPriority);
	HRESULT STDMETHODCALLTYPE OnLowResource(DWORD reserved);
	HRESULT STDMETHODCALLTYPE OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, __RPC__in_opt LPCWSTR szStatusText);
	HRESULT STDMETHODCALLTYPE OnStopBinding(HRESULT hresult, __RPC__in_opt LPCWSTR szError);
	HRESULT STDMETHODCALLTYPE GetBindInfo(DWORD *grfBINDF, BINDINFO *pbindinfo);
	HRESULT STDMETHODCALLTYPE OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC *pformatetc, STGMEDIUM *pstgmed);
	HRESULT STDMETHODCALLTYPE OnObjectAvailable(__RPC__in REFIID riid, __RPC__in_opt IUnknown *punk);
  public:
	/* TObject.Create */ inline __fastcall virtual TGMURLDownload(void) : System::TInterfacedObject() { }
	/* TObject.Destroy */ inline __fastcall virtual ~TGMURLDownload(void) { }
//	__fastcall TGMURLDownload();
//	__fastcall ~TGMURLDownload();

	Boolean __fastcall Download(String FileName);
	void __fastcall Abort();

	__property String ErrorMsg = {read=FErrorMsg};
	__property String LocalFileName = {read=FLocalFileName};
	__property String URL = {read=FURL, write=FURL};
	__property TNotifyEvent OnStart = {read=FOnStart, write=FOnStart};
	__property TNotifyEvent OnStop = {read=FOnStop, write=FOnStop};
	__property TOnProgressEvent OnProgressStatus = {read=FOnProgress, write=FOnProgress};
private:
	void *__IBindStatusCallback;	/* IDropTarget */

public:
	operator IBindStatusCallback*(void) { return (IBindStatusCallback*)&__IBindStatusCallback; }

};

#endif
