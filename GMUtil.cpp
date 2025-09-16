// ---------------------------------------------------------------------------

#pragma hdrstop

#include "GMUTIL.h"
#include <SysUtils.hpp>
#include "Shlobj.h"
//#include "UnitFrmUserList.h"
#include "stdio.h"
//#include "UnitGMXMLDebug.h"

//#include "UnitParentForm.h"
//#include "UnitNoFrameParentForm.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma comment(lib, "Wininet.lib")

/* 사용자 채널 정보 초기화 */
String LogGlobal::ch_div_cd = String( );

__fastcall TGMLogger::TGMLogger( )
{

}

__fastcall TGMLogger::~TGMLogger( )
{

}

void __fastcall TGMLogger::WriteLog( LogType lt, String strMsg )
{
	// TErrorMessageBox::Show( strMsg );
	String LogMsg;
	// LogMsg.sprintf("%d | %s | %s", lt, dateTime, Msg, )

	String strLogPath = L"";
	String strLogMessage = L"";
	String strLogNowHour = Now( ).FormatString( L"hh" );
	// String strLogFilePath = ChangeFileExt( ExtractFileName( Application->ExeName ), L".log" );

	// 채널별 로그 쓰기 추가  2014-06-19 mh
	strLogPath.sprintf( L"%sLog\\%s\\%s\\%s-Ysys-SMain_%s.log",
		ExtractFilePath( Application->ExeName ).w_str( ),
		Now( ).FormatString( "yyyymm" ).w_str( ), Now( ).FormatString( "dd" ).w_str( ),
		LogGlobal::ch_div_cd.c_str( ), strLogNowHour.w_str( ) );

	ForceDirectories( ExtractFilePath( strLogPath ) );

	strLogMessage.sprintf( L"%s || %s \r\n",
		Now( ).FormatString( "hh:nn:ss zzz" ).w_str( ), strMsg.w_str( ) );
	try
	{
		FILE * pfile;
		if ( ( pfile = fopen( AnsiString( strLogPath ).c_str( ), "a+" ) )
			!= NULL )
		{
			fprintf( pfile, AnsiString( strLogMessage ).c_str( ) );
			fclose( pfile );
		}
	}
	catch ( ... )
	{
		//
	}
}

void __fastcall GMSaveToXmlFileRd( String xml, String FileName )
{ // 런다운 저장하기 위해서
	String FullPath = String( ).sprintf( L"%s\\log\\%s\\%s\\%s_%s.xml",
		ExtractFilePath( Application->ExeName ),
		Now( ).FormatString( "yyyymm" ),
		Now( ).FormatString( "dd" ),
		FileName,
		Now( ).FormatString( "hhnnss" ) );

	ForceDirectories( ExtractFilePath( FullPath ) );

	_di_IXMLDocument xmlDoc = NewXMLDocument( );
	try
	{
		xmlDoc->Options = TXMLDocOptions( ) << doNodeAutoCreate << doNodeAutoIndent <<
			doAttrNull << doAutoPrefix << doNamespaceDecl;
		xmlDoc->ParseOptions = TParseOptions( ) << poValidateOnParse;
		xmlDoc->LoadFromXML( xml );
		xmlDoc->SaveToFile( FullPath );
	}
	__finally
	{
		SAFE_RELEASE( xmlDoc );
	}
}

void __fastcall GMSaveToXmlFile( String xml, String FileName, bool use )
{
	if ( !use )
	{
		return;
	}

	// #ifdef _DEBUG
	String FullPath = String( ).sprintf( L"%s\\log\\%s\\%s\\%s_%s.xml",
		ExtractFilePath( Application->ExeName ),
		Now( ).FormatString( "yyyymm" ),
		Now( ).FormatString( "dd" ),
		FileName,
		Now( ).FormatString( "hhnnss" ) );

	ForceDirectories( ExtractFilePath( FullPath ) );

	_di_IXMLDocument xmlDoc = NewXMLDocument( );
	try
	{
		xmlDoc->Options = TXMLDocOptions( ) << doNodeAutoCreate << doNodeAutoIndent <<
			doAttrNull << doAutoPrefix << doNamespaceDecl;
		xmlDoc->ParseOptions = TParseOptions( ) << poValidateOnParse;
		xmlDoc->LoadFromXML( xml );
		xmlDoc->SaveToFile( FullPath );
	}
	__finally
	{
		SAFE_RELEASE( xmlDoc );
	}
	// #endif
}

// ****************************************************************/
// 설명 :    공통 사용 함수
// 입력 :
// 결과 :
// 예제:
// 작성자/날짜: mhSung /
// ****************************************************************/
bool __fastcall GetUDPForChannel( )
{ /* UDP 메시지 채널 적용으로 2014-06-17 */
	bool nRet = 0;

	TIniFile * ini = new TIniFile( ChangeFileExt( Application->ExeName, L".ini" ) );
	try
	{
		nRet = ini->ReadBool( "Sub_Program", "Use_UDP_Channel", true );
	}
	__finally
	{
		delete ini;
	}

	return nRet;
}

String __fastcall replaceNull( String value )
{ // sprintf 함수에 값이 없으면 null 찍히는 것을 공백으로 바꾸기
	return StringReplace( value, L"null", L" ", TReplaceFlags( ) << rfReplaceAll );
}

String __fastcall GetApprvDivCd( int nIndex )
{
	String CodeValue = L"";
	if ( nIndex == 0 )
	{
		CodeValue = L"001"; // 승인
	}
	else if ( nIndex == 1 )
	{
		CodeValue = L"002"; // 미승인
	}
	else if ( nIndex == 2 )
	{
		CodeValue = L"003"; // 가승인
	}
	else if ( nIndex == 3 )
	{
		CodeValue = L"004"; // 예고
	}

	return CodeValue;
}

String __fastcall GetMyDocumentsPath( )
{
	wchar_t szPath[ MAX_PATH ];
	if ( SUCCEEDED( SHGetFolderPath( NULL, CSIDL_COMMON_DOCUMENTS, NULL, 0, szPath ) ) ) // 내문서
	{
		return String( szPath );
	}
	return L"";
}

String __fastcall GetLocalAppDataPath( )
{
	wchar_t szPath[ MAX_PATH ];
	String strPath;
	if ( SUCCEEDED( SHGetFolderPath( NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPath ) ) ) // 내문서
	{
		strPath = String( szPath );
		if ( !strPath.IsEmpty( ) )
		{
			strPath = strPath + L"\\" + ExtractFileName( Application->ExeName );
			if ( !DirectoryExists( strPath ) )
			{
				ForceDirectories( strPath );
			}
		}
	}
	return strPath;
}

void __fastcall ResizeButtonSize( TControl * Button, TCanvas * Canvas )
{
	TButton * btn = ( TButton * )Button;
	btn->Width = Canvas->TextWidth( btn->Caption ) + 10;
}

// ****************************************************************/
// GetNodeValue : 용도 > xml 파싱에서 널값 검사
// ****************************************************************/
TDateTime __fastcall YYMMDDStrToDateTime( String Date )
{ // yy-mm-dd ( 8 )
	// YYYY-mm-dd ( 10 )
	int nYear;
	int nMonth;
	int nDay;

	TDateTime dt = Now( );
	if ( Date.Length( ) == 8 )
	{
		dt = StrToDateTime( Date );
	}
	else if ( Date.Length( ) == 10 )
	{
		dt = StrToDateTime( Date );
	}
	return dt;
}

String __fastcall GetNodeValue( System::OleVariant va )
{
	if ( va.IsNull( ) )
	{
		return L"";
	}

	String tmpValue = va.operator UnicodeString( );
	// TGMDebug::DebugLog(L"치환 시작 \"\\n\" to \"\\r\\n\"");
	String strRet = StringReplace( tmpValue, L"\n", L"\r\n", TReplaceFlags( ) << rfReplaceAll );
	// TGMDebug::DebugLog(L"치환 끝 \"\\n\" to \"\\r\\n\"");
	return strRet;
	// return va.operator UnicodeString( );
}

String __fastcall OleVarToStr( System::OleVariant Value )
{
	if ( Value.IsNull( ) )
	{
		return L"";
	}

	String tmpValue = Value.operator UnicodeString( );
	// TGMDebug::DebugLog(L"치환 시작 \"\\n\" to \"\\r\\n\"");
	String strRet = StringReplace( tmpValue, L"\n", L"\r\n", TReplaceFlags( ) << rfReplaceAll );
	// TGMDebug::DebugLog(L"치환 끝 \"\\n\" to \"\\r\\n\"");
	return strRet;
	// return Value.operator UnicodeString( );
}

int __fastcall OleVarToInt( System::OleVariant Value )
{
	if ( Value.IsNull( ) )
	{
		return 0;
	}
	return Value.operator int( );
}

__int64 __fastcall OleVarToInt64( System::OleVariant Value )
{
	if ( Value.IsNull( ) )
	{
		return 0;
	}

	return Value.operator __int64( );
}

bool __fastcall OleVarYnToBool( System::OleVariant Value )
{
	if ( Value.IsNull( ) )
	{
		return false;
	}
	String strYn = Value.operator UnicodeString( );

	if ( strYn.UpperCase( ) == L"Y" )
	{
		return true;
	}
	return false;
}

bool __fastcall OleVarTrueFalseToBool( System::OleVariant Value )
{
	if ( Value.IsNull( ) )
	{
		return false;
	}
	String strTrueFalse = Value.operator UnicodeString( );

	if ( strTrueFalse.UpperCase( ) == L"TRUE" )
	{
		return true;
	}
	return false;
}

bool __fastcall YnStrToBool( String Value )
{ // 2013-10-31 mh
	try
	{
		if ( Value.Trim( ).IsEmpty( ) )
		{
			return false;
		}

		if ( Value.UpperCase( ) == L"Y" )
		{
			return true;
		}
	}
	catch ( Exception & e )
	{
		return false;
	}
	return false;
}

String __fastcall BoolToYnStr( bool Value )
{
	String strRet = Value ? L"Y" : L"N";
	return strRet;
}

String __fastcall BoolToYStr( bool Value )
{
	String strRet = Value ? L"Y" : L"";
	return strRet;
}

String __fastcall ExtractFileNameWithoutExt( String FileName )
{
	String strRet;
	try
	{
		strRet = ChangeFileExt( ExtractFileName( FileName ), L"" );
	}
	catch ( ... )
	{
		strRet = L"";
	}
	return strRet;
}

