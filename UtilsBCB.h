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
	char * __fastcall Dec2Bin( unsigned long value ); // ���� ��ȯ�� ���Ǵ� ���� �Լ�
	BOOL __fastcall LastWriteTimeCheck(String CheckFileName, BOOL CheckAtt, int CheckDay); // ���� ���� �� ���� ���� ���� �Լ�

protected:

public:
	void __fastcall LogMessage(
		String Message,
		int    Line ); // �α� �Լ�
	String __fastcall GetPhysicalAddress( void ); // ���� �ּҸ� ���ϴ� �Լ�
	String __fastcall GetIPAddress( void ); // �Ϲ� IP �ּҸ� ���ϴ� �Լ�
	String __fastcall GetComName( ); // �� ��ǻ���� �̸��� ���ϴ� �Լ�
	ULONGLONG __fastcall GetFileSize( String FilePath ); // ������ �뷮�� ����Ʈ�� ���ϴ� �Լ�
	String __fastcall GetFileSizeStr( String FilePath ); // ������ �뷮�� ���ڿ��� ���ϴ� �Լ�
	String __fastcall CommandExecute( String strFullCommandLine );
	// ���ǵ������ �����Ͽ� ���๮�� �����ϴ� �Լ�
	TDateTime __fastcall GetCreationTime( String FilePath ); // ������ ������¥�� ���ϴ� �Լ�
	String __fastcall VersionInfo( String FileName ); // ������ ���� ������ ���ϴ� �Լ�
	String __fastcall ToBitPrtSecond( unsigned long rate );
	// �ʴ� ���� ��Ʈ���� ���ڿ��� �����ϴ� �Լ�
	String __fastcall GetExeNamePath( ); // ���� ���������� ��θ� ���ϴ� �Լ�
	void __fastcall FindSubFiles( String strRootPath ); // ���� ������ ���� ��θ� ���ϴ� �Լ�
	void __fastcall Delay( int nTime ); // ���� �ʸ�ŭ ����ϴ� �Լ�.
	ULONGLONG __fastcall StrTimeToFrameCount( String In_strTimeCode );
	// Ÿ���ڵ带 ���ڷ� ����
	String __fastcall FrameCountToStrTime( ULONG In_ulTimeCode );
	// ���ڸ� Ÿ���ڵ�� ����
	String __fastcall BaseConverter(
		int    rsc,
		String data,
		int    obj ); // ���� ��ȯ. 2,8,10,16
	String __fastcall GetDecodedXmlString(String Value); // Safe���ڸ� Ư�����ڷ� ����
	String __fastcall GetSafeXmlString(String Value);  // Ư�����ڸ� Safe���ڷ� ����
	String __fastcall FixedLen(String str, int size); // 23 -> 00000023 ������ ���ں��� ������� 0���� ä���
	void __fastcall OldFileDelete(String sBasePath, int CheckDay); // BasePath �� CheckDay �� ���� ���� �� ���� ����
	ULONG __fastcall TCToSec(String strTC);
	LONG __fastcall StrTimeToSecCount( String strTimeCode );
	String __fastcall SecCountToStrTime( LONG lTimeCode );
	String __fastcall SecCountToStrMinSec( LONG lTimeCode );
	String __fastcall SecToTC(ULONG ulSec);
	void __fastcall SetControlFocus( TWinControl * Control );
	String __fastcall GetSafeFilePath( String Value );
} ;

#endif
