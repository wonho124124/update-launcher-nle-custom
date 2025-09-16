// ---------------------------------------------------------------------------

#ifndef UtilsBCBH
#define UtilsBCBH

// ---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <StrUtils.hpp>
#include <IniFiles.hpp>
#include <Stdio.h>
#include <tchar.h>
#include <winsock.h>
#include <DateUtils.hpp>

// ---------------------------------------------------------------------------

#define MAX_SIZE    256
#define KB_SIZE     (ULONGLONG)1024
#define MB_SIZE     (ULONGLONG)1024576
#define GB_SIZE     (ULONGLONG)1073741824
#define TB_SIZE     (ULONGLONG)1099511627776

// ---------------------------------------------------------------------------

class TUtils
{
private:
	char * __fastcall Dec2Bin( unsigned long value ); // 진수 변환시 사용되는 내부 함수
	BOOL __fastcall LastWriteTimeCheck(String CheckFileName, BOOL CheckAtt, int CheckDay); // 지난 파일 및 폴더 삭제 내부 함수

protected:

public:
	void __fastcall LogMessage(
		String Message,
		int    Line ); // 로그 함수
	String __fastcall GetPhysicalAddress( void ); // 물리 주소를 구하는 함수
	String __fastcall GetIPAddress( void ); // 일반 IP 주소를 구하는 함수
	String __fastcall GetComName( ); // 내 컴퓨터의 이름을 구하는 함수
	ULONGLONG __fastcall GetFileSize( String FilePath ); // 파일의 용량을 바이트로 구하는 함수
	String __fastcall GetFileSizeStr( String FilePath ); // 파일의 용량을 문자열로 구하는 함수
	String __fastcall CommandExecute( String strFullCommandLine );
	// 컨맨드라인을 실행하여 실행문을 리턴하는 함수
	TDateTime __fastcall GetCreationTime( String FilePath ); // 파일의 생성날짜를 구하는 함수
	String __fastcall VersionInfo( String FileName ); // 파일의 버전 정보를 구하는 함수
	String __fastcall ToBitPrtSecond( unsigned long rate );
	// 초당 전송 비트율의 문자열로 리턴하는 함수
	String __fastcall GetExeNamePath( ); // 현재 실행파일의 경로를 구하는 함수
	void __fastcall FindSubFiles( String strRootPath ); // 지정 폴더의 하위 경로를 구하는 함수
	void __fastcall Delay( int nTime ); // 지정 초만큼 대기하는 함수.
	ULONGLONG __fastcall StrTimeToFrameCount( String In_strTimeCode );
	// 타임코드를 숫자로 변경
	String __fastcall FrameCountToStrTime( ULONG In_ulTimeCode );
	// 숫자를 타임코드로 변경
	String __fastcall BaseConverter(
		int    rsc,
		String data,
		int    obj ); // 진수 변환. 2,8,10,16
	String __fastcall GetDecodedXmlString(String Value); // Safe문자를 특수문자로 변경
	String __fastcall GetSafeXmlString(String Value);  // 특수문자를 Safe문자로 변경
	String __fastcall FixedLen(String str, int size); // 23 -> 00000023 지정한 숫자보다 작을경우 0으로 채우기
	void __fastcall OldFileDelete(String sBasePath, int CheckDay); // BasePath 내 CheckDay 가 지난 파일 및 폴더 삭제
	ULONG __fastcall TCToSec(String strTC);
	LONG __fastcall StrTimeToSecCount( String strTimeCode );
	String __fastcall SecCountToStrTime( LONG lTimeCode );
	String __fastcall SecCountToStrMinSec( LONG lTimeCode );
	String __fastcall SecToTC(ULONG ulSec);
	void __fastcall SetControlFocus( TWinControl * Control );
	String __fastcall GetSafeFilePath( String Value );
} ;

#endif