String __fastcall fg_TwoDateToMM( String st_temp1, String st_temp2 )
{
	String result;
	// Ttimestamp Ts1, Ts2;
	// int T1 = 0, T2 = 0;
	// Ts1 = DateTimeToTimeStamp( StrToDateTime( st_temp1 ) );
	// Ts2 = DateTimeToTimeStamp( StrToDateTime( st_temp2 ) );
	// T1 = Ts1.Date - Ts2.Date;
	// result = inttostr( T1 );
	// ShowMessage('두 날짜 사이의 일은 ' + IntToStr(T1));
	return result;
}

String __fastcall fg_TwoTimeToMM( String st_temp1, String st_temp2 )
{ // String Type "hh:nn:ss"
	String result;

	try
	{
		TTimeStamp Ts1, Ts2;
		TTime Ttemp;
		Ts1   = DateTimeToTimeStamp( StrToTime( st_temp1 ) );
		Ts2   = DateTimeToTimeStamp( StrToTime( st_temp2 ) );
		Ttemp = Ts1.Time - Ts2.Time;

		result = TimeToStr( Ttemp );
		return result;
	}
	catch ( Exception & e )
	{
		return "";
	}
}

String __fastcall fg_TwoTimeToMM2( String st_temp1, String st_temp2 )
{ // String Type "hh:nn:ss"
	if ( st_temp1 == "" || st_temp2 == "" )
	{
		return "";
	}

	TDateTime stdt = StrToDateTime( st_temp1 );
	TDateTime etdt = StrToDateTime( st_temp2 );

	TDateTime diffTm = etdt - stdt;
	return diffTm.FormatString( L"hh:nn:ss" );
}

String __fastcall SecToHHnnSS( ULONG sec )
{ // 초로 들어오는 값을 hh:nn:ss 형태로 변환
	int nHour, nMin, nSec;

	nHour = int( sec / SEC2HOUR );
	nMin  = int( ( sec - ( SEC2HOUR * nHour ) ) / SEC3MIN );
	nSec  = int( sec - ( ( SEC2HOUR * nHour ) + ( SEC3MIN * nMin ) ) );

	return String( ).sprintf( L"%02d:%02d:%02d", nHour, nMin, nSec );
}

String __fastcall SecTonnSS( ULONG sec )
{ // 초로 들어오는 값을 nn:ss 형태로 변환
	int nMin, nSec;

	nMin = int( sec / SEC3MIN );
	nSec = int( sec % SEC3MIN );

	return String( ).sprintf( L"%02d:%02d", nMin, nSec );
}

//String __fastcall SecToHHnn( ULONG sec )
//{
//
//}

String __fastcall SecTo00nnSS( ULONG sec )
{ // 초로 들어오는 값을 nn:ss 형태로 변환
	int nMin, nSec;

	nMin = int( sec / SEC3MIN );
	nSec = int( sec % SEC3MIN );

	return String( ).sprintf( L"00:%02d:%02d", nMin, nSec );
}

int __fastcall nnSSToSec( String strNNss )
{ // nn:ss
	AnsiString aNNss = strNNss;

	int retss = 0;
	int mm = atoi( aNNss.c_str( ) );
	int ss = atoi( aNNss.c_str( ) + 3 );
	retss = mm * 60 + ss;
	return retss;
}

int __fastcall HHnnSSToSec( String strHHnnSS )
{ // hh:nn:ss
	AnsiString aNNss = strHHnnSS;

	int retss = 0;
	int hh = atoi( aNNss.c_str( ) );
	int mm = atoi( aNNss.c_str( ) + 3 );
	int ss = atoi( aNNss.c_str( ) + 6 );
	retss = hh * 3600 + mm * 60 + ss;
	return retss;
}

String __fastcall strDateToyyyymmddhhnnss( String sDate )
{
	TDateTime dt = Now( ); // defalut;

	int nPos = sDate.Pos( L"-" );

	if ( sDate.Length( ) == 14 && nPos == 0 )
	{ // YYYYmmddhhnnss
		return sDate;
	}
	else
	{
		dt = StrToDateTime( sDate );
	}

	return dt.FormatString( "yyyymmddhhnnss" );
}

String __fastcall yyyymmddhhnnssToStrDate( String strrYYYYmmddhhnnss, bool CutOfYear )
{
	if ( strrYYYYmmddhhnnss.Trim( ).IsEmpty( ) )
	{
		return "";
	}

	String retValue = "";

	String sYear, sMonth, sDay, sHour, sMin, sSec;

	int nPos = strrYYYYmmddhhnnss.Pos( L"-" );

	if ( strrYYYYmmddhhnnss.Length( ) == 14 && nPos == 0 )
	{ // YYYYmmddhhnnss
		// 12345678901234
		if ( CutOfYear )
		{
			sYear = strrYYYYmmddhhnnss.SubString( 3, 2 );
		}
		else
		{
			sYear = strrYYYYmmddhhnnss.SubString( 1, 4 );
		}

		sMonth = strrYYYYmmddhhnnss.SubString( 5, 2 );
		sDay  = strrYYYYmmddhhnnss.SubString( 7, 2 );
		sHour = strrYYYYmmddhhnnss.SubString( 9, 2 );
		sMin  = strrYYYYmmddhhnnss.SubString( 11, 2 );
		sSec  = strrYYYYmmddhhnnss.SubString( 13, 2 );

		retValue.sprintf( L"%s-%s-%s %s:%s", sYear, sMonth, sDay, sHour, sMin );
	}
	else
	{ // yy-mm-dd hh:nn:ss / 17
		if ( nPos > 0 )
		{
			if ( strrYYYYmmddhhnnss.Length( ) == 17 )
			{
				return strrYYYYmmddhhnnss;
			}
			else if ( strrYYYYmmddhhnnss.Length( ) == 14 )
			{
				TDateTime dt = StrToDateTime( strrYYYYmmddhhnnss );
				return dt.FormatString( "yy-mm-dd hh:nn" );
			}
		}
	}

	return retValue;
}

String __fastcall RemoveSecInDateTimeString( String DateTimeString )
{
	// yyyy-mm-dd hh:nn:ss
	if ( DateTimeString.Length( ) != 19 )
	{
		return DateTimeString;
	}

	return DateTimeString.SubString( 1, 16 );
}

// ****************************************************************/
// 설명 :   특수 문자 변환
// 입력 :
// 결과 :
// 예제:
// 작성자/날짜: mhSung /
// ****************************************************************/

String __fastcall parsingSpecialforXml( String XML )
{
	XML = StringReplace( XML, "&", "&amp;", TReplaceFlags( ) << rfReplaceAll );
	XML = StringReplace( XML, "\"", "&quot;", TReplaceFlags( ) << rfReplaceAll );
	XML = StringReplace( XML, "'", "&apos;", TReplaceFlags( ) << rfReplaceAll );
	XML = StringReplace( XML, "<", "&lt;", TReplaceFlags( ) << rfReplaceAll );
	XML = StringReplace( XML, ">", "&gt;", TReplaceFlags( ) << rfReplaceAll );

	return XML;
}

String __fastcall parsingSignforXML( String XML )
{
	XML = StringReplace( XML, "&quot;", "\"", TReplaceFlags( ) << rfReplaceAll );
	XML = StringReplace( XML, "&amp;", "&", TReplaceFlags( ) << rfReplaceAll );
	XML = StringReplace( XML, "&apos;", "'", TReplaceFlags( ) << rfReplaceAll );
	XML = StringReplace( XML, "&lt;", "<", TReplaceFlags( ) << rfReplaceAll );
	XML = StringReplace( XML, "&gt;", ">", TReplaceFlags( ) << rfReplaceAll );

	return XML;
}

/*

 YSYS HTML 정의 – 운영참조 & 런다운
 1.	CS 와 WEB에서 처리하는 HTML 태그 정의 -
 2.	사용 태그 : <B>, <U>, <I>, <FONT>
 3.	SAMPLE
 A.	<FONT FACE="바탕" COLOR="#ff000008">goo&lt;888&gt;god ew we</FONT>
 B.	<I><FONT FACE="바탕체" COLOR="#08">goo&lt;888&gt;god ew we</FONT></I>
 C.	<B><FONT FACE="새굴림" COLOR="#ff000008">goo&lt;888&gt;god ew we</FONT></B>

 1.	모든 태그의 값은 대문자를 기본으로 함
 2.	<FONT_FACE=”폰트이름”_COLOR=”#XXXXXX”>
 1.	FACE 앞의 공백은 1칸, COLOR 앞의 공백은 1칸
 2.	COLOR 값은 #을 포함한 7자리값 RGB 값
 3.	태그의 순서는 상관 없으나, 중간에 처리되는 값들은 처리 불가
 1.	<FONT FACE="새굴림" COLOR="#ff000008">goo&lt<B>888</B>god ew we</FONT>
 2.	값 사이에 태그가 존재할 경우 CS상에서 처리가 불가.

 // Sample html text
 // <FONT FACE="맑은 고딕" COLOR="#ff000008">goo&lt;888&gt;god ew we</FONT>
 // <B><FONT FACE="맑은 고딕" COLOR="#8000">goo&lt;888&gt;god ew we</FONT></B>
 by alsgy   2013-09-24

 */
String __fastcall FontToHTMLText( TFont * font, String text )
{
	String transText = parsingSpecialforXml( text );
	String fc16type = ColorToWebColorStr( font->Color );
	String fcName = font->Name;
	String sFontStyle = String( ).sprintf( L"<FONT FACE=\"%s\" COLOR=\"%s\">%s</FONT>",
		fcName, fc16type, transText );

	/* 스타일 지정 */
	// Font->Style.Contains( fsBold, fsItalic, fsUnderline, fsStrikeOut)

	if ( font->Style.Contains( fsBold ) )
	{
		sFontStyle = String( ).sprintf( L"<B>%s</B>", sFontStyle );
	}
	else if ( font->Style.Contains( fsItalic ) )
	{
		sFontStyle = String( ).sprintf( L"<I>%s</I>", sFontStyle );
	}
	else if ( font->Style.Contains( fsUnderline ) )
	{
		sFontStyle = String( ).sprintf( L"<U>%s</U>", sFontStyle );
	}

	return sFontStyle;
}

String __fastcall HTMLTextToFontColor( String InText )
{
	int    nPos = 0;
	String beforeColor = "";

	nPos = InText.Pos( L"COLOR=\"" );
	if ( nPos > 0 )
	{
		String FontColor = L""; // L"맑은 고딕";

		String Temp = InText.SubString( nPos + 7, InText.Length( ) - nPos );
		int    yPos = Temp.Pos( L"\"" );
		if ( yPos > 0 )
		{
			FontColor = Temp.SubString( 0, yPos - 1 );

			String rColor = FontColor.SubString( 2, 2 );
			String gColor = FontColor.SubString( 4, 2 );
			String bColor = FontColor.SubString( 6, 2 );
			beforeColor = String( ).sprintf( L"$%s%s%s", bColor, gColor, rColor );
		}
	}
	return beforeColor;
}

