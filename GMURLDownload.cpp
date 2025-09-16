//---------------------------------------------------------------------------

#pragma hdrstop

#include "GMURLDownload.h"
#include "wininet.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "wininet.lib")

//---------------------------------------------------------------------------
/*__fastcall TGMURLDownload::TGMURLDownload()
{
}

// ---------------------------------------------------------------------------
__fastcall TGMURLDownload::~TGMURLDownload( )
{
}
*/
//---------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE TGMURLDownload::OnStartBinding(DWORD dwReserved, __RPC__in_opt ::IBinding* pib)
{
	DoStart();
	return S_OK;
}

//---------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE TGMURLDownload::GetPriority(__RPC__out LONG *pnPriority)
{
	return S_OK;
}

//---------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE TGMURLDownload::OnLowResource(DWORD reserved)
{
	return S_OK;
}

//---------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE TGMURLDownload::OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, __RPC__in_opt LPCWSTR szStatusText)
{
	Boolean Abort = false;
	DoProgress(ulProgressMax, ulProgress, ulStatusCode, szStatusText, Abort);

	Application->ProcessMessages();

	if ((Abort) || (TGMURLDownload::FAbort)) {
		return E_ABORT;
	} else {
		return S_OK;
	}
}

//---------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE TGMURLDownload::OnStopBinding(HRESULT hresult, __RPC__in_opt LPCWSTR szError)
{
	FErrorMsg = szError;
	DoStop();
	return S_OK;
}

//---------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE TGMURLDownload::GetBindInfo(DWORD *grfBINDF, BINDINFO *pbindinfo)
{
	return S_OK;
}

//---------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE TGMURLDownload::OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC *pformatetc, STGMEDIUM *pstgmed)
{
	return S_OK;
}

//---------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE TGMURLDownload::OnObjectAvailable(__RPC__in REFIID riid, __RPC__in_opt IUnknown *punk)
{
	return S_OK;
}

//---------------------------------------------------------------------------
String __fastcall GetURLRoot(String URL)
{
	String rstr;
	if (URL.Length() > 0) {
		for (int i = 2; i <= URL.Length(); i++) {
			if ((URL[i] == L'/') && (URL[i - 1] != L'/') && (URL[i - 1] != L':')) {
				rstr = URL.SubString(i, URL.Length() - i + 1);
				break;
			}
		}
	}
	if ((rstr.Length() > 0) && (rstr[rstr.Length()] != L'/')) {
		rstr = rstr + L'/';
	}
	return rstr;
}

//---------------------------------------------------------------------------
String __fastcall GetBaseURL(String URL)
{
	String rstr;
	for (int i = URL.Length(); i >= 1; i--) {
		if (URL[i] == L'/') {
			rstr = URL.SubString(1, i);
			break;
		}
	}
	if ((rstr.Length() > 0) && (rstr[rstr.Length()] != L'/')) {
		rstr = rstr + L'/';
	}
	return rstr;
}

//---------------------------------------------------------------------------
String __fastcall ExtractURLFileName(String S)
{
	if (S.Length() > 0) {
		for (int i = S.Length(); i >= 1; i--) {
			if ((S[i] == L'/') || (S[i] == L'\\') ||
				(S[i] == L'=') || (S[i] == L'&')) {
					return S.SubString(i + 1, S.Length() - i);
			}
		}
	}
	return String();
}

//---------------------------------------------------------------------------
String __fastcall TGMURLDownload::GetLocation(String URL)
{
	String rstr;
	String Location = L"";

	TIdHTTP* Http = new TIdHTTP(NULL);
	try {
		Http->OnRedirect = HTTPRedirect;
		Http->Request->Referer = GetURLRoot(URL);
		Http->Head(URL);

		Location = Http->Response->Location;
	}
	__finally {
		delete Http;
    }

	if (Location != L"") {
		if (LowerCase(Location.SubString(1, StrLen(L"http://"))) != L"http://") {
			if (Location[1] == L'/') {
				Location.Delete(1, 1);
			}
			Location = GetBaseURL(URL) + Location;
		}
	}
	return GetLocation(Location);
}

//---------------------------------------------------------------------------
Boolean __fastcall TGMURLDownload::Download(String FileName)
{
	FAbort = false;

	//2013.11.26
	//김형기
	//URLEncoding추가
	//URL = L"http://10.100.124.30:9200/ysys/store/board/2014/08/05/20140805124909FI12182.zip";
	URL = TIdURI::URLEncode(URL, IndyTextEncoding_UTF8());
	HRESULT hr = URLDownloadToFileW((IUnknown*)NULL, URL.c_str(), FileName.c_str(), 0, (IBindStatusCallback*) this);
	if (hr == S_OK) {
//	if (URLDownloadToFileW((IUnknown*)NULL, L"http://192.168.0.100:9200/ysys/store/board/2013/1115/PDZVX10_XDCAM_VIEWER_v2302.zip", FileName.c_str(), 0, (IBindStatusCallback*) this) == S_OK) {
		DeleteUrlCacheEntry(URL.c_str()); // cache 삭제
		FLocalFileName = FileName;
		return (!FAbort) && (ErrorMsg == L"") && (FAfterOpen);
	}
	else
	{
		return false;
	}

	/*
	else if(hr == E_OUTOFMEMORY)
	{
		MessageBox(NULL, L"E_OUTOFMEMORY", L"error", MB_OK);
	}
	else if(hr == INET_E_DOWNLOAD_FAILURE)
	{
		MessageBox(NULL, L"INET_E_DOWNLOAD_FAILURE", L"error", MB_OK);
	}
	else
	{
		MessageBox(NULL, SysErrorMessage(GetLastError()).c_str(), L"error", MB_OK);
		//String strMsg = String().sprintf(L"ERROR : URLDownloadToFileW (%08X)", hr);
//		MessageBox(NULL, strMsg.c_str(), Application->Title.c_str(), MB_OK + MB_ICONERROR);
		return false;
	}
	*/
}

//---------------------------------------------------------------------------
void __fastcall TGMURLDownload::Abort()
{
  FAbort = True;
}

//---------------------------------------------------------------------------
void __fastcall TGMURLDownload::HTTPRedirect(System::TObject* Sender, String& Dest, int& NumRedirect, Boolean& Handled, TIdHTTPMethod& VMethod)
{
  Handled = True;
}

//---------------------------------------------------------------------------
void __fastcall TGMURLDownload::DoStart()
{
  FAfterOpen = True;
  if (FOnStart != NULL) {
	FOnStart(this);
  }
}

//---------------------------------------------------------------------------
void __fastcall TGMURLDownload::DoStop()
{
  if (FOnStop != NULL) {
	FOnStop(this);
  }
}

//---------------------------------------------------------------------------
void __fastcall TGMURLDownload::DoProgress(DWORD Max, DWORD Position, DWORD StatusCode, String StatusText, Boolean& Abort)
{
  if (FOnProgress != NULL) {
	FOnProgress(this, Max, Position, StatusCode, StatusText, Abort);
  }
}

