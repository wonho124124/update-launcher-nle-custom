// ---------------------------------------------------------------------------

#ifndef GMUTILH
#define GMUTILH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

#include <Mshtml.h>
#include <Web.HTTPApp.hpp>

#include <Xml.Win.msxmldom.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>

#include "AdvGrid.hpp"
#include "AdvCGrid.hpp"
#include "AdvObj.hpp"
#include "BaseGrid.hpp"

#include <DateUtils.hpp>
#include <SHDocVw.hpp>
#include "GMSWebBrowser.hpp"
#include <System.hpp>
#include "MgrUserInfo.h"

#include "assert.h"
#include <tchar.h>
#include "Wininet.h"

/* 파일 사이즈 */
#define SIZE_KB 1024
#define SIZE_MB 1048576
#define SIZE_GB 1073741824
#define SIZE_TB 1099511627776

#define ZEROTIMECODE "00:00"

#define IDS_DEFAULT_GRID_ROW_HEIGHT 27	// 그리드 로우의 높이

#define EDITOR_LINE_SPACE 25

enum TTimeType { ttHHMM, ttMMSS };

// ---------------------------------------------------------------------------

#define SAFE_DELETE(obj) \
	if((obj) != NULL)\
	{\
		delete (obj);\
		(obj) = NULL;\
	}

#define SAFE_RELEASE(obj) \
	if((obj) != NULL)\
	{\
		obj->Release();\
		(obj) = NULL;\
	}

#define BLACK_BG_HTML \
		L"<html> \
			<head> \
			<style> \
				body { \
					margin: 0 auto; \
					overflow: hidden; \
				} \
			</style> \
			<body bgcolor=\"#000000\"> \
			</body> \
		  </html>"

#define 	SEC2HOUR 			3600
#define 	SEC3MIN 			60
#define 	WORD_PER_SEC 0.19  //1초에 5.2자 기준

// 기사 작성창이 있을때 넓이
#define DEFAULT_CLIENT_WINDOW_WIDTH 	1350
#define DEFAULT_ARTICLE_WINDOW_WIDTH 	985
#define DEFAULT_ARICLE_OTHER_WIDTH		985
#define DEFAULT_ARTICLE_WINDOW_PERCENT 50

// 공통
// Clear detail information
// WParam : int, rowindex
#define WM_OPEN_INIT 				WM_USER + 100
#define WM_SESSION_OUT 				WM_USER + 101
#define WM_CLEARINFO				WM_USER + 2001
// Set display detail information
// WParam : int, rowindex
#define WM_SETDISPLAYINFO			WM_USER + 2002

/* 이상봉 Main 2014-02-20 */
// Set MDI TAB INDEX by MDI Child Form
// WParam : TForm *
#define WM_MAIN_SETTABINDEXBYFORM	WM_USER + 2101
// Notification WM_MDIACTIVATE
// WParam : TForm *
#define WM_MAIN_MDIACTIVATE 		WM_USER + 2102
// Notification MDI WM_SIZE - SIZE_RESTORED
// WParam : TForm *
#define WM_MAIN_MDIRESTORED	 		WM_USER + 2103
// Notification MDI WM_SIZE - SIZE_MINIMIZED
// WParam : TForm *
#define WM_MAIN_MDIMINIMIZED	 	WM_USER + 2104
// Notification MDI WM_SIZE	- SIZE_MAXIMIZED
// WParam : TForm *
#define WM_MAIN_MDIMAXIMIZED	 	WM_USER + 2105
// Clear MDI Chuld caption
#define WM_MAIN_CHILD_CAPCLEAR      WM_USER + 2111
// Restore MDI Chuld caption
#define WM_MAIN_CHILD_CAPRESTORE	WM_USER + 2112