void __fastcall HTMLTextToFont( String InText, String & OutText, TFont * fn )
{
	int nPos = 0;
	nPos = InText.Pos( L"<B>" );
	if ( nPos > 0 )
	{
		fn->Style.Contains( fsBold );
		InText = StringReplace( InText, L"<B>", L"", TReplaceFlags( ) << rfReplaceAll );
		InText = StringReplace( InText, L"</B>", L"", TReplaceFlags( ) << rfReplaceAll );
	}

	nPos = InText.Pos( L"<I>" );
	if ( nPos > 0 )
	{
		fn->Style.Contains( fsItalic );
		InText = StringReplace( InText, L"<I>", L"", TReplaceFlags( ) << rfReplaceAll );
		InText = StringReplace( InText, L"</I>", L"", TReplaceFlags( ) << rfReplaceAll );
	}

	nPos = InText.Pos( L"<U>" );
	if ( nPos > 0 )
	{
		fn->Style.Contains( fsUnderline );
		InText = StringReplace( InText, L"<U>", L"", TReplaceFlags( ) << rfReplaceAll );
		InText = StringReplace( InText, L"</U>", L"", TReplaceFlags( ) << rfReplaceAll );
	}

	nPos = InText.Pos( L"FACE=\"" );
	if ( nPos > 0 )
	{
		String FontName = L""; // L"맑은 고딕";

		String Temp = InText.SubString( nPos + 6, InText.Length( ) - nPos + 1 );
		int    yPos = Temp.Pos( L"\"" );
		if ( yPos > 0 )
		{
			FontName = Temp.SubString( 0, yPos - 1 );
			fn->Name = FontName;
		}

		String removeTxt = String( ).sprintf( L" FACE=\"%s\"", FontName ); // 공백 필수 제거 대상
		InText = StringReplace( InText, removeTxt, "", TReplaceFlags( ) << rfReplaceAll );
	}

	nPos = InText.Pos( L"COLOR=\"" );
	if ( nPos > 0 )
	{
		String FontColor = L""; // L"맑은 고딕";

		String Temp = InText.SubString( nPos + 7, InText.Length( ) - nPos );
		int    yPos = Temp.Pos( L"\"" );
		if ( yPos > 0 )
		{
			FontColor = Temp.SubString( 0, yPos - 1 );

			try
			{
				TColor cl = WebColorStrToColor( FontColor );
				fn->Size  = 10; // default
				fn->Color = cl;
			}
			catch ( ... )
			{

			}
		}

		String removeTxt = String( ).sprintf( L" COLOR=\"%s\"", FontColor ); // 공백 필수 제거 대상
		InText = StringReplace( InText, removeTxt, "", TReplaceFlags( ) << rfReplaceAll );
	}

	InText = StringReplace( InText, L"<FONT  >", L"", TReplaceFlags( ) << rfReplaceAll );
	InText  = StringReplace( InText, L"<FONT >", L"", TReplaceFlags( ) << rfReplaceAll );
	InText  = StringReplace( InText, L"<FONT>", L"", TReplaceFlags( ) << rfReplaceAll );
	InText  = StringReplace( InText, L"</FONT>", L"", TReplaceFlags( ) << rfReplaceAll );
	OutText = parsingSignforXML( InText );
}

// TList int형 오름차순 정렬 함수
int __fastcall IntCompareASC( void * a, void * b )
{
	int ret = 0;
	if ( ( int )a > ( int )b )
	{
		ret = 1;
	}
	else if ( ( int )a < ( int )b )
	{
		ret = -1;
	}
	else
	{
		ret = 0;
	}
	return ret;
}

// TList int형 내림차순 정렬 함수
int __fastcall IntCompareDESC( void * a, void * b )
{
	int ret = 0;
	if ( ( int )a > ( int )b )
	{
		ret = -1;
	}
	else if ( ( int )a < ( int )b )
	{
		ret = 1;
	}
	else
	{
		ret = 0;
	}
	return ret;
}

// TGMSWebBroser에 HTML을 직접 로딩하는 함수
void __fastcall GMSWebBrowserLoadHTML( TGMSWebBrowser * WebBrowser, String HTMLText )
{
	if ( WebBrowser == NULL )
	{
		return;
	}

	// Registry.SetValue("HKEY_CURRENT_USER\\AppEvents\\Schemes\\Apps\\Explorer\\Navigating\\.Current","","NULL");

	WebBrowser->Silent = true;
	WebBrowser->Navigate( "about:blank" );
	while ( WebBrowser->ReadyState < Shdocvw::READYSTATE_INTERACTIVE )
	{
		Application->ProcessMessages( );
	}

	TStringList * S1;
	TMemoryStream * MS;
	if ( WebBrowser->Document != NULL )
	{
		S1 = new TStringList;
		try
		{
			MS = new TMemoryStream;
			try
			{
				S1->Text = HTMLText;
				S1->SaveToStream( MS, TEncoding::UTF8 );
				MS->Seek( 0, 0 );

				IPersistStreamInit * I;
				IStream * s;
				TStreamAdapter * adpt = new TStreamAdapter( MS, soReference );
				try
				{
					WebBrowser->Document->QueryInterface( IID_IPersistStreamInit, ( void ** ) & I );
					adpt->QueryInterface( IID_IStream, ( void ** ) & s );
					I->Load( s );
				}
				__finally
				{
					s->Release( );
					I->Release( );
					// SAFE_DELETE(adpt);
				}
			}
			__finally
			{
				delete MS;
			}
		}
		__finally
		{
			delete S1;
		}
	}
}

// TGMSWebBroser에서 텍스트만 추출하는 함수
String __fastcall GMSWebBrowserGetInnerText( TGMSWebBrowser * WebBrowser )
{
	BSTR bstrInnerText;
	::IHTMLDocument2 * pHTMLDocument;
	::IHTMLElement * pHTMLElement;
	HRESULT hr;
	hr = WebBrowser->Document->QueryInterface( ::IID_IHTMLDocument2, ( void * * ) &pHTMLDocument );
	if ( hr != S_OK )
	{
		return String( );
	}
	hr = pHTMLDocument->get_body( &pHTMLElement );
	if ( hr != S_OK )
	{
		return String( );
	}
	pHTMLElement->get_innerText( & bstrInnerText );
	pHTMLElement->Release( );
	pHTMLDocument->Release( );
	String strRet = String( bstrInnerText );
	return strRet;
}

String __fastcall GMHTMLStrip( String HTMLText )
{
	if ( Trim( HTMLText ).IsEmpty( ) )
	{
		return String( );
	}
	String strRet;
	TGMSWebBrowser * web = new TGMSWebBrowser( Application );
	try
	{
		web->Left         = -10000;
		web->Width        = 5;
		web->Height       = 5;
		web->ParentWindow = Application->MainForm->Handle;
		GMSWebBrowserLoadHTML( web, HTMLText );
		while ( web->ReadyState != Shdocvw::READYSTATE_COMPLETE )
		{
			Application->ProcessMessages( );
		}

		for ( int i = 0; i < 10; i++ )
		{
			Application->ProcessMessages( );
		}

		DWORD stTick = GetTickCount( );
		while ( strRet.IsEmpty( ) )
		{
			strRet = GMSWebBrowserGetInnerText( web );
			Application->ProcessMessages( );
			if ( GetTickCount( ) - stTick > 500 )
			{
				break;
			}
		}
	}
	__finally
	{
		delete web;
	}
	return strRet;
}

// TWebBroser에 HTML을 직접 로딩하는 함수
void __fastcall WebBrowserLoadHTML( Shdocvw::TWebBrowser * WebBrowser, String HTMLText )
{
	if ( WebBrowser == NULL )
	{
		return;
	}

	// Registry.SetValue("HKEY_CURRENT_USER\\AppEvents\\Schemes\\Apps\\Explorer\\Navigating\\.Current","","NULL");

	WebBrowser->Silent = true;
	WebBrowser->Navigate( "about:blank" );
	while ( WebBrowser->ReadyState < Shdocvw::READYSTATE_INTERACTIVE )
	{
		Application->ProcessMessages( );
	}

	TStringList * S1;
	TMemoryStream * MS;
	if ( WebBrowser->Document != NULL )
	{
		S1 = new TStringList;
		try
		{
			MS = new TMemoryStream;
			try
			{
				S1->Text = HTMLText;
				S1->SaveToStream( MS );
				MS->Seek( 0, 0 );

				IPersistStreamInit * I;
				IStream * s;
				TStreamAdapter * adpt = new TStreamAdapter( MS, soReference );
				try
				{
					WebBrowser->Document->QueryInterface( IID_IPersistStreamInit, ( void ** ) & I );
					adpt->QueryInterface( IID_IStream, ( void ** ) & s );
					I->Load( s );
				}
				__finally
				{
					s->Release( );
					I->Release( );
					// SAFE_DELETE(adpt);
				}
			}
			__finally
			{
				delete MS;
			}
		}
		__finally
		{
			delete S1;
		}
	}
}

String __fastcall GetFileDownloadURL( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"General", L"FileDownloadURL", L"http://192.168.0.100:9200/ysys/download" );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetFileUploadURL( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"General", L"FileUploadURL", L"http://192.168.0.100:9200/ysys/webupload?cmd=upload&dir=" );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetBoardUploadDivCD( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"Board", L"BoardUploadDivCD", L"001" );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetArticleUploadDivCD( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"Article", L"ArticleUploadDivCD", L"002" );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetPatchUploadDivCD( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"Article", L"PatchUploadDivCD", L"008" );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetMyNoteUploadDivCD( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"Board", L"MyNoteUploadDivCD", L"003" );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetTemplateUploadDivCD( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"Template", L"TemplateUploadDivCD", L"005" );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetFileDefaultDownloadPath( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"General", L"FileDefaultDownloadPath", ExtractFilePath( Application->ExeName ) );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

void __fastcall SetFileDefaultDownloadPath( String AIniPath, String AValue )
{
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		ini->WriteString( L"General", L"FileDefaultDownloadPath", AValue );
	}
	__finally
	{
		delete ini;
	}
}

String __fastcall GetFileDefaultUploadPath( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"General", L"FileDefaultUploadPath", ExtractFilePath( Application->ExeName ) );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

void __fastcall SetFileDefaultUploadPath( String AIniPath, String AValue )
{
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		ini->WriteString( L"General", L"FileDefaultUploadPath", AValue );
	}
	__finally
	{
		delete ini;
	}
}

