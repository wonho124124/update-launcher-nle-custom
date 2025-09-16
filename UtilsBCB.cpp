// ---------------------------------------------------------------------------

#pragma hdrstop

#include "UtilsBCB.h"

// ---------------------------------------------------------------------------

#pragma package(smart_init)

// ---------------------------------------------------------------------------

void __fastcall TUtils::FindSubFiles( String strRootPath )
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	SetCurrentDirectoryW( strRootPath.c_str( ) );
	hFind = FindFirstFileW( L"*.*", &FindFileData );

	if ( hFind != INVALID_HANDLE_VALUE )
	{
		do
		{
			if ( String( FindFileData.cFileName ) != L"." && String
				( FindFileData.cFileName ) != L".." )
			{
				String strFileName = IncludeTrailingPathDelimiter( strRootPath )
					+ String( FindFileData.cFileName );
				if ( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					// FindSubFiles(strFileName);
				}
				else if ( FindFileData.dwFileAttributes &
					FILE_ATTRIBUTE_HIDDEN )
				{
					// SKIP
				}
				else
				{
					// m_ullTotalSize = m_ullTotalSize + getFileSize(strFileName);
					// String strSubFileName = AnsiReplaceStr(strFileName, EditSource->Text, "");
					// TransferAddList(strFileName, EditTarget->Text + strSubFileName);
				}
			}

			Application->ProcessMessages( );

		}
		while ( FindNextFile( hFind, &FindFileData ) );

		FindClose( hFind );
	}

}

void __fastcall TUtils::LogMessage(
	String Message,
	int    Line )
{
	String strLogPath = L"";
	String strLogMessage = L"";
	String strLogFilePath =
		ChangeFileExt( ExtractFileName( Application->ExeName ), L".log" );

	strLogPath.sprintf( L"%s\\Logs\\%s\\%s",
		LOG_BASE_PATH,
		Now( ).FormatString( L"yyyymmdd" ).c_str( ),
		strLogFilePath.c_str( ) );

	ForceDirectories( ExtractFilePath( strLogPath ) );

	strLogMessage.sprintf( L"[%s] %s (line:%d)\r\n",
		Now( ).DateTimeString( ).c_str( ), Message.c_str( ), Line );

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
	}
}

// ---------------------------------------------------------------------------

String __fastcall TUtils::GetPhysicalAddress( void )
{
	String strPhysicalAddress;
	unsigned char MACData[ 6 ];

	UUID uuid;
	UuidCreateSequential( & uuid ); // Ask OS to create UUID

	for ( int i = 2; i < 8; i++ ) // Bytes 2 through 7 inclusive are MAC address
	{
		MACData[ i - 2 ] = uuid.Data4[ i ];
	}
	strPhysicalAddress.printf( L"%02X-%02X-%02X-%02X-%02X-%02X", MACData[ 0 ],
		MACData[ 1 ], MACData[ 2 ], MACData[ 3 ], MACData[ 4 ], MACData[ 5 ] );

	return strPhysicalAddress;
}

// ---------------------------------------------------------------------------

String __fastcall TUtils::GetIPAddress( void )
{
	WSAData wsaData;
	if ( WSAStartup( MAKEWORD( 1, 1 ), &wsaData ) != 0 )
	{
		return L"";
	}

	char slocal[ 256 ];
	if ( gethostname( slocal, 256 ) == SOCKET_ERROR )
	{
		return L"";
	}

	hostent * hos = gethostbyname( slocal );
	if ( hos == NULL )
	{
		return L"";
	}

	char * addr = *( hos->h_addr_list );
	String LocalIP = String( ( unsigned char )addr[ 0 ] ) + '.' +
		String( ( unsigned char )addr[ 1 ] ) + '.' +
		String( ( unsigned char )addr[ 2 ] ) + '.' +
		String( ( unsigned char )addr[ 3 ] );
	WSACleanup( );
	return LocalIP;
}

// ---------------------------------------------------------------------------