// 게시판
// Clear detail information
// WParam : int, rowindex
#define WM_BB_CLEARINFO				WM_USER + 2201
// Set display detail information
// WParam : int, rowindex
#define WM_BB_SETDISPLAYINFO		WM_USER + 2202
// Restore content
#define WM_BB_CONTENTRESTORE		WM_USER + 2203
// Add comment
// WParam : String, comment content
// LParam : not use
#define WM_BB_ADDCOMMENT			WM_USER + 2211
// Update comment
// WParam : String, comment content
// LParam : String, comment sequence
#define WM_BB_UPDATECOMMENT			WM_USER + 2212
// Delete comment
// WParam : int, comment index
// LParam : String, comment sequence
#define WM_BB_DELETECOMMENT			WM_USER + 2213
// Download file
// WParam : String, file path
// LParam : String, file name
#define WM_BB_DOWNLOADFILE			WM_USER + 2221

#define PREROW_CHECK_INTERVAL	    150

// 마이노트
// Clear detail information
// WParam : int, rowindex
#define WM_MYNOTE_CLEARINFO			WM_USER + 2301
// Set display detail information
// WParam : int, rowindex
#define WM_MYNOTE_SETDISPLAYINFO	WM_USER + 2302
// Restore content
#define WM_MYNOTE_CONTENTRESTORE	WM_USER + 2303

// 영상보관함
#define WM_VIDEOBOX_SETDISPLAYINFO  WM_USER + 2402

// 기사
#define WM_ARTICLE_AUTOSAVE_DONE 	WM_USER + 4000
#define WM_ARTICLECONTENT_LOAD 		WM_USER + 4001
#define WM_ARTICLEMEDIA_LOAD 		WM_USER + 4002
#define WM_ARTICLEATTACH_LOAD 		WM_USER + 4003
#define WM_ARTICLESNS_LOAD 			WM_USER + 4004
#define WM_ARTICLEORDER_LOAD 		WM_USER + 4005
#define WM_ARTICLELIST_REFRESH 		WM_USER + 4006
#define WM_DR_ARTCL_HIST_REFRESH 	WM_USER + 4007
#define WM_ARTICLE_SNS_WARNNING 	WM_USER + 4010
#define WM_ARTICLE_CLOSE_VIEW 		WM_USER + 4011
#define WM_ARTICLE_MEDIA_PREVIEW 	WM_USER + 4012
#define WM_WIRE_SETDISPLAYINFO		WM_USER + 4013
#define WM_ARTICLE_OPEN_INIT		WM_USER + 4014

// 시스템
#define WM_SYSUSERMGT_SETUSERINFO 	WM_USER+9000

// 자료
#define WM_MAMVIDEOLIST_SETPREVIEW 	WM_USER+6000
#define WM_MAM_LOAD_HUMAN_CLASS WM_USER+6001

#define WM_LOGOUT WM_USER+100

// ****************************************************************/
// 설명 :   윈도우 사용자 메시지 모음
// 입력 :
// 결과 :
// 예제:
// 작성자/날짜: mhSung /
// ****************************************************************/
/* 런다운 실행에 대한 버튼 리턴 값 정의 */
#define IDM_BTN_NONE		0
#define IDM_BTN_RDWRITE 	1
#define IDM_BTN_RDSEARCH 	2
#define IDM_BTN_RD_IMGINFO 	3
#define IDM_BTN_RD_VIEW		4

#define 	WM_MAIN_CHANGE_PWD          WM_USER+9999
// Ysys-UDP Close Message  2014-07-24 mh
#define 	WM_UDP_CLOSE				WM_USER+9998

#define 	WM_RUNDOWN_DETAIL 			WM_USER+5000
#define 	WM_RUNDOWN_LIST_REFRESH 	WM_USER+5001
#define 	WM_RUNDOWN_YTN_LIST_REFRESH WM_USER+5002
#define		WM_RUNDOWN_ARTICLE_OPEN		WM_USER+5003
#define 	WM_RUNDOWN_MENU_STATUS 		WM_USER+5004
#define 	WM_RUNDOWN_CLOSE	 		WM_USER+5005
#define 	WM_OPEN_RUNDOWN_DETAIL  	WM_USER+5006
#define 	WM_RUNDOWN_ORDER_LOAD		WM_USER+5007
#define 	WM_RUNDOWN_MEDIA_LIST_LOAD	WM_USER+5008
#define 	WM_RUNDOWNOF_ARTICLE_OPEN	WM_USER+5009
#define 	WM_RUNDOWNOR_MEDIA_LIST_LOAD WM_USER+5010
#define 	WM_RUNDOWN_TAB_OR_LOAD		WM_USER+5011
#define 	WM_RUNDOWN_TAB_ARTICLE_LOAD	WM_USER+5013
#define 	WM_RUNDOWN_ARTICLE_LOCK 	WM_USER+5014
#define 	WM_RUNDOWN_TRANS_DISPLAY   	WM_USER+5015
#define 	WM_RUNDOWN_CLOSE_VIEW   	WM_USER+5016
#define 	WM_RUNDOWN_OR_CLOSE_VIEW	WM_USER+5017
#define 	WM_RUNDOWN_OR_LOAD_VIEW		WM_USER+5018
#define 	WM_RUNDOWN_ARTICLE_CLOSE	WM_USER+5019