String __fastcall GetTemplatePreviewURL( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"Template", L"PreviewURL", L"http://192.168.0.100:9200/ysys/web/preview.jsp?captmpltid=" );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetHTMLEditorFileName( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"General", L"HTMLEditorFileName", ExtractFilePath( Application->ExeName ) + L"\Board Editor Test\HtmlEditor.html" );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetBoardNoticeID( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"Board", L"BoardNoticeID", L"10" );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetBoardMonitoringID( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"Board", L"BoardMonitoringID", L"13" );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetBoardUserNoteID( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"Board", L"BoardUserNoteID", L"14" );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetRatingBoardID( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"Board", L"RatingBoardID", L"20" );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetRatingReviewID( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"Board", L"RatingReviewID", L"21" );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetRatingMediaNoticeID( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"Board", L"RatingMediaNoticeID", L"22" );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetPlPgmNoticeID( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"Board", L"PlPgmNoticeID", L"30" );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetFileSizeString( __int64 AFileSize )
{
	if ( AFileSize < SIZE_KB )
	{
		return FormatFloat( "#,0 Bytes;-#,0 Bytes;", ( double )AFileSize );
	}
	else if ( AFileSize < SIZE_MB )
	{
		return FormatFloat( "#,0.00 KB;-#,0.00 KB;", ( double )AFileSize / ( double )SIZE_KB );
	}
	else if ( AFileSize < SIZE_GB )
	{
		return FormatFloat( "#,0.00 MB;-#,0.00 MB;", ( double )AFileSize / ( double )SIZE_MB );
	}
	else if ( AFileSize < SIZE_TB )
	{
		return FormatFloat( "#,0.00 GB;-#,0.00 GB;", ( double )AFileSize / ( double )SIZE_GB );
	}
	else
	{
		return FormatFloat( "#,0.00 TB;-#,0.00 TB;", ( double )AFileSize / ( double )SIZE_TB );
	}
}

String __fastcall GetTransferSizeString( __int64 ACopySize, __int64 AFileSize )
{
	if ( AFileSize < SIZE_KB )
	{
		return FormatFloat( "#,0;-#,0;", ( double )ACopySize ) + "/" +
			FormatFloat( "#,0 Bytes;-#,0 Bytes;", ( double )AFileSize );
	}
	else if ( AFileSize < SIZE_MB )
	{
		return FormatFloat( "#,0.00;-#,0.00;", ( double )ACopySize / ( double )SIZE_KB ) + "/" +
			FormatFloat( "#,0.00 KB;-#,0.00 KB;", ( double )AFileSize / ( double )SIZE_KB );
	}
	else if ( AFileSize < SIZE_GB )
	{
		// ShowMessage(FormatFloat("#,0.00;-#,0.00;", (double)ACopySize / (double)SIZE_MB) + "/" +
		// FormatFloat("#,0.00 MB;-#,0.00 MB;", (double)AFileSize / (double)SIZE_MB));
		return FormatFloat( "#,0.00;-#,0.00;", ( double )ACopySize / ( double )SIZE_MB ) + "/" +
			FormatFloat( "#,0.00 MB;-#,0.00 MB;", ( double )AFileSize / ( double )SIZE_MB );
	}
	else if ( AFileSize < SIZE_TB )
	{
		return FormatFloat( "#,0.00;-#,0.00;", ( double )ACopySize / ( double )SIZE_GB ) + "/" +
			FormatFloat( "#,0.00 GB;-#,0.00 GB;", ( double )AFileSize / ( double )SIZE_GB );
	}
	else
	{
		return FormatFloat( "#,0.00;-#,0.00;", ( double )ACopySize / ( double ) SIZE_TB ) + '/' +
			FormatFloat( "#,0.00 TB;-#,0.00 TB;", ( double )AFileSize / ( double )SIZE_TB );
	}
}

