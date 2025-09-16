// CodeGear C++Builder
// Copyright (c) 1995, 2014 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'GMSWebBrowser.pas' rev: 28.00 (Windows)

#ifndef GmswebbrowserHPP
#define GmswebbrowserHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <Winapi.Windows.hpp>	// Pascal unit
#include <System.Classes.hpp>	// Pascal unit
#include <System.Variants.hpp>	// Pascal unit
#include <System.Win.StdVCL.hpp>	// Pascal unit
#include <Vcl.Forms.hpp>	// Pascal unit
#include <Vcl.Graphics.hpp>	// Pascal unit
#include <Vcl.OleCtrls.hpp>	// Pascal unit
#include <Vcl.OleServer.hpp>	// Pascal unit
#include <Winapi.ActiveX.hpp>	// Pascal unit
#include <Winapi.Messages.hpp>	// Pascal unit
#include <System.SysUtils.hpp>	// Pascal unit
#include <SHDocVw.hpp>	// Pascal unit
#include <IEConst.hpp>	// Pascal unit
#include <Vcl.Controls.hpp>	// Pascal unit
#include <System.Types.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gmswebbrowser
{
//-- type declarations -------------------------------------------------------
struct TDOCHOSTUIINFO;
typedef TDOCHOSTUIINFO *PDOCHOSTUIINFO;

struct DECLSPEC_DRECORD TDOCHOSTUIINFO
{
public:
	unsigned cbSize;
	unsigned dwFlags;
	unsigned dwDoubleClick;
};


__interface IDocHostShowUI;
typedef System::DelphiInterface<IDocHostShowUI> _di_IDocHostShowUI;
__interface  INTERFACE_UUID("{C4D244B0-D43E-11CF-893B-00AA00BDCE1A}") IDocHostShowUI  : public System::IInterface 
{
	
public:
	virtual HRESULT __stdcall ShowMessage(NativeUInt hwnd, System::WideChar * lpstrText, System::WideChar * lpstrCaption, int dwType, System::WideChar * lpstrHelpFile, int dwHelpContext, NativeInt &plResult) = 0 ;
	virtual HRESULT __stdcall ShowHelp(NativeUInt hwnd, System::WideChar * pszHelpFile, int uCommand, int dwData, const System::Types::TPoint ptMouse, _di_IDispatch &pDispachObjectHit) = 0 ;
};

__interface IDocHostUIHandler;
typedef System::DelphiInterface<IDocHostUIHandler> _di_IDocHostUIHandler;
__interface  INTERFACE_UUID("{BD3F23C0-D43E-11CF-893B-00AA00BDCE1A}") IDocHostUIHandler  : public System::IInterface 
{
	
public:
	virtual HRESULT __stdcall ShowContextMenu(const unsigned dwID, const System::Types::PPoint ppt, const System::_di_IInterface pcmdtReserved, const _di_IDispatch pdispReserved) = 0 ;
	virtual HRESULT __stdcall GetHostInfo(TDOCHOSTUIINFO &pInfo) = 0 ;
	virtual HRESULT __stdcall ShowUI(const unsigned dwID, const _di_IOleInPlaceActiveObject pActiveObject, const _di_IOleCommandTarget pCommandTarget, const _di_IOleInPlaceFrame pFrame, const _di_IOleInPlaceUIWindow pDoc) = 0 ;
	virtual HRESULT __stdcall HideUI(void) = 0 ;
	virtual HRESULT __stdcall UpdateUI(void) = 0 ;
	virtual HRESULT __stdcall EnableModeless(const BOOL fEnable) = 0 ;
	virtual HRESULT __stdcall OnDocWindowActivate(const BOOL fActivate) = 0 ;
	virtual HRESULT __stdcall OnFrameWindowActivate(const BOOL fActivate) = 0 ;
	virtual HRESULT __stdcall ResizeBorder(const System::Types::PRect prcBorder, const _di_IOleInPlaceUIWindow pUIWindow, const BOOL fRameWindow) = 0 ;
	virtual HRESULT __stdcall TranslateAccelerator(const Winapi::Windows::PMsg lpMsg, const System::PGUID pguidCmdGroup, const unsigned nCmdID) = 0 ;
	virtual HRESULT __stdcall GetOptionKeyPath(System::WideChar * &pchKey, const unsigned dw) = 0 ;
	virtual HRESULT __stdcall GetDropTarget(const _di_IDropTarget pDropTarget, /* out */ _di_IDropTarget &ppDropTarget) = 0 ;
	virtual HRESULT __stdcall GetExternal(/* out */ _di_IDispatch &ppDispatch) = 0 ;
	virtual HRESULT __stdcall TranslateUrl(const unsigned dwTranslate, const System::WideChar * pchURLIn, System::WideChar * &ppchURLOut) = 0 ;
	virtual HRESULT __stdcall FilterDataObject(const _di_IDataObject pDO, /* out */ _di_IDataObject &ppDORet) = 0 ;
};

class DELPHICLASS TEnhancedWebBrowserUI;
#pragma pack(push,4)
class PASCALIMPLEMENTATION TEnhancedWebBrowserUI : public System::Classes::TPersistent
{
	typedef System::Classes::TPersistent inherited;
	
private:
	bool FScrollBar;
	bool FBorder3D;
	bool FFlatScrollBars;
	bool FContextMenu;
	
public:
	__fastcall TEnhancedWebBrowserUI(void);
	
__published:
	__property bool ScrollBars = {read=FScrollBar, write=FScrollBar, nodefault};
	__property bool FlatScrollBars = {read=FFlatScrollBars, write=FFlatScrollBars, nodefault};
	__property bool ContextMenu = {read=FContextMenu, write=FContextMenu, nodefault};
	__property bool Border3D = {read=FBorder3D, write=FBorder3D, nodefault};
public:
	/* TPersistent.Destroy */ inline __fastcall virtual ~TEnhancedWebBrowserUI(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS TGMSWebBrowser;
class PASCALIMPLEMENTATION TGMSWebBrowser : public Shdocvw::TWebBrowser
{
	typedef Shdocvw::TWebBrowser inherited;
	
private:
	int FRefCount;
	TEnhancedWebBrowserUI* FEnhancedWebBrowserUI;
	void *FDefInetExplorerServerProc;
	void *FDefShellObjViewProc;
	NativeUInt FShellDocObjViewHandle;
	NativeUInt FInetExplorerServerHandle;
	void *FShellDocObjInstance;
	void *FInetExplorerServerInstance;
	bool FHooksSet;
	Shdocvw::TWebBrowserDocumentComplete FSaveDocumentCompleteEvent;
	void __fastcall HookChildWindows(void);
	void __fastcall InetExplorerServerWndProc(Winapi::Messages::TMessage &Message);
	void __fastcall ShellDocObjWndProc(Winapi::Messages::TMessage &Message);
	void __fastcall UnHookChildWindows(void);
	void __fastcall DoDocumentComplete(System::TObject* ASender, const _di_IDispatch pDisp, const System::OleVariant &URL);
	
protected:
	HIDESBASE HRESULT __stdcall QueryInterface(const GUID &IID, /* out */ void *Obj);
	HIDESBASE int __stdcall _AddRef(void);
	HIDESBASE int __stdcall _Release(void);
	virtual void __fastcall CreateWnd(void);
	virtual void __fastcall DestroyWnd(void);
	
public:
	__fastcall virtual TGMSWebBrowser(System::Classes::TComponent* AOwner);
	__fastcall virtual ~TGMSWebBrowser(void);
	HRESULT __stdcall ShowContextMenu(const unsigned dwID, const System::Types::PPoint ppt, const System::_di_IInterface pcmdtReserved, const _di_IDispatch pdispReserved);
	HRESULT __stdcall GetHostInfo(TDOCHOSTUIINFO &pInfo);
	HRESULT __stdcall ShowUI(const unsigned dwID, const _di_IOleInPlaceActiveObject pActiveObject, const _di_IOleCommandTarget pCommandTarget, const _di_IOleInPlaceFrame pFrame, const _di_IOleInPlaceUIWindow pDoc);
	HRESULT __stdcall HideUI(void);
	HRESULT __stdcall UpdateUI(void);
	HIDESBASE HRESULT __stdcall EnableModeless(const BOOL fEnable);
	HRESULT __stdcall OnDocWindowActivate(const BOOL fActivate);
	HRESULT __stdcall OnFrameWindowActivate(const BOOL fActivate);
	HRESULT __stdcall ResizeBorder(const System::Types::PRect prcBorder, const _di_IOleInPlaceUIWindow pUIWindow, const BOOL fRameWindow);
	HRESULT __stdcall TranslateAccelerator(const Winapi::Windows::PMsg lpMsg, const System::PGUID pguidCmdGroup, const unsigned nCmdID);
	HRESULT __stdcall GetOptionKeyPath(System::WideChar * &pchKey, const unsigned dw);
	HRESULT __stdcall GetDropTarget(const _di_IDropTarget pDropTarget, /* out */ _di_IDropTarget &ppDropTarget);
	HRESULT __stdcall GetExternal(/* out */ _di_IDispatch &ppDispatch);
	HRESULT __stdcall TranslateUrl(const unsigned dwTranslate, const System::WideChar * pchURLIn, System::WideChar * &ppchURLOut);
	HRESULT __stdcall FilterDataObject(const _di_IDataObject pDO, /* out */ _di_IDataObject &ppDORet);
	HRESULT __stdcall ShowMessage(NativeUInt hwnd, System::WideChar * lpstrText, System::WideChar * lpstrCaption, int dwType, System::WideChar * lpstrHelpFile, int dwHelpContext, NativeInt &plResult);
	HRESULT __stdcall ShowHelp(NativeUInt hwnd, System::WideChar * pszHelpFile, int uCommand, int dwData, const System::Types::TPoint ptMouse, _di_IDispatch &pDispachObjectHit);
	
__published:
	__property TEnhancedWebBrowserUI* UI = {read=FEnhancedWebBrowserUI};
public:
	/* TWinControl.CreateParented */ inline __fastcall TGMSWebBrowser(HWND ParentWindow) : Shdocvw::TWebBrowser(ParentWindow) { }
	
private:
	void *__IDocHostShowUI;	// IDocHostShowUI 
	void *__IDocHostUIHandler;	// IDocHostUIHandler 
	
public:
	#if defined(MANAGED_INTERFACE_OPERATORS)
	// {C4D244B0-D43E-11CF-893B-00AA00BDCE1A}
	operator _di_IDocHostShowUI()
	{
		_di_IDocHostShowUI intf;
		GetInterface(intf);
		return intf;
	}
	#else
	operator IDocHostShowUI*(void) { return (IDocHostShowUI*)&__IDocHostShowUI; }
	#endif
	#if defined(MANAGED_INTERFACE_OPERATORS)
	// {BD3F23C0-D43E-11CF-893B-00AA00BDCE1A}
	operator _di_IDocHostUIHandler()
	{
		_di_IDocHostUIHandler intf;
		GetInterface(intf);
		return intf;
	}
	#else
	operator IDocHostUIHandler*(void) { return (IDocHostUIHandler*)&__IDocHostUIHandler; }
	#endif
	#if defined(MANAGED_INTERFACE_OPERATORS)
	// {00000000-0000-0000-C000-000000000046}
	operator System::_di_IInterface()
	{
		System::_di_IInterface intf;
		GetInterface(intf);
		return intf;
	}
	#else
	operator System::IInterface*(void) { return (System::IInterface*)&__IDocHostShowUI; }
	#endif
	
};


class DELPHICLASS TWebBrowser;
class PASCALIMPLEMENTATION TWebBrowser : public TGMSWebBrowser
{
	typedef TGMSWebBrowser inherited;
	
public:
	/* TGMSWebBrowser.Create */ inline __fastcall virtual TWebBrowser(System::Classes::TComponent* AOwner) : TGMSWebBrowser(AOwner) { }
	/* TGMSWebBrowser.Destroy */ inline __fastcall virtual ~TWebBrowser(void) { }
	
public:
	/* TWinControl.CreateParented */ inline __fastcall TWebBrowser(HWND ParentWindow) : TGMSWebBrowser(ParentWindow) { }
	
};


//-- var, const, procedure ---------------------------------------------------
static const System::Int8 DOCHOSTUITYPE_BROWSE = System::Int8(0x0);
static const System::Int8 DOCHOSTUITYPE_AUTHOR = System::Int8(0x1);
static const System::Int8 DOCHOSTUIDBLCLK_DEFAULT = System::Int8(0x0);
static const System::Int8 DOCHOSTUIDBLCLK_SHOWPROPERTIES = System::Int8(0x1);
static const System::Int8 DOCHOSTUIDBLCLK_SHOWCODE = System::Int8(0x2);
static const System::Int8 DOCHOSTUIFLAG_DIALOG = System::Int8(0x1);
static const System::Int8 DOCHOSTUIFLAG_DISABLE_HELP_MENU = System::Int8(0x2);
static const System::Int8 DOCHOSTUIFLAG_NO3DBORDER = System::Int8(0x4);
static const System::Int8 DOCHOSTUIFLAG_SCROLL_NO = System::Int8(0x8);
static const System::Int8 DOCHOSTUIFLAG_DISABLE_SCRIPT_INACTIVE = System::Int8(0x10);
static const System::Int8 DOCHOSTUIFLAG_OPENNEWWIN = System::Int8(0x20);
static const System::Int8 DOCHOSTUIFLAG_DISABLE_OFFSCREEN = System::Int8(0x40);
static const System::Byte DOCHOSTUIFLAG_FLAT_SCROLLBAR = System::Byte(0x80);
static const System::Word DOCHOSTUIFLAG_DIV_BLOCKDEFAULT = System::Word(0x100);
static const System::Word DOCHOSTUIFLAG_ACTIVATE_CLIENTHIT_ONLY = System::Word(0x200);
}	/* namespace Gmswebbrowser */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_GMSWEBBROWSER)
using namespace Gmswebbrowser;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// GmswebbrowserHPP