// 런다운 TCP 메시지 2014-07-21
#define		WM_RDTCPSERVER_ACTIVE       WM_USER+5101
#define 	WM_RDTCPSERVER_PROCESS      WM_USER+5102
#define 	WM_RDTCPSERVER_STATUS		WM_USER+5103
#define 	WM_RDTCPCLIENT_PROCESS		WM_USER+5104

/* jbkim 채팅 프로그램 2014-01-16 */
#define 	WM_SHOW_CHAT 				WM_USER+5012

#define DF_COLOR_ORD_LOCK 	Vcl::Graphics::clHotLight
#define DF_COLOR_UNLOCK 	Vcl::Graphics::clGray

// ****************************************************************/
// UnitArticleWrite.h 에서 옮겨옴 >> 2014-04-09 mh
typedef struct _MacroButtonInfo
{ // 기사 작성 시간 계산하기 위해서

	String Index;
	String ID; // 혹시나 모를 확장을 위해서 필요할까봐 미리 만듦
	String Caption;
	String Macro;
	TControl * Button; // 버튼 컨트롤 포인터
	int TextWidth;
} MACRO_BUTTON_INFO, *pMACRO_BUTTON_INFO;

enum TMessageType
{
	mstInfo, mstWarnning, mstError, mstQuestion
} ;

class TErrorMessageBox
{
public:
	static void __fastcall Show( String Message, TMessageType MessageType, String Caption = String( ) );
	static void __fastcall Show( String Message, String Description = String( ) );
} ;

void __fastcall TErrorMessageBox::Show( String Message, TMessageType MessageType, String Caption )
{
	UINT uIcon = MB_ICONERROR;
	if ( Caption.IsEmpty( ) )
	{
		switch ( MessageType )
		{
		case mstInfo:
			Caption = L"알림";
			uIcon = MB_ICONINFORMATION;
			break;
		case mstWarnning:
			Caption = L"경고";
			uIcon = MB_ICONWARNING;
			break;
		case mstError:
			Caption = L"오류";
			uIcon = MB_ICONERROR;
			break;
		case mstQuestion:
			Caption = L"확인";
			uIcon = MB_ICONQUESTION;
			break;
		default:
			;
		}
	}
	Application->MessageBox( Message.c_str( ), Caption.c_str( ), uIcon | MB_OK );
}

void __fastcall TErrorMessageBox::Show( String Message, const String Description )
{
	if ( !Description.IsEmpty( ) )
	{
		Message = String( ).sprintf( L"%s\n오류내용:%s", Message.c_str( ), Description.c_str( ) );
	}
	TErrorMessageBox::Show( Message.c_str( ), mstError, L"오류" );
}

enum LogType
{
	ltInfo, ltWarning, ltError
} ;

struct LogGlobal
{
	static String ch_div_cd;
} ;

class TGMLogger
{
private:

protected:

public:
	__fastcall TGMLogger( );
	__fastcall ~TGMLogger( );
	static void __fastcall WriteLog( LogType lt, String strMsg );
} ;

enum TEditMode
{
	emNew,
	emUpdate,
	emReadOnly
} ;