// SHBrowseForFolder 콜백함수
int __stdcall BrowseCallBackProc( HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
{
	switch ( uMsg )
	{
		// 폴더 선택 대화 상자 초기화 할 때, 초기 경로 설정
	case BFFM_INITIALIZED:
		{
			SendMessage( hWnd, BFFM_SETSELECTION, TRUE, ( LPARAM )lpData );
		}
		break;

		// 사용자가 폴더를 선택할 경우 대화상자에 선택된 경로 표시
	case BFFM_SELCHANGED:
		{
			TCHAR szText[ MAX_PATH ] =
			{
				0
			} ;
			SHGetPathFromIDList( ( LPCITEMIDLIST )lParam, szText );
			SendMessage( hWnd, BFFM_SETSTATUSTEXT, 0, ( LPARAM )szText );
		}
		break;
	}

	return 0; // Always return 0.
}

Boolean __fastcall SelectFolderDlg( HWND DlgHandle, String DlgTitle, String & FolderName )
{
	TCHAR DisplayName[ MAX_PATH ] =
	{
		0,
	} ;

	TBrowseInfo * BrowseInfo = new TBrowseInfo( );
	try
	{
		memset( BrowseInfo, 0x00, sizeof( TBrowseInfo ) );
		BrowseInfo->hwndOwner      = DlgHandle;
		BrowseInfo->pidlRoot       = NULL;
		BrowseInfo->pszDisplayName = DisplayName;
		BrowseInfo->lpfn           = BrowseCallBackProc;
		BrowseInfo->lParam         = ( LPARAM )FolderName.c_str( );
		BrowseInfo->lpszTitle      = DlgTitle.c_str( );
		BrowseInfo->ulFlags        = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN;
		PItemIDList PIDL = SHBrowseForFolder( BrowseInfo );
		if ( ( PIDL != NULL ) && ( SHGetPathFromIDList( PIDL, DisplayName ) ) )
		{
			FolderName = String( DisplayName );
			return true;
		}
		else
		{
			return false;
		}
	}
	__finally
	{
		SAFE_DELETE( BrowseInfo );
	}
	return false;
}

// ****************************************************************/
// 설명 :    한글 입력 여부 확인
// 입력 :
// 결과 :
// 예제:
// 작성자/날짜: hkkim /
// ****************************************************************/

bool __fastcall IsHanMode( void )
{
	unsigned long Conversion, Sentence;
	bool bRet = false;
	HIMC hImc = ImmGetContext( Application->Handle );
	try
	{
		ImmGetConversionStatus( hImc, & Conversion, & Sentence );
		bRet = ( Conversion == IME_CMODE_HANGEUL || Conversion == IME_CMODE_NATIVE );
	}
	__finally
	{
		ImmReleaseContext( Application->Handle, hImc );
	}
	return bRet;
}

void __fastcall ToggleHanMode( void )
{
	unsigned long Conversion, Sentence;
	HIMC hImc = ImmGetContext( Application->Handle );
	try
	{
		ImmGetConversionStatus( hImc, & Conversion, & Sentence );
		if ( Conversion == IME_CMODE_HANGEUL || Conversion == IME_CMODE_NATIVE )
		{
			ImmSetConversionStatus( hImc, IME_CMODE_ALPHANUMERIC, Sentence );
		}
		else
		{
			ImmSetConversionStatus( hImc, IME_CMODE_NATIVE, Sentence );
		}
	}
	__finally
	{
		ImmReleaseContext( Application->Handle, hImc );
	}
}

void __fastcall SetHanMode( void )
{
	unsigned long Conversion, Sentence;
	HIMC hImc = ImmGetContext( Application->Handle );
	try
	{
		ImmGetConversionStatus( hImc, & Conversion, & Sentence );
		ImmSetConversionStatus( hImc, IME_CMODE_NATIVE, Sentence );
	}
	__finally
	{
		ImmReleaseContext( Application->Handle, hImc );
	}
}

void __fastcall SetAlphaNumMode( void )
{
	unsigned long Conversion, Sentence;
	HIMC hImc = ImmGetContext( Application->Handle );
	try
	{
		ImmGetConversionStatus( hImc, & Conversion, & Sentence );
		ImmSetConversionStatus( hImc, IME_CMODE_ALPHANUMERIC, Sentence );
	}
	__finally
	{
		ImmReleaseContext( Application->Handle, hImc );
	}
}

// ****************************************************************/
// 설명 :   파일 정보 알아오기
// 입력 :
// 결과 :   BY_HANDLE_FILE_INFORMATION 구조체에서 정보를 찾아서 쓰면 됨
// 예제:
// 작성자/날짜: mhSung /   2013-11-13
// ****************************************************************/

BY_HANDLE_FILE_INFORMATION __fastcall GetLocalFileInfo( String FileName )
{ // LARGE_INTEGER UlSize;
	BY_HANDLE_FILE_INFORMATION FileInfo;

	HANDLE hFile = ::CreateFileW( FileName.c_str( ), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if ( hFile != INVALID_HANDLE_VALUE )
	{ // ::GetFileSizeEx( hFile, & UlSize );
		::GetFileInformationByHandle( hFile, & FileInfo );
		::CloseHandle( hFile );
	}
	return ( BY_HANDLE_FILE_INFORMATION ) FileInfo;
}

// ****************************************************************/
// 설명 :    파일타임을 DateTime ( YYYY-MM-DD ) 형식으로 변환
// 입력 :
// 결과 :
// 예제:
// 작성자/날짜: mhSung / 2013-11-13
// ****************************************************************/

String __fastcall FILETIMEtoDateTimeString( FILETIME * ftData )
{
	FILETIME ft;
	SYSTEMTIME st;
	TDateTime dt;

	FileTimeToLocalFileTime( ftData, & ft );
	FileTimeToSystemTime( & ft, & st );
	dt = SystemTimeToDateTime( st );

	return dt.FormatString( L"yyyy-mm-dd" );
}

// ****************************************************************/
// 설명 :  폰트 정보도 ini 파일에서 불러오기
// 입력 :
// 결과 :
// 예제:
// 작성자/날짜: mhSung /
// ****************************************************************/

String __fastcall GetDefaultFontName( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"Default_Set", L"fontface", L"맑은 고딕" );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetDefaultFontColor( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"Default_Set", L"fontcolor", "#000000" );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetDefaultFontSize( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"Default_Set", L"fontsize", "11" );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

int __fastcall GetCertificationTimeSec( String AIniPath )
{ // 로그인 인증 대기 시간 SOAP 시간과 동기화 필수
	int Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadInteger( L"Login_Info", L"CertificationTimeSec", 60 );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetExecuteChat( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"Sub_Program", L"Chat", "Ysys-Talk" );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetExecuteUDP( String AIniPath )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"Sub_Program", L"UDP", "Ysys-UDP" );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetChannelCode( String AIniPath, String sKey, String sDefault /* YTN=001,YTNDMB=002,YTNWEATHER=003,YTNSCIENCE=004,FOREIGN=005 */ )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"CHANNEL", sKey, sDefault );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

String __fastcall GetReporterRequireFormatList( String AIniPath, String sKey, String sDefault )
{
	String Ret;
	TIniFile * ini = new TIniFile( AIniPath );
	try
	{
		Ret = ini->ReadString( L"Article", sKey, sDefault );
	}
	__finally
	{
		delete ini;
	}
	return Ret;
}

// ****************************************************************/
// 설명 :    그리드 상태 초기화
// 입력 :
// 결과 :
// 예제:
// 작성자/날짜: mhSung /
// ****************************************************************/
/*
void __fastcall DefaultGridStyleRd( TAdvStringGrid * Out_Grid, bool SelEdit )
{ // 런다운용 특화 2014-04-25
	Out_Grid->Look = glXP;

	Out_Grid->Navigation->AllowCtrlEnter = false;

	Out_Grid->ActiveRowShow = true;

	Out_Grid->ActiveRowColor = DF_COLOR_UNLOCK; // clblack

	Out_Grid->MouseActions->DisjunctRowSelect = true;

	Out_Grid->FixedFont->Name  = L"맑은 고딕";
	Out_Grid->FixedFont->Size  = 12;
	Out_Grid->FixedFont->Color = clWhite;
	Out_Grid->FixedRowHeight   = IDS_DEFAULT_GRID_ROW_HEIGHT;
	Out_Grid->FixedFont->Style = TFontStyles( ) << fsBold;

	Out_Grid->Flat = true; // 2014-05-01

	Out_Grid->Font->Size  = 12;
	Out_Grid->Font->Name  = L"맑은 고딕";
	Out_Grid->Font->Color = StringToColor( L"$001D1D1D" );

	Out_Grid->Options = Out_Grid->Options << goColSizing << goVertLine << goFixedVertLine;
	if ( SelEdit )
	{
		Out_Grid->Options = Out_Grid->Options << goEditing << goRangeSelect; // >> goRowSelect;
	}
	else
	{
		Out_Grid->Options = Out_Grid->Options >> goEditing << goRangeSelect; // >> goRowSelect;
	}
	Out_Grid->DefaultRowHeight = IDS_DEFAULT_GRID_ROW_HEIGHT;
	Out_Grid->MouseActions->WheelAction = waScroll;

	Out_Grid->BorderColor = clSilver;

	Out_Grid->Bands->Active = false;
	// Out_Grid->Bands->PrimaryColor            = StringToColor( L"$00F3F3F3" );
	Out_Grid->ControlLook->FixedGradientFrom = StringToColor( L"$009D6200" );
	Out_Grid->ControlLook->FixedGradientTo   = StringToColor( L"$009D6200" );
	Out_Grid->GridFixedLineColor             = clSilver;

	Out_Grid->ShowSelection = true;
	// Out_Grid->SelectionColor     = DF_COLOR_UNLOCK;
	Out_Grid->SelectionTextColor = clWhite;

	Out_Grid->IntelliZoom                    = false;
	Out_Grid->WordWrap                       = false;
	Out_Grid->ScrollSynch                    = true;
	Out_Grid->Navigation->LeftRightRowSelect = false;

	// Default Row
	Out_Grid->RowCount = 2;

	Out_Grid->EditWithTags = true;
	Out_Grid->EnableHTML   = false;

	TBaseFrm_Parent * frmParent = dynamic_cast < TBaseFrm_Parent * > ( Out_Grid->Owner );
	if ( frmParent != NULL &&
		Out_Grid->OnMouseDown == NULL &&
		Out_Grid->OnMouseMove == NULL &&
		!Out_Grid->Options.Contains( goRowMoving ) )
	{
		Out_Grid->OnMouseDown = frmParent->DefaultGridMouseDown;
		Out_Grid->OnMouseMove = frmParent->DefaultGridMouseMove;
	}
}

void __fastcall DefaultGridStyle( TAdvStringGrid * Out_Grid, bool IsRowSelect, UINT FontSize )
{
	// Out_Grid->FixedFont->Name  = L"맑은 고딕";
	// Out_Grid->FixedFont->Size  = 12;
	// Out_Grid->FixedFont->Style = TFontStyles( );
	// Out_Grid->FixedRowHeight   = IDS_DEFAULT_GRID_ROW_HEIGHT;
	// Out_Grid->FixedFont->Style = Out_Grid->FixedFont->Style << fsBold;
	//
	// Out_Grid->Font->Size = 12;
	// Out_Grid->Font->Name = L"맑은 고딕";
	//
	// Out_Grid->Options          = Out_Grid->Options << goColSizing;
	// Out_Grid->DefaultRowHeight = IDS_DEFAULT_GRID_ROW_HEIGHT;
	// Out_Grid->MouseActions->WheelAction = waScroll;
	//
	// // Default Row
	// Out_Grid->RowCount = 2;
	if ( FontSize == 0 )
	{
		FontSize = 11;
	}
	Out_Grid->Look = glXP;
	if ( !IsRowSelect )
	{
		Out_Grid->ActiveRowShow  = true;
		Out_Grid->ActiveRowColor = clSilver;
	}
	Out_Grid->Flat = true;

	Out_Grid->FixedFont->Name  = L"맑은 고딕";
	Out_Grid->FixedFont->Size  = FontSize;
	Out_Grid->FixedFont->Color = clWhite;
	Out_Grid->FixedRowHeight   = IDS_DEFAULT_GRID_ROW_HEIGHT;
	Out_Grid->FixedFont->Style = TFontStyles( ) << fsBold;
	Out_Grid->EnableHTML       = false;

	Out_Grid->Font->Size  = FontSize;
	Out_Grid->Font->Name  = L"맑은 고딕";
	Out_Grid->Font->Color = StringToColor( L"$001D1D1D" );

	if ( Out_Grid->Options.Contains( goEditing ) )
	{
		Out_Grid->Options = Out_Grid->Options << goColSizing << goVertLine << goFixedVertLine;
		Out_Grid->Options = Out_Grid->Options >> goRangeSelect >> goRowSelect;
	}
	else
	{
		Out_Grid->Options = Out_Grid->Options << goColSizing << goVertLine << goFixedVertLine;
		if ( IsRowSelect )
		{
			Out_Grid->Navigation->LeftRightRowSelect = false;
			Out_Grid->Options                        = Out_Grid->Options << goRowSelect;
		}
		Out_Grid->Options = Out_Grid->Options >> goRangeSelect;
	}

	Out_Grid->DefaultRowHeight = IDS_DEFAULT_GRID_ROW_HEIGHT;
	Out_Grid->MouseActions->WheelAction = waScroll;

	Out_Grid->BorderColor = clSilver;

	// Out_Grid->Bands->Active                  = true;
	// Out_Grid->Bands->PrimaryColor            = StringToColor( L"$00F3F3F3" );
	Out_Grid->ControlLook->FixedGradientFrom = StringToColor( L"$009D6200" );
	Out_Grid->ControlLook->FixedGradientTo   = StringToColor( L"$009D6200" );
	Out_Grid->GridFixedLineColor             = clSilver;

	Out_Grid->ShowSelection      = true;
	Out_Grid->SelectionColor     = StringToColor( L"$00633E00" );
	Out_Grid->SelectionTextColor = clWhite;

	Out_Grid->IntelliZoom                    = false;
	Out_Grid->WordWrap                       = false;
	Out_Grid->ScrollSynch                    = true;
	Out_Grid->Navigation->LeftRightRowSelect = false;

	// Default Row
	Out_Grid->AutoHideSelection = false; // 옵션 기능 추기 mh
	Out_Grid->RowCount          = 2;

	TBaseFrm_Parent *        frmParent = dynamic_cast < TBaseFrm_Parent * > ( Out_Grid->Owner );
	TBaseFrm_NoframeParent * frmParentNoframe = dynamic_cast < TBaseFrm_NoframeParent * > ( Out_Grid->Owner );
	if ( ( frmParent != NULL || frmParentNoframe != NULL ) &&
		Out_Grid->OnMouseMove == NULL &&
		Out_Grid->OnMouseDown == NULL &&
		!Out_Grid->Options.Contains( goRowMoving ) )
	{
		Out_Grid->OnMouseDown = frmParent->DefaultGridMouseDown;
		Out_Grid->OnMouseMove = frmParent->DefaultGridMouseMove;
	}
}

void __fastcall DefaultColumnGridStyle( TAdvColumnGrid * Out_Grid, bool IsRowSelect, UINT FontSize )
{
	// int i;
	//
	// Out_Grid->FixedFont->Name  = L"맑은 고딕";
	// Out_Grid->FixedFont->Size  = 12;
	// Out_Grid->FixedFont->Style = TFontStyles( );
	// Out_Grid->FixedRowHeight   = IDS_DEFAULT_GRID_ROW_HEIGHT;
	// Out_Grid->FixedFont->Style = Out_Grid->FixedFont->Style << fsBold;
	//
	// for (i = 0; i < Out_Grid->ColCount; i++) {
	// Out_Grid->Columns->Items[i]->Font->Size = 12;
	// Out_Grid->Columns->Items[i]->Font->Name = L"맑은 고딕";
	//
	// Out_Grid->Columns->Items[i]->HeaderFont->Size = 12;
	// Out_Grid->Columns->Items[i]->HeaderFont->Name = L"맑은 고딕";
	// Out_Grid->Columns->Items[i]->HeaderFont->Style = Out_Grid->Columns->Items[i]->HeaderFont->Style << fsBold;
	// }
	//
	// Out_Grid->Options          = Out_Grid->Options << goColSizing;
	// Out_Grid->DefaultRowHeight = IDS_DEFAULT_GRID_ROW_HEIGHT;
	// Out_Grid->MouseActions->WheelAction = waScroll;
	//
	// // Default Row
	// Out_Grid->RowCount = 2;
	if ( FontSize == 0 )
	{
		FontSize = 11;
	}
	Out_Grid->Look = glXP;
	if ( !IsRowSelect )
	{
		Out_Grid->ActiveRowShow  = true;
		Out_Grid->ActiveRowColor = clSilver;
	}
	Out_Grid->Flat = true;

	Out_Grid->FixedFont->Name  = L"맑은 고딕";
	Out_Grid->FixedFont->Size  = FontSize;
	Out_Grid->FixedFont->Color = clWhite;
	Out_Grid->FixedRowHeight   = IDS_DEFAULT_GRID_ROW_HEIGHT;
	Out_Grid->FixedFont->Style = TFontStyles( ) << fsBold;
	Out_Grid->EnableHTML       = false;

	Out_Grid->Font->Size  = FontSize;
	Out_Grid->Font->Name  = L"맑은 고딕";
	Out_Grid->Font->Color = StringToColor( L"$001D1D1D" );

	for ( int i = 0; i < Out_Grid->ColCount; i++ )
	{
		Out_Grid->Columns->Items[ i ]->Font->Size  = FontSize;
		Out_Grid->Columns->Items[ i ]->Font->Name  = L"맑은 고딕";
		Out_Grid->Columns->Items[ i ]->Font->Color = StringToColor( L"$001D1D1D" );

		Out_Grid->Columns->Items[ i ]->HeaderFont->Size  = FontSize;
		Out_Grid->Columns->Items[ i ]->HeaderFont->Name  = L"맑은 고딕";
		Out_Grid->Columns->Items[ i ]->HeaderFont->Color = clWhite;
		Out_Grid->Columns->Items[ i ]->HeaderFont->Style = Out_Grid->Columns->Items[ i ]->HeaderFont->Style << fsBold;
	}

	if ( Out_Grid->Options.Contains( goEditing ) )
	{
		Out_Grid->Options = Out_Grid->Options << goColSizing << goVertLine << goFixedVertLine;
		if ( IsRowSelect )
		{
			Out_Grid->Navigation->LeftRightRowSelect = false;
			Out_Grid->Options                        = Out_Grid->Options << goRowSelect;
		}
		Out_Grid->Options = Out_Grid->Options >> goRangeSelect;
	}
	else
	{
		Out_Grid->Options = Out_Grid->Options << goColSizing << goVertLine << goFixedVertLine;
		Out_Grid->Options = Out_Grid->Options >> goRangeSelect;
	}

	Out_Grid->DefaultRowHeight = IDS_DEFAULT_GRID_ROW_HEIGHT;
	Out_Grid->MouseActions->WheelAction = waScroll;

	// Out_Grid->BorderColor                    = clSilver;
	// Out_Grid->Bands->Active                  = true;
	// Out_Grid->Bands->PrimaryColor            = StringToColor( L"$00F3F3F3" );
	Out_Grid->ControlLook->FixedGradientFrom = StringToColor( L"$009D6200" );
	Out_Grid->ControlLook->FixedGradientTo   = StringToColor( L"$009D6200" );
	Out_Grid->GridFixedLineColor             = StringToColor( L"$00DADADA" );
	Out_Grid->GridLineColor                  = StringToColor( L"$00DADADA" );

	Out_Grid->ShowSelection      = true;
	Out_Grid->SelectionColor     = StringToColor( L"$00633E00" );
	Out_Grid->SelectionTextColor = clWhite;

	Out_Grid->IntelliZoom                    = false;
	Out_Grid->WordWrap                       = false;
	Out_Grid->ScrollSynch                    = true;
	Out_Grid->Navigation->LeftRightRowSelect = false;

	// Default Row
	Out_Grid->RowCount = 2;
	TBaseFrm_Parent *        frmParent = dynamic_cast < TBaseFrm_Parent * > ( Out_Grid->Owner );
	TBaseFrm_NoframeParent * frmParentNoframe = dynamic_cast < TBaseFrm_NoframeParent * > ( Out_Grid->Owner );
	if ( ( frmParent != NULL || frmParentNoframe != NULL ) &&
		Out_Grid->OnMouseMove == NULL &&
		Out_Grid->OnMouseDown == NULL &&
		!Out_Grid->Options.Contains( goRowMoving ) )
	{
		Out_Grid->OnMouseDown = frmParent->DefaultGridMouseDown;
		Out_Grid->OnMouseMove = frmParent->DefaultGridMouseMove;
	}
}

*/
// ****************************************************************/
// 설명 :  프로그램 정보 알아오기
// 입력 :
// 결과 :
// 예제:
// 작성자/날짜: hkkim/
// ****************************************************************/

String __fastcall Information( String AFilename, String AQuery )
{
	/*
	 ※ AQuery List
	 - CompanyName
	 - FileDescription
	 - FileVersion
	 - InternalName
	 - LegalCopyright
	 - LegalTrademarks
	 - ProductName
	 - ProductVersion
	 */
	if ( AQuery.IsEmpty( ) )
	{
		return String( );
	}

	DWORD dwHandle;
	LPVOID ptr;
	UINT uLength;

	DWORD dwVersionInfoSize = GetFileVersionInfoSize( AFilename.c_str( ), &dwHandle );
	if ( dwVersionInfoSize == 0 )
	{
		return String( );
	}

	LPVOID pFileInfo = ( LPVOID )HeapAlloc( GetProcessHeap( ), HEAP_ZERO_MEMORY, dwVersionInfoSize );

	GetFileVersionInfo( AFilename.c_str( ), dwHandle, dwVersionInfoSize, pFileInfo );
	VerQueryValue( pFileInfo, L"\\VarFileInfo\\Translation", & ptr, & uLength );

	WORD * id = ( WORD * )ptr;
	String szString = L"\\StringFileInfo\\" + IntToHex( id[ 0 ], 4 ) + IntToHex( id[ 1 ], 4 ) + L"\\" + AQuery;

	VerQueryValue( pFileInfo, szString.c_str( ), & ptr, & uLength );
	if ( uLength == 0 )
	{
		return String( );
	}

	String sOut = String( ( wchar_t * ) ptr );
	HeapFree( GetProcessHeap( ), 0, pFileInfo );

	return sOut;
}

// ****************************************************************/
// 설명 :  사용자 정보 알아오기
// 입력 :
// 결과 :
// 예제:
// 작성자/날짜: hkkim/
// ****************************************************************/
/*
void __fastcall GetSelectUser( TComponent * Owner, TUserInfo * UserInfo, String & UserName, String & UserId,
	TEdit * editUserName, bool AutoComplete, bool ArticleGroupOnly )
{
	String GroupName, GroupId;
	GetSelectUser( Owner, UserInfo, UserName, UserId, GroupName, GroupId, editUserName, AutoComplete, ArticleGroupOnly );
}

void __fastcall GetSelectUser( TComponent * Owner, TUserInfo * UserInfo, String & UserName, String & UserId,
	String & GroupName, String & GroupId, TEdit * editUserName, bool AutoComplete, bool ArticleGroupOnly )
{
	UserName = Trim( UserName );
	TfrmUserListDlg * frmUserListDlg = new TfrmUserListDlg( Owner, UserInfo, UserName, UserId );
	try
	{
		if ( ArticleGroupOnly )
		{
			frmUserListDlg->GetSelectArticleUserSOAP( );
		}
		else
		{
			frmUserListDlg->GetSelectUserSOAP( );
		}
		USERLISTITEM * UserItem = frmUserListDlg->GetCorrectUserItem( );
		if ( UserItem != NULL && AutoComplete == true )
		{
			UserId    = UserItem->user_id;
			UserName  = UserItem->user_nm;
			GroupName = UserItem->group_nm;
			GroupId   = UserItem->group_id;
		}
		else
		{
			if ( frmUserListDlg->ShowModal( ) == mrOk )
			{
				UserId    = frmUserListDlg->m_SelectedItem->user_id;
				UserName  = frmUserListDlg->m_SelectedItem->user_nm;
				GroupName = frmUserListDlg->m_SelectedItem->group_nm;
				GroupId   = frmUserListDlg->m_SelectedItem->group_id;
			}
			else // if ( UserId.IsEmpty( ) )
			{
				UserId    = String( );
				UserName  = String( );
				GroupName = String( );
				GroupId   = String( );
			}
		}

		if ( editUserName != NULL )
		{
			editUserName->Text = UserName;
		}
	}
	__finally
	{
		SAFE_DELETE( frmUserListDlg );
	}
}
*/
bool __fastcall IsValidLength( const String CheckStr, const int MaxLength )
{
	AnsiString CheckStrAnsi( CheckStr );
	return ( CheckStrAnsi.Length( ) <= MaxLength );
}

String __fastcall IntToCommaStr( int itNumber )
{
	bool blMinus = false;
	String stString, stPayString = L"";

	if ( itNumber < 0 )
	{
		itNumber = abs( itNumber );
		blMinus  = true;
	}

	stString = IntToStr( itNumber );

	do
	{
		if ( !stPayString.IsEmpty( ) )
		{
			stPayString = L"," + stPayString;
		}
		stPayString = stString.SubString( stString.Length( ) - 2 < 1 ? 1 : stString.Length( ) - 2, 3 ) + stPayString;

		stString = stString.SubString( 1, stString.Length( ) - 3 );
	}
	while ( !stString.IsEmpty( ) );

	if ( blMinus )
	{
		stPayString = L"-" + stPayString;
	}
	return ( stPayString );

}

ULONG __fastcall StrTimeToSecCount( String In_strTimeCode )
{ // hh:nn:ss 를 초로 변환 ( hh:nn:ss:zzz 일때는 zzz 는 생략)  jbkim
	if ( In_strTimeCode.IsEmpty( ) || In_strTimeCode.Length( ) < 8 )
	{
		return 0;
	}
	ULONG ulOutFrameCount = ( _ttoi( In_strTimeCode.c_str( ) ) * 3600 ) +
		( _ttoi( In_strTimeCode.c_str( ) + 3 ) * 60 ) +
		( _ttoi( In_strTimeCode.c_str( ) + 6 ) );

	return ulOutFrameCount;
}

String __fastcall SecCountToStrTime( ULONG In_ulTimeCode, bool bHideHour )
{ // jbkim
	int hh = 0, mm = 0, ss = 0;
	String strRet;
	hh = In_ulTimeCode / 3600;
	mm = ( In_ulTimeCode - ( hh * 3600 ) ) / 60;
	ss = ( In_ulTimeCode - ( ( hh * 3600 ) + ( mm * 60 ) ) );

	if ( bHideHour )
	{
		strRet = String( ).sprintf( L"%02d:%02d", mm, ss );
	}
	else
	{
		strRet = String( ).sprintf( L"%02d:%02d:%02d", hh, mm, ss );
	}
	return strRet;
}

void __fastcall OnEditKeyPressDigit( TObject * Sender, System::WideChar & Key )
{
	if ( Key == VK_BACK )
	{
		return;
	}

	if ( !IsDigit( Key ) )
	{
		Key = 0;
	}
}

TRect __fastcall GetMDIClientAreaBoundsRect( TForm * AMDIForm )
{
	System::Types::TRect R;

	if ( AMDIForm->FormStyle == fsMDIForm )
	{
		if ( !( ::GetClientRect( AMDIForm->ClientHandle, &R ) ) )
		{
			RaiseLastOSError( );
		}
	}
	else
	{
		TErrorMessageBox::Show( L"MDIForm is not an MDI form" );
	}
	return R;
}

String __fastcall StripHtmlMarkup( const String ASource )
{
	String Ret;

	bool InTag = false;
	int  Count = 0;
	for ( int i = 1; i <= ASource.Length( ); i++ )
	{
		if ( InTag )
		{
			if ( ASource[ i ] == '>' )
			{
				InTag = false;
			}
		}
		else
		{
			if ( ASource[ i ] == '<' )
			{
				InTag = true;
			}
			else
			{
				Ret = Ret + ASource[ i ];
				Count++ ;
			}
		}
	}
	return Ret;
}

bool __fastcall CheckPasswordValidate( String Password )
{
	Password = Trim( Password );
	if ( Password.Length( ) < 6 )
	{
		TErrorMessageBox::Show( L"비밀번호 길이가 너무 짧습니다.", mstWarnning );
		return false;
	}

	const String NotAllowChar[ 4 ] =
	{
		L"\"",
		L"'",
		L",",
		L"\\"
	} ;
	for ( int i = 0; i < 4; i++ )
	{
		if ( Password.Pos( NotAllowChar[ i ] ) > 0 )
		{
			TErrorMessageBox::Show( L"비밀번호에 허용되지 않은 특수문자가 포함되어 있습니다.", mstWarnning );
			return false;
		}
	}

	int AlphaCount = 0;
	int NumberCount = 0;
	for ( int i = 1; i <= Password.Length( ); i++ )
	{
		if ( IsDigit( Password[ i ] ) )
		{
			NumberCount++ ;
			continue;
		}
		AlphaCount++ ;
	}
	if ( ( AlphaCount == 0 ) || ( NumberCount == 0 ) )
	{
		TErrorMessageBox::Show( L"비밀번호에 영문과 숫자가 조합되지 않았습니다.", mstWarnning );
		return false;
	}

	return true;
}
/*
void __fastcall TGMDebug::DebugLog( const String Log )
{
#ifdef _DEBUG

	if ( frmXMLDebug->HandleAllocated( ) )
	{
		String strLog = String( ).sprintf( L"[%s] %s\n", Now( ).FormatString( L"yyyy-mm-dd hh:nn:ss.zzz" ).c_str( ), Log.c_str( ) );
		frmXMLDebug->mmoLog->Lines->Add( strLog );

		if ( frmXMLDebug->chkDontShow->Checked )
		{
			return;
		}
		frmXMLDebug->Show( );
	}
#endif
}

void __fastcall TGMDebug::OutputDebugLog( const String Log )
{
#ifdef _DEBUG
	OutputDebugStringW( String( ).sprintf( L"[%s] %s\n", Now( ).FormatString( L"yyyy-mm-dd hh:nn:ss.zzz" ).c_str( ), Log.c_str( ) ).c_str( ) );
#endif
}

void __fastcall TGMDebug::DebugXML( const String XML )
{
#ifdef _DEBUG
	if ( frmXMLDebug->HandleAllocated( ) )
	{
		frmXMLDebug->mmoRawText->Text = Trim( XML );
		frmXMLDebug->ViewXMLLog( );

		if ( frmXMLDebug->chkDontShow->Checked )
		{
			return;
		}

		frmXMLDebug->Show( );
	}
#endif
}
*/

void __fastcall ClearFixedColImage( TAdvStringGrid * InGrid )
{
	for ( int i = 0; i < InGrid->ColCount; i++ )
	{
		InGrid->RemoveImageIdx( i, 0 );
	}
}

void __fastcall ClearFixedColImage( TAdvColumnGrid * InGrid )
{
	for ( int i = 0; i < InGrid->ColCount; i++ )
	{
		if ( Trim( InGrid->Cells[ i ][ 0 ] ).IsEmpty( ) )
		{
			continue;
		}
		InGrid->RemoveImageIdx( i, 0 );
		InGrid->FontColors[ i ][ 0 ] = clWhite;
	}
}

// 컨트롤에 팝업메뉴를 띄운다
void __fastcall ShowPopupMenu( TControl * Control, TPopupMenu * PopupMenu )
{
	TPoint pt( Control->Left, Control->Top + Control->Height );
	pt = Control->Parent->ClientToScreen( pt );
	PopupMenu->Popup( pt.x, pt.y );
}

int __fastcall GetUtf8Length( String Value )
{
	RawByteString rbs = AnsiToUtf8( Value );
	return rbs.Length( );
}

void __fastcall ParseDownloadURL( String URL, TRANSFER_FILE * TransferFile )
{
	::URL_COMPONENTS URLComp;
	memset( & URLComp, 0, sizeof( ::URL_COMPONENTS ) );

	URLComp.dwHostNameLength = INTERNET_MAX_HOST_NAME_LENGTH;
	URLComp.lpszHostName     = new wchar_t[ INTERNET_MAX_HOST_NAME_LENGTH + 1 ];
	URLComp.dwUrlPathLength  = INTERNET_MAX_PATH_LENGTH;
	URLComp.lpszUrlPath      = new wchar_t[ INTERNET_MAX_PATH_LENGTH + 1 ];

	URLComp.dwStructSize = sizeof( ::URL_COMPONENTS );

	try
	{
		if ( InternetCrackUrl( URL.c_str( ), URL.Length( ), 0, &URLComp ) )
		{
			// OutputDebugString(URLComp.lpszHostName);
			// OutputDebugString(URLComp.lpszUrlPath);
			// OutputDebugString(IntToStr(URLComp.nPort).c_str());
			String FilePath, FileName;
			TStringList * slURL = new TStringList( );
			try
			{
				slURL->Delimiter     = L'/';
				slURL->DelimitedText = String( URLComp.lpszUrlPath );
				FileName             = slURL->Strings[ slURL->Count - 1 ];
				slURL->Delete( slURL->Count - 1 );
				FilePath = slURL->DelimitedText;
				if ( FilePath[ 1 ] == L'/' )
				{
					FilePath.Delete( 1, 1 );
				}
			}
			__finally
			{
				delete slURL;
			}
			TransferFile->src_file_path = String( ).sprintf( L"http://%s:%u/%s",
				URLComp.lpszHostName,
				URLComp.nPort,
				FilePath.c_str( ) );
			TransferFile->src_file_name = FileName;
		}
	}
	__finally
	{
		delete[ ]URLComp.lpszHostName;
		delete[ ]URLComp.lpszUrlPath;
	}
}

long __fastcall AutoComputeArticleTime( String artcl_ctt, TList * MacroButtons )
{
	artcl_ctt = Trim( artcl_ctt );
	if ( artcl_ctt.IsEmpty( ) )
	{
		return 0;
	}

	// ##처리
	TStringList * sl = new TStringList( );
	try
	{
		sl->Text = artcl_ctt;
		for ( int i = sl->Count - 1; i >= 0; i-- )
		{
			if ( sl->Strings[ i ].Length( ) == 0 )
			{
				continue;
			}
			if ( sl->Strings[ i ].Pos( L"##" ) > 0 )
			{
				sl->Delete( i );
			}
			// 매크로 처리
			else if ( MacroButtons != NULL )
			{
				MACRO_BUTTON_INFO * mbi = NULL;
				String MacroPrefix;

				for ( int j = 0; j < MacroButtons->Count; j++ )
				{
					mbi         = ( MACRO_BUTTON_INFO * )MacroButtons->Items[ j ];
					MacroPrefix = mbi->Macro.SubString( 1, mbi->Macro.Length( ) - 1 );
					if ( sl->Strings[ i ].Length( ) == 0 )
					{
						continue;
					}
					if ( sl->Strings[ i ].Pos( MacroPrefix ) > 0 )
					{
						sl->Delete( i );
						break;
					}
				}
			}
		}
		artcl_ctt = sl->Text;
	}
	__finally
	{
		delete sl;
	}

	int charCount = 0;
	int artcl_ctt_len = artcl_ctt.Length( );
	for ( int i = 1; i <= artcl_ctt_len; i++ )
	{
		if ( artcl_ctt[ i ] >= 0 && artcl_ctt[ i ] < 128 )
		{
			if ( ( artcl_ctt[ i ] < 48 ) ||
				( artcl_ctt[ i ] > 57 && artcl_ctt[ i ] < 64 ) ||
				( artcl_ctt[ i ] > 90 && artcl_ctt[ i ] < 97 ) ||
				( artcl_ctt[ i ] > 122 ) )
			{
				continue;
			}
		}
		++charCount;
	}

	double ArticleTime = charCount * WORD_PER_SEC;
	return ( long )ArticleTime;
}

// ---------------------------------------------------------------------------
// 상세검색이나 정보 부분에 해당하는 상단 패널 스타일
void __fastcall DefaultTopPanelStyle( TPanel * InnerPanel, TPanel * OuterPanel )
{
	InnerPanel->ParentBackground = false;
	InnerPanel->BevelOuter       = bvNone;
	OuterPanel->BevelOuter       = bvNone;
	InnerPanel->StyleElements    = TStyleElements( );
	OuterPanel->StyleElements    = TStyleElements( );
	InnerPanel->Color            = TColor( RGB( 234, 238, 232 ) );
	OuterPanel->Color            = TColor( RGB( 52, 52, 52 ) );
	OuterPanel->Padding->Left    = 7;
	OuterPanel->BorderWidth      = 1;
}

void __fastcall ClearGridObject( TBaseGrid * InGrid, int ACol )
{
	if ( ACol < 0 || ACol > InGrid->ColCount - 1 )
	{
		return;
	}

	for ( int i = 0; i < InGrid->RowCount; i++ )
	{
		InGrid->Objects[ ACol ][ i ] = NULL;
	}
}

void __fastcall SetControlFocus( TWinControl * Control )
{
	TForm * OwnerForm = ( TForm * )Control->Owner;

	if ( OwnerForm->Showing && Control->CanFocus( ) )
	{
		Control->SetFocus( );
	}
}

int __fastcall GetGridNo( int ItemIndex, String rowcount, String curPage )
{
	int nRet = ItemIndex + ( StrToIntDef( rowcount, 1 ) * ( StrToIntDef( curPage, 1 ) - 1 ) );
	return nRet;
}

void __fastcall HideWebBrowserScrollBar( TGMSWebBrowser * WebBrowser )
{
	IDispatch * pIDisp = WebBrowser->Document;
	if ( pIDisp != NULL )
	{
		IHTMLDocument2 * pHTMLDocument2;
		HRESULT hr;
		hr = pIDisp->QueryInterface( IID_IHTMLDocument2, ( void * * ) &pHTMLDocument2 );
		if ( hr == S_OK )
		{
			IHTMLElement * pIElement;
			hr = pHTMLDocument2->get_body( &pIElement );

			if ( pIElement == NULL )
			{
				return;
			}

			IHTMLBodyElement * pIBodyElement;
			hr = pIElement->QueryInterface( IID_IHTMLBodyElement, ( void * * ) &pIBodyElement );
			if ( pIBodyElement != NULL )
			{
				pIBodyElement->put_scroll( L"no" );
			}
		}
		pIDisp->Release( );
	}
}

// 프레임을 String TimeCode로 변경해 준다.
// 포맷 분'초"
String __fastcall SecTimeToStrTimeQuotFormat( int Sec )
{
	int    hh = 0;
	int    mm = 0;
	int    ss = 0;
	String strTimeValue = L"";

	hh = ( Sec ) / 3600.0;
	mm = ( Sec % 3600 ) / 60.0;
	ss = ( Sec % 3600 ) % 60;

	// 시간과 분이 똑같으니 시간이 있으면  전체로 표시하고 시간이 없으면 분,초만 표시하자
	if ( hh > 0 )
	{
		strTimeValue.sprintf( L"%02d'%02d'%02d\"", hh, mm, ss );
	}
	else
	{
		strTimeValue.sprintf( L"%02d'%02d\"", mm, ss );
	}
	// if(hh > 0 && mm > 0 && ss > 0)
	// strTimeValue.sprintf(L"%d'%d'%d\"", hh, mm, ss);
	// else if(mm > 0 && ss > 0)
	// strTimeValue.sprintf(L"%d'%d\"", mm, ss);
	// else if(mm > 0 && ss == 0)
	// strTimeValue.sprintf(L"%d'", mm);
	// else
	// strTimeValue.sprintf(L"%d\"", ss);

	return strTimeValue;
}

void __fastcall ChangeCheckBoxState( TCheckBox * CheckBox, bool Checked )
{
	TNotifyEvent OnClick = CheckBox->OnClick;
	CheckBox->OnClick = NULL;
	CheckBox->Checked = Checked;
	CheckBox->OnClick = OnClick;
}

String __fastcall GetBoardXmlSaveFilePath( const String FileName )
{
	String WebEditorDir = ExtractFileDir( GetHTMLEditorFileName( ChangeFileExt( Application->ExeName, L".ini" ) ) );
	String FilePath = ExtractFilePath( Application->ExeName ) + WebEditorDir + L"\\" + FileName + L".xml";
	return FilePath;
}

void __fastcall DaumEditorEnterEvent( TGMSWebBrowser * WebBrowser )
{
	IHTMLDocument2 * Doc; // current HTML document
	IHTMLWindow2 * HTMLWindow; // parent window of current HTML document
	String JSFn; // stores JavaScipt function call
	OleVariant ScriptResult;

	if ( GetAsyncKeyState( VK_UP ) & 0x8000 ||
		GetAsyncKeyState( VK_DOWN ) & 0x8000 )
	{
		// Get reference to current document
		WebBrowser->Document->QueryInterface( IID_IHTMLDocument2, ( void ** ) & Doc );
		if ( Doc == NULL )
		{
			return;
		}

		// Get parent window of current document
		Doc->get_parentWindow( ( IHTMLWindow2 ** ) & HTMLWindow );
		if ( HTMLWindow == NULL )
		{
			return;
		}

		// Run JavaScript
		try
		{
			// JSFn := Format('foo("%s",%d)', [S, I]);  // build function call
			JSFn = L"setFocus()"; // build function call
			HTMLWindow->execScript( JSFn.c_str( ), L"JavaScript", ScriptResult ); // execute function
		}
		catch ( Exception & E )
		{
			// handle exception in case JavaScript fails to run
		}
	}
}

void __fastcall WebBrowserScroll( TGMSWebBrowser * WebBrowser, long x, long y )
{
	// WebBrowser1.OleObject.Document.ParentWindow.Scroll By(0, +100000);
	IHTMLDocument2 * Doc; // current HTML document
	IHTMLWindow2 * HTMLWindow; // parent window of current HTML document
	String JSFn; // stores JavaScipt function call
	OleVariant ScriptResult;
	// Get reference to current document
	WebBrowser->Document->QueryInterface( IID_IHTMLDocument2, ( void ** ) & Doc );
	if ( Doc == NULL )
	{
		return;
	}

	// Get parent window of current document
	Doc->get_parentWindow( ( IHTMLWindow2 ** ) & HTMLWindow );
	if ( HTMLWindow == NULL )
	{
		return;
	}
	HTMLWindow->scrollBy( x, y );
}

String __fastcall CheckMkNewsYn( String mknews_yn, String ch_div_cd )
{
	// mknews_yn
	// 값은 1111 형태로 4자리로 들어오는데 각 자리는 채널을 뜻하며
	// 1은 참 0은 거짓 이다
	// 1(YTN)1(DMB)1(weather)1(science)순서
	if ( mknews_yn.IsEmpty( ) || mknews_yn.Length( ) != 4 )
	{
		return L"N";
	}
	String ytn_ch_div_cd = GetChannelCode( ChangeFileExt( Application->ExeName, L".ini" ), L"YTN", L"001" );
	String dmb_ch_div_cd = GetChannelCode( ChangeFileExt( Application->ExeName, L".ini" ), L"YTNDMB", L"002" );
	String weather_ch_div_cd = GetChannelCode( ChangeFileExt( Application->ExeName, L".ini" ), L"YTNWEATHER", L"003" );
	String science_sch_div_cd = GetChannelCode( ChangeFileExt( Application->ExeName, L".ini" ), L"YTNSCIENCE", L"004" );

	int nPos = 0;
	if ( ch_div_cd == ytn_ch_div_cd )
	{
		nPos = 1;
	}
	else if ( ch_div_cd == dmb_ch_div_cd )
	{
		nPos = 2;
	}
	else if ( ch_div_cd == weather_ch_div_cd )
	{
		nPos = 3;
	}
	else if ( ch_div_cd == science_sch_div_cd )
	{
		nPos = 4;
	}

	if ( mknews_yn[ nPos ] == L'1' )
	{
		return L"Y";
	}

	return L"N";
}

void __fastcall SetEditLineSpace( TRichEdit * Editor, int nLineSpace )
{
	PARAFORMAT2 pf2 =
	{
		0,
	} ;
	pf2.cbSize           = sizeof( PARAFORMAT2 );
	pf2.dwMask           = PFM_LINESPACING;
	pf2.bLineSpacingRule = 5;
	/*
	 0 : Single spacing. The dyLineSpacing member is ignored.
	 1 :	One-and-a-half spacing. The dyLineSpacing member is ignored.
	 2 :	Double spacing. The dyLineSpacing member is ignored.
	 3 : The dyLineSpacing member specifies the spacingfrom one line to the next, in twips. However, if dyLineSpacing specifies a value that is less than single spacing, the control displays single-spaced text.
	 4 :	The dyLineSpacing member specifies the spacing from one line to the next, in twips. The control uses the exact spacing specified, even if dyLineSpacing specifies a value that is less than single spacing.
	 5 :	The value of dyLineSpacing / 20 is the spacing, in lines, from one line to the next. Thus, setting dyLineSpacing to 20 produces single-spaced text, 40 is double spaced, 60 is triple spaced, and so on.
	 */
	pf2.dyLineSpacing = nLineSpace;
	Editor->SelectAll( );
	SendMessage( Editor->Handle, EM_SETPARAFORMAT, SCF_SELECTION, ( long ) & pf2 );
	Editor->SelLength = 0;
}

bool __fastcall IsEmptyRow( TBaseGrid * Grid, int ARow )
{
	for ( int i = 0; i < Grid->ColCount; i++ )
	{
		if ( !Grid->Cells[ i ][ ARow ].IsEmpty( ) )
		{
			return false;
		}
	}
	return true;
}

// ---------------------------------------------------------------------------
bool __fastcall TimeValidate( const String TimeStr, TTimeType TimeType )
{
	TStringList * sl = new TStringList( );
	try
	{
		sl->StrictDelimiter = true;
		sl->Delimiter       = L':';
		sl->DelimitedText   = TimeStr;
		if ( sl->Count != 2 )
		{
			return false;
		}
		String strHH, strMM, strSS;
		if ( TimeType == ttHHMM )
		{
			strHH = sl->Strings[ 0 ];
			strMM = sl->Strings[ 1 ];
			int HH, MM, SS;
			HH = StrToIntDef( strHH, -1 );
			if ( HH > 24 || strHH < 0 )
			{
				return false;
			}
			MM = StrToIntDef( strMM, -1 );
			if ( MM > 59 || strMM < 0 )
			{
				return false;
			}
		}
		else
		{
			strMM = sl->Strings[ 0 ];
			strSS = sl->Strings[ 1 ];
			int MM, SS;
			MM = StrToIntDef( strMM, -1 );
			if ( MM > 59 || strMM < 0 )
			{
				return false;
			}
			SS = StrToIntDef( strSS, -1 );
			if ( SS > 59 || strMM < 0 )
			{
				return false;
			}
		}

	}
	__finally
	{
		delete sl;
	}
	return true;
}

int __fastcall TimeStrToSec( const String TimeStr, TTimeType TimeType )
{
	int           nRet = 0;
	TStringList * sl = new TStringList( );
	try
	{
		sl->StrictDelimiter = true;
		sl->Delimiter       = L':';
		sl->DelimitedText   = TimeStr;
		if ( sl->Count != 2 )
		{
			return 0;
		}
		String strHH, strMM, strSS;
		if ( TimeType == ttHHMM )
		{
			strHH = sl->Strings[ 0 ];
			strMM = sl->Strings[ 1 ];
			int HH, MM, SS;
			HH = StrToIntDef( strHH, -1 );
			if ( HH > 24 || strHH < 0 )
			{
				return 0;
			}
			MM = StrToIntDef( strMM, -1 );
			if ( MM > 59 || strMM < 0 )
			{
				return 0;
			}

			nRet = ( HH * 60 * 60 ) + ( MM * 60 );
		}
		else
		{
			strMM = sl->Strings[ 0 ];
			strSS = sl->Strings[ 1 ];
			int MM, SS;
			MM = StrToIntDef( strMM, -1 );
			if ( MM > 59 || strMM < 0 )
			{
				return 0;
			}
			SS = StrToIntDef( strSS, -1 );
			if ( SS > 59 || strMM < 0 )
			{
				return 0;
			}

			nRet = ( MM * 60 ) + SS;
		}

	}
	__finally
	{
		delete sl;
	}
	return nRet;
}