ULONGLONG __fastcall TUtils::GetFileSize( String FilePath )
{
	LARGE_INTEGER ulSize;
	HANDLE hFile = ::CreateFileW( FilePath.c_str( ), GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if ( hFile == INVALID_HANDLE_VALUE )
	{
		return 0;
	}
	else
	{
		::GetFileSizeEx( hFile, & ulSize );
		::CloseHandle( hFile );
	}

	return ( ULONGLONG )ulSize.QuadPart;
}

// ---------------------------------------------------------------------------

String __fastcall TUtils::GetFileSizeStr( String FilePath )
{
	ULONGLONG ullFileSize = GetFileSize( FilePath );
	String    strFileSize = L"";

	if ( ullFileSize > TB_SIZE )
	{
		strFileSize.sprintf( L"%0.2f TB", ( double )ullFileSize / TB_SIZE );
	}
	else if ( ullFileSize > GB_SIZE )
	{
		strFileSize.sprintf( L"%0.2f GB", ( double )ullFileSize / GB_SIZE );
	}
	else if ( ullFileSize > MB_SIZE )
	{
		strFileSize.sprintf( L"%0.2f MB", ( double )ullFileSize / MB_SIZE );
	}
	else if ( ullFileSize > KB_SIZE )
	{
		strFileSize.sprintf( L"%0.2f KB", ( double )ullFileSize / KB_SIZE );
	}
	else
	{
		strFileSize.sprintf( L"%0.2f Byte", ullFileSize );
	}

	return strFileSize;
}

// ---------------------------------------------------------------------------

String __fastcall TUtils::CommandExecute( String strFullCommandLine )
{
	HANDLE hwrite, hread;
	SECURITY_ATTRIBUTES sa;
	sa.nLength              = sizeof( SECURITY_ATTRIBUTES );
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle       = true;

	if ( !CreatePipe( &hread, &hwrite, &sa, 0 ) )
	{
		return L"";
	}

	STARTUPINFOW si;
	memset( & si, 0, sizeof( STARTUPINFO ) );
	si.cb         = sizeof( STARTUPINFO );
	si.dwFlags    = STARTF_USESTDHANDLES;
	si.hStdOutput = hwrite;
	si.hStdError  = hwrite;
	PROCESS_INFORMATION pi;

	if ( !CreateProcessW( NULL, strFullCommandLine.c_str( ), NULL, NULL, true,
		DETACHED_PROCESS, NULL, NULL, &si, &pi ) )
	{
		return L"";
	}
	if ( hwrite )
	{
		CloseHandle( hwrite );
	}

	char buffer[ 512 ];
	DWORD BytesRead;
	String strResult;
	while ( ReadFile( hread, buffer, sizeof( buffer ) - 1, &BytesRead,
		NULL ) && BytesRead )
	{
		buffer[ BytesRead ] = '\0';
		strResult           = strResult + UnicodeString( buffer );

		Application->ProcessMessages( );
	}
	if ( hread )
	{
		CloseHandle( hread );
	}
	if ( pi.hProcess )
	{
		CloseHandle( pi.hProcess );
	}
	if ( pi.hThread )
	{
		CloseHandle( pi.hThread );
	}

	return strResult;
}

// ---------------------------------------------------------------------------

String __fastcall TUtils::GetComName( )
{
	DWORD dwSize = MAX_COMPUTERNAME_LENGTH + 1;
	WCHAR szBuf[ MAX_COMPUTERNAME_LENGTH + 1 ];
	szBuf[ 0 ] = L'\0';

	GetComputerNameW( szBuf, & dwSize );
	return String( szBuf );
}

TDateTime __fastcall TUtils::GetCreationTime( String FilePath )
{
	TSearchRec rc;
	FindFirst( FilePath, faArchive | faDirectory, rc );

	TDateTime dt = FileDateToDateTime( rc.Time );
	dt.FormatString( L"yyyy-mm-dd hh:nn:ss" );

	return dt;
}

String __fastcall TUtils::GetExeNamePath( )
{
	return ExtractFilePath( Application->ExeName );
}
// ---------------------------------------------------------------------------

String __fastcall TUtils::VersionInfo( String FileName )
{
	DWORD dwHandle = 0, dwVersionInfoSize;
	UINT uLength;
	LPVOID pFileInfo, ptr;
	String sOut;

	dwVersionInfoSize = GetFileVersionInfoSizeW( FileName.c_str( ), &dwHandle );
	if ( dwVersionInfoSize == 0 )
	{
		return L"0";
	}

	pFileInfo = ( LPVOID ) HeapAlloc( GetProcessHeap( ), HEAP_ZERO_MEMORY,
		dwVersionInfoSize );

	GetFileVersionInfoW( FileName.c_str( ), dwHandle, dwVersionInfoSize,
		pFileInfo );
	VerQueryValueW( pFileInfo, TEXT( "\\VarFileInfo\\Translation" ), & ptr,
		& uLength );

	WORD * id = ( WORD * ) ptr;

	String szString = L"\\StringFileInfo\\" + IntToHex( id[ 0 ], 4 ) +
		IntToHex( id[ 1 ], 4 ) + L"\\FileVersion";

	VerQueryValueW( pFileInfo, szString.c_str( ), & ptr, & uLength );
	sOut = String( ( WCHAR * ) ptr );
	HeapFree( GetProcessHeap( ), 0, pFileInfo );

	return sOut;
}

String __fastcall TUtils::ToBitPrtSecond( unsigned long rate )
{
	String sRet = L"";
	if ( rate == 0 )
	{
		sRet = L"0";
	}
	else if ( rate > 1048576 )
	{
		sRet.sprintf( L"%.2f MBps", ( double )rate / MB_SIZE );
	}
	else if ( rate > 1000 )
	{
		sRet.sprintf( L"%.2f KBps", ( double )rate / KB_SIZE );
	}
	else
	{
		sRet.sprintf( L"%.2f Bps", ( double )rate );
	}

	return sRet;
}

void __fastcall TUtils::Delay( int nTime )
{
	DWORD nStartTM;
	nStartTM = GetTickCount( );
	while ( true )
	{
		if ( nStartTM + nTime < GetTickCount( ) )
		{
			return;
		}

		::Sleep(10);
		Application->ProcessMessages( );
	}
}

ULONGLONG __fastcall TUtils::StrTimeToFrameCount( String In_strTimeCode )
{

	ULONGLONG ulOutFrameCount = ( _ttoi( In_strTimeCode.c_str( ) ) * (29.97 * 60 * 60) ) +
		( _ttoi( In_strTimeCode.c_str( ) + 3 ) * (29.97 * 60) ) +
		( _ttoi( In_strTimeCode.c_str( ) + 6 ) * 29.97 ) +
		( _ttoi( In_strTimeCode.c_str( ) + 9 ) );

	return ulOutFrameCount;
}

String __fastcall TUtils::FrameCountToStrTime( ULONG In_ulTimeCode )
{
	int hh = 0, mm = 0, ss = 0, ff = 0;
	hh = In_ulTimeCode /  (29.97 * 60 * 60);
	mm = ( In_ulTimeCode - ( hh *  (29.97 * 60 * 60) ) ) / (29.97 * 60);
	ss = ( In_ulTimeCode - ( ( hh *  (29.97 * 60 * 60) ) + ( mm * (29.97 * 60) ) ) ) / 29.97;
	ff = In_ulTimeCode - ( ( hh * (29.97 * 60 * 60) ) + ( mm * (29.97 * 60) ) + ( ss * 29.97 ) );

	return String( ).sprintf( L"%02d:%02d:%02d;%02d", hh, mm, ss, ff );
}

char * __fastcall TUtils::Dec2Bin( unsigned long value )
{
	int temp[ MAX_SIZE ], i;
	char result[ MAX_SIZE ];
	char bin[ MAX_SIZE ];

	for ( i = 255; i >= 0; i-- )
	{
		temp[ i ] = value & 1;
		value     = value >> 1; // 오른쪽으로 한 비트만 이동하여 2진수를 만든다.
	}

	for ( i = 0; i <= 255; i++ )
	{
		if ( temp[ i ] == 0 )
		{
			result[ i ] = '0';
		}

		if ( temp[ i ] == 1 )
		{
			result[ i ] = '1';
		}

		result[ i + 1 ] = NULL;
	}

	// 2진수 앞의 0은 짤라냄
	int n;
	for ( n = 0; n <= 255; n++ )
	{
		if ( result[ n ] == '1' )
		{
			break;
		}
	}

	int x = 0;
	for ( int i = n; i <= 255; i++ )
	{
		bin[ x ]     = result[ i ];
		bin[ x + 1 ] = NULL;
		x++ ;
	}

	return strdup( bin );
}

String __fastcall TUtils::BaseConverter(
	int    rsc,
	String data,
	int    obj )
{

	unsigned long decvalue;
	String temp;

	// 무조건 10진수로 변경을 함
	decvalue = wcstoul( data.c_str( ), NULL, rsc );

	switch ( obj )
	{
	case 2:
		{
			temp.sprintf( L"%s", Dec2Bin( decvalue ) );
			break;
		}
	case 8:
		{
			temp.sprintf( L"%o", decvalue );
			break;
		}
	case 10:
		{
			temp.sprintf( L"%lu", decvalue );
			break;
		}
	case 16:
		{
			temp.sprintf( L"%x", decvalue );
			break;
		}
	}

	return temp;
}

String __fastcall TUtils::GetSafeXmlString( String Value )
{
	// "":&quot;" "&:&amp;" "':&apos;" "<:&lt;" ">:&gt;"에 대한 처리
	if ( Value == L"" )
	{
		return Value;
	}
	String SafeString = AnsiReplaceStr( Value, L"&", L"&amp;" );
	SafeString = AnsiReplaceStr( SafeString, L"\"", L"&quot;" );
	SafeString = AnsiReplaceStr( SafeString, L"'", L"&apos;" );
	SafeString = AnsiReplaceStr( SafeString, L"<", L"&lt;" );
	SafeString = AnsiReplaceStr( SafeString, L">", L"&gt;" );
	return SafeString;
}

String __fastcall TUtils::GetSafeFilePath( String Value )
{
	// "":&quot;" "&:&amp;" "':&apos;" "<:&lt;" ">:&gt;"에 대한 처리
	if ( Value == L"" )
	{
		return Value;
	}

	String SafeString = AnsiReplaceStr( Value, L"\\", L" " );
	SafeString = AnsiReplaceStr( SafeString, L"\"", L" " );
	SafeString = AnsiReplaceStr( SafeString, L"?", L" " );
	SafeString = AnsiReplaceStr( SafeString, L":", L" " );
	SafeString = AnsiReplaceStr( SafeString, L"<", L" " );
	SafeString = AnsiReplaceStr( SafeString, L">", L" " );
	SafeString = AnsiReplaceStr( SafeString, L"/", L" " );
	SafeString = AnsiReplaceStr( SafeString, L"|", L" " );
	SafeString = AnsiReplaceStr( SafeString, L"*", L" " );

	SafeString = Trim(SafeString);
	return SafeString;
}


String __fastcall TUtils::GetDecodedXmlString( String Value )
{
	// "":&quot;" "&:&amp;" "':&apos;" "<:&lt;" ">:&gt;"에 대한 처리
	if ( Value == L"" )
	{
		return Value;
	}
	String SafeString = AnsiReplaceStr( Value, L"&quot;", L"\"" );
	SafeString = AnsiReplaceStr( SafeString, L"&amp;", L"&" );
	SafeString = AnsiReplaceStr( SafeString, L"&apos;", L"'" );
	SafeString = AnsiReplaceStr( SafeString, L"&lt;", L"<" );
	SafeString = AnsiReplaceStr( SafeString, L"&gt;", L">" );

	// SafeString = AnsiReplaceStr(SafeString, L"#60;", L"<");
	// SafeString = AnsiReplaceStr(SafeString, L"#62;", L">");
	return SafeString;
}

String __fastcall TUtils::FixedLen( String str, int size )
{
	for ( int i = str.Length( ); i < size; i++ )
	{
		str = L"0" + str;
	}

	return str;
}

void __fastcall TUtils::OldFileDelete( String sBasePath, int CheckDay )
{
	TSearchRec sr;
	int iAttributes = faAnyFile;

	// iAttributes |= faReadOnly * true;
	// iAttributes |= faHidden * true;
	// iAttributes |= faSysFile * true;
	// iAttributes |= faVolumeID * true;
	// iAttributes |= faDirectory * true;
	// iAttributes |= faArchive * true;
	// iAttributes |= faAnyFile * true;

	sBasePath = IncludeTrailingPathDelimiter( sBasePath );
	String sAnyFile = sBasePath + L"*.*";

	if ( FindFirst( sAnyFile, iAttributes, sr ) == 0 )
	{
		do
		{
			if ( ( sr.Attr & faDirectory ) == sr.Attr )
			{
				if ( sr.Name == L"." || sr.Name == L".." )
				{
					// Skip
				}
				else
				{
					OldFileDelete( sBasePath + sr.Name + L"\\", CheckDay );
					LastWriteTimeCheck( sBasePath + sr.Name, false, CheckDay );
				}
			}
			else
			{
				LastWriteTimeCheck( sBasePath + sr.Name, true, CheckDay );
			}
			Application->ProcessMessages( );

		}
		while ( FindNext( sr ) == 0 );

		FindClose( sr );
	}
}

BOOL __fastcall TUtils::LastWriteTimeCheck( String CheckFileName, BOOL CheckAtt, int CheckDay )
{

	int hFileHandle = FileOpen( CheckFileName.c_str( ), fmOpenRead );
	FILETIME CreateTime, LastAccessTime, LastWriteTime;
	GetFileTime( reinterpret_cast < HANDLE > ( hFileHandle ), & CreateTime, & LastAccessTime, & LastWriteTime );

	FILETIME TempTime;
	SYSTEMTIME SystemTime;
	TDateTime DateTime;

	FileTimeToLocalFileTime( & CreateTime, & TempTime );
	FileTimeToSystemTime( & TempTime, & SystemTime );
	DateTime = SystemTimeToDateTime( SystemTime );

	FileClose( hFileHandle );

	// if(Date() - CheckDay > DateTime.FormatString("yyyy-mm-dd am/pm hh:nn:ss"))
	if ( IncDay( Now( ), -CheckDay ) > DateTime )
	{
		if ( CheckAtt )
		{
			if ( DeleteFile( CheckFileName ) )
			{
				return true;
			}
			else
			{
				return false;
			}

		}
		else
		{
			if ( RemoveDir( CheckFileName ) )
			{
				return true;
			}
			else
			{
				return false;
			}

		}
	}
	return false;
}

ULONG __fastcall TUtils::TCToSec(String strTC)
{
	int hh = 0, nn = 0, ss = 0;
	hh= _ttoi(strTC.c_str());
	nn= _ttoi(strTC.c_str()+3);
	ss= _ttoi(strTC.c_str()+6);

	ULONG ulSec = (hh * 3600) + (nn* 60) + ss;
	return ulSec;
}


LONG __fastcall TUtils::StrTimeToSecCount( String strTimeCode )
{

	LONG lOutSecCount = ( _ttoi( strTimeCode.c_str( ) ) * 3600 ) +
		( _ttoi( strTimeCode.c_str( ) + 3 ) * 60 ) +
		( _ttoi( strTimeCode.c_str( ) + 6 ) );

	return lOutSecCount;
}

String __fastcall TUtils::SecCountToStrTime( LONG lTimeCode )
{
	int hh = 0, mm = 0, ss = 0;

	if ( lTimeCode >= 0 )
	{
		hh = lTimeCode / 3600;
		mm = ( lTimeCode - ( hh * 3600 ) ) / 60;
		ss = lTimeCode - ( ( hh * 3600 ) + ( mm * 60 ) );

		return String( ).sprintf( L"%02d:%02d:%02d", hh, mm, ss );
	}

	lTimeCode = lTimeCode * -1;
	hh = lTimeCode / 3600;
	mm = ( lTimeCode - ( hh * 3600 ) ) / 60;
	ss = lTimeCode - ( ( hh * 3600 ) + ( mm * 60 ) );

	return String( ).sprintf( L"-%02d:%02d:%02d", hh, mm, ss );
}

String __fastcall TUtils::SecCountToStrMinSec( LONG lTimeCode )
{
	int mm = 0, ss = 0;

	mm = lTimeCode / 60;
	ss = lTimeCode % 60;

	return String( ).sprintf( L"%02d:%02d", mm, ss );
}

String __fastcall TUtils::SecToTC(ULONG ulSec)
{
	int hh = 0, nn = 0, ss = 0;
	hh = ulSec / 3600;
	nn = (ulSec -(hh * 3600)) / 60;
	ss = ulSec - ((hh * 3600) + (nn * 60));

	String strTC = String().sprintf(L"%02d:%02d:%02d", hh, nn, ss);
	return strTC;
}

void __fastcall TUtils::SetControlFocus( TWinControl * Control )
{
	if ( Control == NULL )
	{
		return;
	}
	TForm * OwnerForm = ( TForm * )Control->Owner;

	if ( OwnerForm->Showing && Control->CanFocus( ) )
	{
		Control->SetFocus( );
	}
}