String __fastcall replaceNull( String value );
String __fastcall GetApprvDivCd( int nIndex );
void __fastcall GMSaveToXmlFile( String xml, String FileName, bool use = false );
void __fastcall GMSaveToXmlFileRd( String xml, String FileName );
/* */
void __fastcall ResizeButtonSize( TControl * Button, TCanvas * Canvas );
String __fastcall GetNodeValue( System::OleVariant va );
String __fastcall parsingSpecialforXml( String XML );
String __fastcall parsingSignforXML( String XML );
TDateTime __fastcall YYMMDDStrToDateTime( String Date );
String __fastcall fg_TwoDateToMM( String st_temp1, String st_temp2 );
String __fastcall fg_TwoTimeToMM( String st_temp1, String st_temp2 );
String __fastcall fg_TwoTimeToMM2( String st_temp1, String st_temp2 );
String __fastcall SecToHHnnSS( ULONG sec );
String __fastcall SecTonnSS( ULONG sec );
String __fastcall SecTo00nnSS( ULONG sec );
int __fastcall nnSSToSec( String strNNss );
int __fastcall HHnnSSToSec( String strHHnnSS );
String __fastcall yyyymmddhhnnssToStrDate( String strrYYYYmmddhhnnss, bool CutOfYear = false );
String __fastcall strDateToyyyymmddhhnnss( String sDate );
String __fastcall RemoveSecInDateTimeString( String DateTimeString );

String __fastcall FontToHTMLText( TFont * font, String text );
void __fastcall HTMLTextToFont( String InText, String & OutText, TFont * fn );
String __fastcall HTMLTextToFontColor( String InText );

int __fastcall IntCompareASC( void * a, void * b );
int __fastcall IntCompareDESC( void * a, void * b );
void __fastcall GMSWebBrowserLoadHTML( TGMSWebBrowser * WebBrowser, String HTMLText );
String __fastcall GMHTMLStrip( String HTMLText );
String __fastcall GMSWebBrowserGetInnerText( TGMSWebBrowser * WebBrowser );
void __fastcall WebBrowserLoadHTML( Shdocvw::TWebBrowser * WebBrowser, String HTMLText );
String __fastcall OleVarToStr( System::OleVariant Value );
int __fastcall OleVarToInt( System::OleVariant Value );
__int64 __fastcall OleVarToInt64( System::OleVariant Value );
bool __fastcall OleVarYnToBool( System::OleVariant Value );
bool __fastcall OleVarTrueFalseToBool( System::OleVariant Value );
bool __fastcall YnStrToBool( String Value );
String __fastcall BoolToYnStr( bool Value );
String __fastcall BoolToYStr( bool Value ); // 2014-01-02 mh True 일때만 값 찍어주기
String __fastcall ExtractFileNameWithoutExt( String FileName );
String __fastcall GetBoardXmlSaveFilePath( const String FileName ); // 게시판 내용보기 xml저장 위치

String __fastcall GetFileDownloadURL( String AIniPath );
String __fastcall GetFileUploadURL( String AIniPath );
String __fastcall GetBoardUploadDivCD( String AIniPath );
String __fastcall GetMyNoteUploadDivCD( String AIniPath );
String __fastcall GetArticleUploadDivCD( String AIniPath );
String __fastcall GetTemplateUploadDivCD( String AIniPath );
String __fastcall GetPatchUploadDivCD( String AIniPath );
int __fastcall GetCertificationTimeSec( String AIniPath );
String __fastcall GetChannelCode( String AIniPath, String sKey, String sDefault ); // ini 파일에서 불러오도록 2014-03-18
String __fastcall GetReporterRequireFormatList( String AIniPath, String sKey, String sDefault );

String __fastcall GetFileDefaultDownloadPath( String AIniPath );
void __fastcall SetFileDefaultDownloadPath( String AIniPath, String AValue );

String __fastcall GetFileDefaultUploadPath( String AIniPath );
void __fastcall SetFileDefaultUploadPath( String AIniPath, String AValue );

String __fastcall GetTemplatePreviewURL( String AIniPath );

String __fastcall GetHTMLEditorFileName( String AIniPath );

String __fastcall GetBoardNoticeID( String AIniPath );
String __fastcall GetBoardMonitoringID( String AIniPath );
String __fastcall GetBoardUserNoteID( String AIniPath );

String __fastcall GetRatingBoardID( String AIniPath );
String __fastcall GetRatingReviewID( String AIniPath );
String __fastcall GetRatingMediaNoticeID( String AIniPath );

String __fastcall GetPlPgmNoticeID( String AIniPath );

String __fastcall GetFileSizeString( __int64 AFileSize );
String __fastcall GetTransferSizeString( __int64 ACopySize, __int64 AFileSize );
String __fastcall GetMyDocumentsPath( );
String __fastcall GetLocalAppDataPath( );
String __fastcall IntToCommaStr( int itNumber );

ULONG __fastcall StrTimeToSecCount( String In_strTimeCode );
String __fastcall SecCountToStrTime( ULONG In_ulTimeCode, bool bHideHour = false );

/* 한글 처리 부분 */
bool __fastcall IsHanMode( void );
void __fastcall ToggleHanMode( void );
void __fastcall SetHanMode( void );
void __fastcall SetAlphaNumMode( void );

/* 기사 시간 검색 */
long __fastcall AutoComputeArticleTime( String artcl_ctt, TList * MacroButtons = NULL ); // 2014-04-09 메크로 기능

bool __fastcall TimeValidate(const String TimeStr, TTimeType TimeType);  //시간 정합성 확인
int __fastcall TimeStrToSec(const String TimeStr, TTimeType TimeType);

// 파일 정보
BY_HANDLE_FILE_INFORMATION __fastcall GetLocalFileInfo( String FileName );
String __fastcall FILETIMEtoDateTimeString( FILETIME * ftData );

// SHBrowseForFolder 콜백함수
int __stdcall BrowseCallBackProc( HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData );
Boolean __fastcall SelectFolderDlg( HWND ADlgHandle, String ADlgTitle, String & AFolderName );

// 파일정보 (버전 정보 획득 등에서 확인)
String __fastcall Information( String AFilename, String AQuery );

String __fastcall GetDefaultFontName( String AIniPath );
String __fastcall GetDefaultFontColor( String AIniPath );
String __fastcall GetDefaultFontSize( String AIniPath );

String __fastcall GetExecuteChat( String AIniPath );
String __fastcall GetExecuteUDP( String AIniPath );
String __fastcall GetChannelCode( String AIniPath, String ChannelName /* YTN=001,YTNDMB=002,YTNWEATHER=003,YTNSCIENCE=004,FOREIGN=005 */ );

void __fastcall DefaultGridStyle( TAdvStringGrid * Out_Grid, bool IsRowSelect = false, UINT FontSize = 0 );
void __fastcall DefaultGridStyleRd( TAdvStringGrid * Out_Grid, bool SelEdit ); // 2014-04-25 런다운용 그리드
void __fastcall DefaultColumnGridStyle( TAdvColumnGrid * Out_Grid, bool IsRowSelect = false, UINT FontSize = 0 );

// 상세검색이나 정보 부분에 해당하는 상단 패널 스타일
void __fastcall DefaultTopPanelStyle( TPanel * InnerPanel, TPanel * OuterPanel );

bool __fastcall IsValidLength( const String CheckStr, const int MaxLength );

void __fastcall OnEditKeyPressDigit( TObject * Sender, System::WideChar & Key );
TRect __fastcall GetMDIClientAreaBoundsRect( TForm * AMDIForm );
String __fastcall StripHtmlMarkup( const String ASource );
bool __fastcall CheckPasswordValidate( String Password );
void __fastcall ClearFixedColImage( TAdvStringGrid * InGrid );
void __fastcall ClearFixedColImage( TAdvColumnGrid * InGrid );
void __fastcall ShowPopupMenu( TControl * Control, TPopupMenu * PopupMenu );
int __fastcall GetUtf8Length( String Value );
void __fastcall ClearGridObject( TBaseGrid * InGrid, int ACol );
int __fastcall GetGridNo( int ItemIndex, String rowcount, String curPage );

// 사용자 선택
// 인자 : Owner, UserInfo, 사용자명, 리턴값 사용자 아이디
void __fastcall GetSelectUser( TComponent * Owner, TUserInfo * UserInfo, String & UserName, String & UserId, TEdit * editUserName,
	bool AutoComplete, bool ArticleGroupOnly = false );
// 사용자 선택
// 인자 : Owner, UserInfo, 사용자명, 리턴값 사용자 아이디, 그룹
void __fastcall GetSelectUser( TComponent * Owner, TUserInfo * UserInfo, String & UserName, String & UserId, String & GroupName, String & GroupId,
	TEdit * editUserName, bool AutoComplete, bool ArticleGroupOnly = false );

void __fastcall SetControlFocus( TWinControl * Control );

/* UDP 메시지 채널 적용으로 2014-06-17 */
bool __fastcall GetUDPForChannel( );

void __fastcall HideWebBrowserScrollBar( TGMSWebBrowser * WebBrowser );

String __fastcall SecTimeToStrTimeQuotFormat( int Sec );

void __fastcall ChangeCheckBoxState( TCheckBox * CheckBox, bool Checked );

// 다음에디터 UP/DOWN Key에 의해 포커스 빠지는 현상 처리 이벤트 함수
void __fastcall DaumEditorEnterEvent( TGMSWebBrowser * WebBrowser );

// 웹브라우저에서 스크롤 하는 함수
void __fastcall WebBrowserScroll( TGMSWebBrowser * WebBrowser, long x, long y );

// 외신기사, 내신기사에서 채널별 기사화 체크 함수
String __fastcall CheckMkNewsYn( String mknews_yn, String ch_div_cd );
//GMS에디터 줄간격 설정
void __fastcall SetEditLineSpace(TRichEdit* Editor, int nLineSpace);

//그리드에서 로우가 모두 비였는지 확인하는 함수
bool __fastcall IsEmptyRow(TBaseGrid* Grid, int ARow);
// ****************************************************************/
// 설명 :
// 입력 :
// 결과 :
// 예제:
// 작성자/날짜:   /
// ****************************************************************/

typedef struct _TransferFile
{
	String file_id; // 파일 아이디 업로드 후 결과 받음
	String file_size; // 파일사이즈
	String src_file_path; // 원본파일패스
	String src_file_name; // 원본파일명
	String trg_file_path; // 목적파일패스
	String trg_file_name; // 목적파일명

	String file_status; // 001 : 신규, 002 : 업데이트, 003 : 삭제
	String result; // 작업 결과.  < !-- 001:성공 002: 실패 003: 기타 -->
} TRANSFER_FILE, *pTRANSFER_FILE;

class TTransferFiles : public TList
{
protected:
	TRANSFER_FILE * __fastcall GetItems( int Index );
	void __fastcall SetItems( int Index, TRANSFER_FILE * Item );

public:
	__fastcall TTransferFiles( ) : TList( )
	{ } ;
	__fastcall ~TTransferFiles( );
	void __fastcall Add( TRANSFER_FILE * Item );
	void __fastcall Clear( );
	__property TRANSFER_FILE * Items[ int Index ] =
	{
		read = GetItems,
		write = SetItems
	} ;
} ;

__fastcall TTransferFiles::~TTransferFiles( )
{
	Clear( );
}

TRANSFER_FILE * __fastcall TTransferFiles::GetItems( int Index )
{
	return ( TRANSFER_FILE * )TList::Get( Index );
}

void __fastcall TTransferFiles::SetItems( int Index, TRANSFER_FILE * Item )
{
	TList::Put( Index, Item );
}

void __fastcall TTransferFiles::Add( TRANSFER_FILE * Item )
{
	TList::Add( Item );
}

void __fastcall TTransferFiles::Clear( )
{
	for ( int i = 0; i < this->Count; i++ )
	{
		delete( TRANSFER_FILE * )this->Items[ i ];
	}
	TList::Clear( );
}

void __fastcall ParseDownloadURL( String URL, TRANSFER_FILE * TransferFile );
// ****************************************************************/
// 설명 :    첨부파일 정보
// 입력 :
// 결과 :
// 예제:
// 작성자/날짜:   /
// ****************************************************************/

typedef struct _AttachFile
{
	String file_id; // 파일 id
	String file_nm; // 파일명
	String file_org_nm; // 원본 파일명
	String file_vnm; // 서버에 위치한 파일명
	String file_loc; // 파일위치
	String file_size; // 파일크기
} ATTACH_FILE, *pATTACH_FILE;

class TAttachFiles : public TList
{
protected:
	ATTACH_FILE * __fastcall GetItems( int Index );
	void __fastcall SetItems( int Index, ATTACH_FILE * Item );

public:
	__fastcall TAttachFiles( ) : TList( )
	{ } ;
	__fastcall ~TAttachFiles( );
	void __fastcall Add( ATTACH_FILE * Item );
	void __fastcall Clear( );
	String __fastcall GetRequestXML( );

	__property ATTACH_FILE * Items[ int Index ] =
	{
		read = GetItems,
		write = SetItems
	} ;
} ;

__fastcall TAttachFiles::~TAttachFiles( )
{
	Clear( );
}

ATTACH_FILE * __fastcall TAttachFiles::GetItems( int Index )
{
	return ( ATTACH_FILE * )TList::Get( Index );
}

void __fastcall TAttachFiles::SetItems( int Index, ATTACH_FILE * Item )
{
	TList::Put( Index, Item );
}

void __fastcall TAttachFiles::Add( ATTACH_FILE * Item )
{
	TList::Add( Item );
}

void __fastcall TAttachFiles::Clear( )
{
	for ( int i = 0; i < this->Count; i++ )
	{
		delete( ATTACH_FILE * )this->Items[ i ];
	}
	TList::Clear( );
}

String __fastcall TAttachFiles::GetRequestXML( )
{
	String strRet;
	_di_IXMLDocument xmlDoc = NewXMLDocument( );
	try
	{
		xmlDoc->Options =
			TXMLDocOptions( ) << doNodeAutoCreate << doNodeAutoIndent <<
			doAttrNull << doAutoPrefix << doNamespaceDecl;

		_di_IXMLNode reqNode = xmlDoc->AddChild( "request" );
		_di_IXMLNode dataNode = reqNode->AddChild( "data" );

		if ( this->Count <= 0 )
		{
			_di_IXMLNode recordNode = dataNode->AddChild( "record" );
			_di_IXMLNode recItemNode = recordNode->AddChild( "file_id" );
			recItemNode->SetText( String( ) );
			recItemNode = recordNode->AddChild( "file_nm" );
			recItemNode->SetText( String( ) );
		}
		else
		{
			for ( int i = 0; i < this->Count; i++ )
			{
				_di_IXMLNode recordNode = dataNode->AddChild( "record" );
				_di_IXMLNode recItemNode = recordNode->AddChild( "file_id" );
				recItemNode->SetText( this->Items[ i ]->file_id );
				recItemNode = recordNode->AddChild( "file_nm" );
				recItemNode->SetText( this->Items[ i ]->file_nm );
			}
		}
		strRet = xmlDoc->XML->Text;
	}
	__finally
	{
		SAFE_RELEASE( xmlDoc );
	}

	return strRet;
}
// ****************************************************************/
// 설명 :
// 입력 :
// 결과 :
// 예제:
// 작성자/날짜:   /
// ****************************************************************/

typedef struct _ThumbnailInfo
{
	int DataIndex;
	int ImageIndex;
	bool Assigned;
	bool Processed;
	String ImageURL;
	TJPEGImage * JpegImage;
	TBitmap * BitmapImage;

	_ThumbnailInfo( )
	{
		DataIndex   = -1;
		ImageIndex  = -1;
		Assigned    = false;
		Processed   = false;
		ImageURL    = String( );
		JpegImage   = NULL;
		BitmapImage = NULL;
	}
} THUMB_INFO, *pTHUMB_INFO;

class TThumbnailList : public TList
{
private:
	int __fastcall GetProcessedCount( );

protected:
	THUMB_INFO * __fastcall GetItems( int Index );
	void __fastcall SetItems( int Index, THUMB_INFO * Item );

public:
	__fastcall TThumbnailList( );
	__fastcall ~TThumbnailList( );
	void __fastcall Add( THUMB_INFO * Item );
	void __fastcall Delete( int Index );
	void __fastcall Clear( );
	__property THUMB_INFO * Items[ int Index ] =
	{
		read = GetItems,
		write = SetItems
	} ;
	__property int ProcessedCount =
	{
		read = GetProcessedCount
	} ;
	THUMB_INFO * __fastcall GetAvailableItem( );
} ;

__fastcall TThumbnailList::TThumbnailList( )
	: TList( )
{
}

__fastcall TThumbnailList::~TThumbnailList( )
{
	Clear( );
}

int __fastcall TThumbnailList::GetProcessedCount( )
{
	int nRet = 0;
	for ( int i = 0; i < this->Count; i++ )
	{
		THUMB_INFO * ti = ( THUMB_INFO * )this->Items[ i ];
		if ( ti->Processed )
		{
			nRet++ ;
		}
	}
	return nRet;
}

THUMB_INFO * __fastcall TThumbnailList::GetAvailableItem( )
{
	for ( int i = 0; i < this->Count; i++ )
	{
		THUMB_INFO * ti = ( THUMB_INFO * )this->Items[ i ];
		if ( !ti->Assigned )
		{
			ti->Assigned = true;
			return ti;
		}
	}
	return NULL;
}

THUMB_INFO * __fastcall TThumbnailList::GetItems( int Index )
{
	return ( THUMB_INFO * )TList::Get( Index );
}

void __fastcall TThumbnailList::SetItems( int Index, THUMB_INFO * Item )
{
	TList::Put( Index, Item );
}

void __fastcall TThumbnailList::Add( THUMB_INFO * Item )
{
	TList::Add( Item );
}

void __fastcall TThumbnailList::Delete( int Index )
{
	THUMB_INFO * ti = ( THUMB_INFO * )this->Items[ Index ];
	if ( ti->JpegImage != NULL )
	{
		delete ti->JpegImage;
		ti->JpegImage = NULL;
	}
	if ( ti->BitmapImage != NULL )
	{
		delete ti->BitmapImage;
		ti->BitmapImage = NULL;
	}
	delete ti;
	TList::Delete( Index );
}

void __fastcall TThumbnailList::Clear( )
{
	for ( int i = 0; i < this->Count; i++ )
	{
		THUMB_INFO * ti = ( THUMB_INFO * )this->Items[ i ];
		if ( ti->JpegImage != NULL )
		{
			delete ti->JpegImage;
		}
		if ( ti->BitmapImage != NULL )
		{
			delete ti->BitmapImage;
		}
		delete ti;
	}
	TList::Clear( );
}

class TMgrColumnSort
{
	enum TSortDirection
	{
		sdAsc, sdDesc
	} ;

private:
	int m_Column;
	TSortDirection m_SortDirection;

public:
	__fastcall TMgrColumnSort( );
	void __fastcall InitSortConfig( );
	String __fastcall GetSortDirectionStr( const int ACol );
} ;

__fastcall TMgrColumnSort::TMgrColumnSort( )
{
	InitSortConfig( );
}

void __fastcall TMgrColumnSort::InitSortConfig( )
{
	m_Column        = -1;
	m_SortDirection = sdAsc;
}

String __fastcall TMgrColumnSort::GetSortDirectionStr( const int ACol )
{
	if ( m_Column < 0 )
	{
		// 최초 정렬
		m_Column        = ACol;
		m_SortDirection = sdAsc;
	}
	else if ( m_Column == ACol )
	{
		if ( m_SortDirection == sdAsc )
		{
			m_SortDirection = sdDesc;
		}
		else if ( m_SortDirection == sdDesc )
		{
			m_SortDirection = sdAsc;
		}
	}
	else
	{
		m_SortDirection = sdAsc;
	}

	m_Column = ACol;
	String strSortDirection = m_SortDirection == sdAsc ? L"ASC" : L"DESC";
	return strSortDirection;
}

class TGMDebug
{
public:
	static void __fastcall DebugLog( const String Log );
	static void __fastcall OutputDebugLog( const String Log );
	static void __fastcall DebugXML( const String XML );
} ;

#endif
