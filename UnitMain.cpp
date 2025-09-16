//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitMain.h"
#include "Tlhelp32.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

int FindAndTerminateProcess(String Process, bool bTerminateProcess)
{
	int ProcCnt = 0;
    HANDLE  snapshot;
	PROCESSENTRY32 processinfo;

    processinfo.dwSize = sizeof(processinfo);
    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == NULL)
	{
		return ProcCnt;
	}

	BOOL status = Process32First(snapshot, &processinfo);
	while(status)
	{
		if(Process == processinfo.szExeFile) //process명 비교
		{
			// 프로세스 종료
			if(bTerminateProcess)
			{
				// 사용자가 Kill을 원하는 프로세스의 이름 입력시 같은 정보를 가진 프로세스를 찾았다면
				// OpenProcess를 이용해 핸들을 가져옵니다.
				HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, processinfo.th32ProcessID);
				if(hProcess != NULL)
				{
					TerminateProcess(hProcess, -1);
					CloseHandle(hProcess);
				}
			}
			else
			{
				// 프로세스 개수 체크
				ProcCnt++;
			}

		}
        status = Process32Next(snapshot, &processinfo);
    }

	CloseHandle(snapshot);

	return ProcCnt;
}

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	Application->ShowMainForm = false;

	m_pUtils = new TUtils();

	m_TransferFiles = new TTransferFiles;
	m_URLDownload = new TGMURLDownload( );

	memset(&m_stConfig, 0, sizeof(CONFIG));
	LoadConfig();
	tmrAutoStart->Enabled = true;
}
//---------------------------------------------------------------------------

__fastcall TForm1::~TForm1()
{
	FTP_Disconnect();
	delete  m_pUtils;
}


void __fastcall TForm1::LogMessage(String strMessage, int nLine)
{
    txtLog->Lines->Add(strMessage);
    m_pUtils->LogMessage(strMessage, nLine);
}

//---------------------------------------------------------------------------

void __fastcall TForm1::LoadConfig()
{
	TIniFile* ini = new TIniFile(ChangeFileExt(Application->ExeName, L"Config.ini"));
	try
	{
		m_stConfig.MODULE_NAME = ini->ReadString(L"CONFIG", L"MODULE_NAME", L"");
		m_stConfig.DOWNLOAD_METHOD = ini->ReadString(L"CONFIG", L"DOWNLOAD_METHOD", L"HTTP");

		if(m_stConfig.DOWNLOAD_METHOD == L"HTTP")
		{
			m_stConfig.URL_ROOT = ini->ReadString(L"SERVER", L"URL_ROOT", L"");
		}
		else if(m_stConfig.DOWNLOAD_METHOD == L"FTP")
		{
			m_stConfig.HOST = ini->ReadString(L"SERVER", L"HOST", L"");
			m_stConfig.PORT = ini->ReadString(L"SERVER", L"PORT", L"");
			m_stConfig.USERNAME = ini->ReadString(L"SERVER", L"USERNAME", L"");
			m_stConfig.PASSWORD = ini->ReadString(L"SERVER", L"PASSWORD", L"");
			m_stConfig.PASSIVE = ini->ReadBool(L"SERVER", L"PASSIVE", true);
			m_stConfig.TRANSFER_TYPE = ini->ReadBool(L"SERVER", L"TRANSFER_TYPE", (int)ttBinary);
		}


		m_stConfig.VERSION_FILE = ini->ReadString(L"SERVER", L"VERSION_FILE", L"");
		m_stConfig.UPDATE_FILE = ini->ReadString(L"SERVER", L"UPDATE_FILE", L"");

		m_stConfig.RUNASSPC = ini->ReadBool(L"SERVER", L"RUNASSPC", false);
		m_stConfig.RUNASSPC_CMD = ini->ReadString(L"SERVER", L"RUNASSPC_CMD", L""); // L"/program:%s /user:\"admin\" /password:\"mbcnpsmaster\"");
	}
	__finally
	{
		delete ini;
	}

	Form1->Caption = L" 자동업데이트 알림 - " + m_stConfig.MODULE_NAME;
}


//---------------------------------------------------------------------------
int __fastcall TForm1::GetBuildVersion(String strVersionInfo)
{
    int nBuildVersion = 0;
	TStringList* pVersionList = new TStringList();
	try
	{
		pVersionList->Delimiter = '.';
		pVersionList->DelimitedText = strVersionInfo;
		if(pVersionList->Count)
		{
			nBuildVersion = StrToIntDef(pVersionList->Strings[pVersionList->Count-1], 0);
        }
	}
	__finally
	{
		delete pVersionList;
	}

	return nBuildVersion;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ExecuteUpdateMonitoringModule(String remoteVer)
{
	String strUpdateMonitoringModulePath = ExtractFilePath(Application->ExeName) + L"update_monitoring.exe";
	if(FileExists(strUpdateMonitoringModulePath))
	{
		String strExecutePath = ExtractFilePath(Application->ExeName) + m_stConfig.MODULE_NAME;
		String args = String().sprintf(L"\"%s\" \"%s\"", strExecutePath.c_str(), remoteVer.c_str());
		ShellExecute(NULL, L"open", strUpdateMonitoringModulePath.c_str(), args.c_str(), NULL, SW_HIDE);
	}
}


void __fastcall TForm1::ExecuteExeModule()
{
	bool bAreadyExecute = false;

	// 프로세스가 실행되고 있는지 체크.
	if(FindAndTerminateProcess(m_stConfig.MODULE_NAME, false) > 0)
	{
		bAreadyExecute = true;
	}
	else if(FindAndTerminateProcess(ChangeFileExt(m_stConfig.MODULE_NAME, L""), false) > 0)
	{
		bAreadyExecute = true;
	}

	// 이미 실행되어 있는 경우에는 다시 실행하지 않음.
	if(!bAreadyExecute)
	{
		String strExecutePath = ExtractFilePath(Application->ExeName) + m_stConfig.MODULE_NAME;
		ShellExecute(NULL, L"open", strExecutePath.c_str(), L"", NULL, SW_SHOWNORMAL);
	}
	Close();
}

void __fastcall TForm1::tmrAutoStartTimer(TObject *Sender)
{
	tmrAutoStart->Enabled = false;

	lblCurrentVersion->Caption = m_pUtils->VersionInfo( ExtractFilePath(Application->ExeName) + m_stConfig.MODULE_NAME);
	LogMessage(String().sprintf(L"Local Version %s", lblCurrentVersion->Caption.c_str()), __LINE__);

	if(m_stConfig.DOWNLOAD_METHOD == L"FTP")
	{
		// FTP 방식인 경우 FTP 연결을 시도한다.
		// 연결 오류인 경우 바로 모듈을 실행.
		LogMessage(L"업데이트 서버에 연결합니다.", __LINE__);
		if(!FTP_Connect())
		{
			if(!IdFTP1->Connected())
			{
				LogMessage(L"에러 : 업데이트 서버 연결에 실패하였습니다.", __LINE__);
				//ExecuteUpdateMonitoringModule();
				ExecuteExeModule();
				return ;
			}
		}
	}


	String strVersionFilePath = String().sprintf(L"C:\\Users\\Public\\Documents\\Gemiso\\Update\\%s",
		ExtractFileName(AnsiReplaceText(m_stConfig.VERSION_FILE, L"/", L"\\")).c_str());
	if(!DirectoryExists(ExtractFilePath(strVersionFilePath)))
	{
		ForceDirectories(ExtractFilePath(strVersionFilePath));
	}

	LogMessage(L"버전 체크.", __LINE__);

	::Sleep(1000);
	bool bVersionCheck;
    if(m_stConfig.DOWNLOAD_METHOD == L"HTTP")
	{
		bVersionCheck = HTTP_Download(m_stConfig.URL_ROOT + m_stConfig.VERSION_FILE, strVersionFilePath);
	}
	else if(m_stConfig.DOWNLOAD_METHOD == L"FTP")
	{
		bVersionCheck = FTP_Download(m_stConfig.VERSION_FILE, strVersionFilePath);
	}

	// 버전 데이터 파일이 없거나,
	if(!bVersionCheck)
	{
		//ExecuteUpdateMonitoringModule();
		ExecuteExeModule();
		return;
	}

	TStringList *pVersionInfo = new TStringList();
	try
	{
		pVersionInfo->LoadFromFile(strVersionFilePath);
		lblUpdateVersion->Caption = pVersionInfo->Text.Trim();
		LogMessage(String().sprintf(L"Server Verison %s", lblUpdateVersion->Caption.c_str()), __LINE__);
	}
	__finally
	{
		delete pVersionInfo;
		DeleteFile(strVersionFilePath);
	}


	ExecuteUpdateMonitoringModule(lblUpdateVersion->Caption);

	if(lblCurrentVersion->Caption == lblUpdateVersion->Caption)
	{
		LogMessage(L"서버와 로컬 버전이 동일합니다.", __LINE__);
		ExecuteExeModule();
		return ;
	}

	Form1->Show();

}
//---------------------------------------------------------------------------



bool __fastcall TForm1::FTP_Connect()
{
    try
	{
		// 이미 연결 된 경우에는 연결 끊고 다시 연결.
		if(IdFTP1->Connected())
		{
			FTP_Disconnect();
		}

		IdFTP1->Host = m_stConfig.HOST;
		IdFTP1->Port = StrToIntDef(m_stConfig.PORT, 21);
		IdFTP1->Username = m_stConfig.USERNAME;
		IdFTP1->Password = m_stConfig.PASSWORD;


		IdFTP1->Passive = m_stConfig.PASSIVE;
		if((ttType)m_stConfig.TRANSFER_TYPE == ttASCII)
		{
			IdFTP1->TransferType = ftASCII;
		}
		else if((ttType)m_stConfig.TRANSFER_TYPE == ttBinary)
		{
			IdFTP1->TransferType = ftBinary;
		}

		LogMessage(String().sprintf(L"Connect. %s:%s@%s:%d",
			IdFTP1->Username.c_str(), IdFTP1->Password.c_str(), IdFTP1->Host.c_str(), IdFTP1->Port), __LINE__);
		LogMessage(String().sprintf(L"Passive : %s, TransferType : %s",
			IdFTP1->Passive ? L"PASSIVE" : L"ACTIVE",  IdFTP1->TransferType == ftBinary ? L"Binary" : L"ASCII"), __LINE__);

		IdFTP1->Connect();
    }
	catch(Exception& e)
    {
		LogMessage(L"Connect exception. " + e.Message, __LINE__);
        return false;
    }

    return true;
}

bool __fastcall TForm1::FTP_Disconnect()
{
    try
	{
		// 연결이 되어 있는 상태라면 연결 끊기
		if(IdFTP1->Connected())
		{
			IdFTP1->Disconnect();
			LogMessage(L"Disconnect. ", __LINE__);
		}
    }
	catch(Exception& e)
    {
		LogMessage(L"Disconnect exception. " + e.Message, __LINE__);
        return false;
    }

    return true;
}

bool __fastcall TForm1::FTP_Download(String strSourcePath, String strTargetPath)
{
	if(IdFTP1->Connected())
	{
		try
		{
			LogMessage(L"Remote path : " + strSourcePath, __LINE__);
			LogMessage(L"Local path : " + strTargetPath, __LINE__);

			m_strRemotePath = strSourcePath;
			if(FileExists(strTargetPath))
			{
				LogMessage(L"Delete . " + strTargetPath, __LINE__);
				DeleteFile(strTargetPath);
			}

			m_RetrieveCurrentDir = IdFTP1->RetrieveCurrentDir();
			LogMessage(String().sprintf(L"RetrieveCurrentDir : %s", IdFTP1->RetrieveCurrentDir().c_str()), __LINE__);

			String strTempDir = AnsiReplaceText(strSourcePath, L"/", L"\\");

			String strChangeDir = ExtractFileDir(strTempDir);
			String strFileName = ExtractFileName(strTempDir);
			LogMessage(String().sprintf(L"Dir : %s, Name : %s", strChangeDir.c_str(), strFileName.c_str()), __LINE__);

			if (m_RetrieveCurrentDir != strChangeDir)
			{
				try
				{
					if (strChangeDir == String())
					{
                        strChangeDir = "/";
					}
					LogMessage(String().sprintf(L"ChangeDir : %s", strChangeDir.c_str()), __LINE__);
					IdFTP1->ChangeDir(strChangeDir);

					LogMessage(String().sprintf(L"RetrieveCurrentDir : %s", IdFTP1->RetrieveCurrentDir().c_str()), __LINE__);
				}
				catch(Exception &ex)
				{
					LogMessage(L"Exception. ChangeDir() " + ex.Message, __LINE__);
				}

			}

			LogMessage(String().sprintf(L"src : %s, trg : %s", strFileName.c_str(), strTargetPath.c_str()), __LINE__);

			for(int n=0; n<2; n++)
			{
				try
				{

					IdFTP1->Get(strFileName, strTargetPath, true);  // GET : Over Write.

					bool bFileExists = FileExists(strTargetPath);
					LONGLONG llFileSize = m_pUtils->GetFileSize(strTargetPath);

					LogMessage(String().sprintf(L"File exists(%s), Size=(%s)",
						bFileExists ? L"TRUE" : L"FALSE", String(llFileSize).c_str()), __LINE__);
					if(bFileExists && llFileSize > 0)
					{
						break;
					}
				}
				catch(Exception &e)
				{
					LogMessage(L"Exception. Get() " + e.Message, __LINE__);

					bool bFileExists = FileExists(strTargetPath);
					LONGLONG llFileSize = m_pUtils->GetFileSize(strTargetPath);
					LogMessage(String().sprintf(L"File exists(%s). Size=(%s)",
						bFileExists ? L"TRUE" : L"FALSE", String(llFileSize).c_str()), __LINE__);

					if(bFileExists && llFileSize > 0)
					{
						break;
					}
					else
					{
						return bFileExists && llFileSize > 0;
					}
				}
			}

		}
		catch(Exception &ex)
		{
			LogMessage(L"Exception.. " + ex.Message, __LINE__);
		}
	}
	else
	{
		LogMessage(L"에러 : 업데이트 서버에 연결이 되지 않았습니다.", __LINE__);
		return false;
	}

    return true;
}

bool __fastcall TForm1::HTTP_Download(String strSourcePath, String strTargetPath)
{

	try
	{

        LogMessage(L"Remote path : " + strSourcePath, __LINE__);
		LogMessage(L"Local path : " + strTargetPath, __LINE__);

		m_strRemotePath = strSourcePath;
		if(FileExists(strTargetPath))
		{
			DeleteFile(strTargetPath);
		}

		TRANSFER_FILE * TransferFile = new TRANSFER_FILE;
		ParseDownloadURL( strSourcePath, TransferFile );
		TransferFile->trg_file_path = ExtractFilePath( strTargetPath );
		TransferFile->trg_file_name = ExtractFileName( strTargetPath );
		TransferFile->file_status   = L"";
		TransferFile->file_id   = ChangeFileExt(ExtractFileName( strTargetPath ), L"");
		m_TransferFiles->Add( TransferFile );

		TDownloadThread * pDownloadThread = new TDownloadThread( m_TransferFiles );
		pDownloadThread->Resume();
		pDownloadThread->WaitFor();

		m_TransferFiles->Clear();

		if(!FileExists(strTargetPath))
		{
			return false;
		}

	}
	__finally
	{

	}

    return true;
}

void __fastcall TForm1::btnCancelClick(TObject *Sender)
{

	ExecuteExeModule();
}
//---------------------------------------------------------------------------


LONGLONG __fastcall TForm1::GetServerFileSize(String strFilePath)
{
	String strChangeDir, strFileName;
	strChangeDir = ExtractFileDir(AnsiReplaceStr(strFilePath, L"/", L"\\"));
	strFileName = ExtractFileName(AnsiReplaceStr(strFilePath, L"/", L"\\"));
	try
	{
		IdFTP1->ChangeDir(strChangeDir);
	}
	catch(Exception &ex)
	{
		//
	}

	LONGLONG lFileSize = IdFTP1->Size(strFileName);
	if(lFileSize > 0)
	{
		return lFileSize;
	}

	return 0;
}
void __fastcall TForm1::IdFTP1Work(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCount)
{
	try
	{
		if(ProgressBar->Max >= AWorkCount)
		{
			ProgressBar->Position = AWorkCount;
		}
	}
	catch(Exception &e)
	{
		LogMessage(L"FTP work exception. " + e.Message, __LINE__);
	}


}
//---------------------------------------------------------------------------
void __fastcall TForm1::IdFTP1WorkBegin(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCountMax)

{
	try
	{
		if(AWorkCountMax)
		{
			ProgressBar->Max = AWorkCountMax;
		}
		else
		{
			//ProgressBar->Max = GetServerFileSize(m_strRemotePath);
		}
	}
	catch(Exception &e)
	{
		LogMessage(L"FTP begin exception. " + e.Message, __LINE__);
	}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::IdFTP1WorkEnd(TObject *ASender, TWorkMode AWorkMode)
{
	try
	{
		ProgressBar->Position = ProgressBar->Max;
	}
	catch(Exception &e)
	{
		LogMessage(L"FTP end exception. " + e.Message, __LINE__);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::btnUpdateClick(TObject *Sender)
{

	FindAndTerminateProcess(m_stConfig.MODULE_NAME, true);
	FindAndTerminateProcess(ChangeFileExt(m_stConfig.MODULE_NAME, L""), true);

	String strUpdateFilePath = String().sprintf(L"C:\\Users\\Public\\Documents\\Gemiso\\Update\\%s",
		ExtractFileName(AnsiReplaceText(m_stConfig.UPDATE_FILE, L"/", L"\\")).c_str());
	if(!DirectoryExists(ExtractFilePath(strUpdateFilePath)))
	{
		ForceDirectories(ExtractFilePath(strUpdateFilePath));
	}

	if(m_stConfig.DOWNLOAD_METHOD == L"HTTP")
	{
		HTTP_Download(m_stConfig.URL_ROOT + m_stConfig.UPDATE_FILE, strUpdateFilePath);
	}
	else if(m_stConfig.DOWNLOAD_METHOD == L"FTP")
	{
		FTP_Download(m_stConfig.UPDATE_FILE, strUpdateFilePath);
	}


	if(m_stConfig.RUNASSPC)
	{
		String runasspcPath = ExtractFilePath(Application->ExeName) + L"runasspc.exe";
		if(FileExists(runasspcPath) && m_stConfig.RUNASSPC_CMD != L"")
		{
			String cmd = String().sprintf(m_stConfig.RUNASSPC_CMD.c_str(), strUpdateFilePath.c_str());

			// runasspc.exe 프로그램을 이용하여 권한 상승 실행
			ShellExecute(NULL, L"open", runasspcPath.c_str(), cmd.c_str(), NULL, SW_SHOWNORMAL);
		}
		else
		{
            // 기본 실행.
			ShellExecute(NULL, L"open", strUpdateFilePath.c_str(), L"", NULL, SW_SHOWNORMAL);
		}
	}
	else
	{
		// 기본 실행.
		ShellExecute(NULL, L"open", strUpdateFilePath.c_str(), L"", NULL, SW_SHOWNORMAL);
	}
	Close();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::IdFTP1Connected(TObject *Sender)
{
	LogMessage(L"Connected", __LINE__);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::IdFTP1Disconnected(TObject *Sender)
{
	LogMessage(L"Disconnected", __LINE__);
}
//---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
__fastcall TDownloadThread::TDownloadThread( TTransferFiles * TransferFiles )
	: TThread( true )
	, m_TransferFiles( NULL )
{
	m_TransferFiles = TransferFiles;
}

// ---------------------------------------------------------------------------
__fastcall TDownloadThread::~TDownloadThread( )
{

}

// ---------------------------------------------------------------------------
void __fastcall TDownloadThread::DoControl( )
{
	try
	{
		Form1->FFileIndex = 0;
		while ( ( !Form1->FIsCancel ) && ( !Terminated ) && ( Form1->FFileIndex < m_TransferFiles->Count ) )
		{
			String FileId = m_TransferFiles->Items[ Form1->FFileIndex ]->file_id;
			String SrcFilePath = m_TransferFiles->Items[ Form1->FFileIndex ]->src_file_path;
			String SrcFileName = m_TransferFiles->Items[ Form1->FFileIndex ]->src_file_name;
			String TrgFilePath = m_TransferFiles->Items[ Form1->FFileIndex ]->trg_file_path;
			String TrgFileName = m_TransferFiles->Items[ Form1->FFileIndex ]->trg_file_name;
			String FileStatus = m_TransferFiles->Items[ Form1->FFileIndex ]->file_status;
			if ( m_TransferFiles->Items[ Form1->FFileIndex ] != NULL )
			{
				/*
				if ( m_TransferFiles->Items[ Form1->FFileIndex ]->file_status == FILE_STATUS_DELETE )
				{

					// DELETE
					String strFilePath = IncludeTrailingPathDelimiter( TrgFilePath ) + TrgFileName;
					Form1->m_pUtils->LogMessage(String().sprintf(L"%d Delete %s", Form1->FFileIndex, strFilePath.c_str()), __LINE__);
					if ( FileExists( strFilePath ) )
					{
//						DeleteFile( strFilePath );
					}

					m_TransferFiles->Items[ Form1->FFileIndex ]->result = RESULT_SUCCESS;
				}

				else if(m_TransferFiles->Items[ Form1->FFileIndex ]->file_status == FILE_STATUS_DUPLICATION)
				{
					String strFilePath = IncludeTrailingPathDelimiter( TrgFilePath ) + TrgFileName;
					Form1->m_pUtils->LogMessage(String().sprintf(L"%d Duplication %s", Form1->FFileIndex, strFilePath.c_str()), __LINE__);
					m_TransferFiles->Items[ Form1->FFileIndex ]->result = RESULT_SUCCESS;
				}

				else
				*/
				{
					// DOWNLOAD
					String strSrcFilePath = IncludeTrailingPathDelimiter( SrcFilePath ) + SrcFileName;
					String strTrgFilePath = IncludeTrailingPathDelimiter( TrgFilePath ) + TrgFileName;
					Form1->m_pUtils->LogMessage(String().sprintf(L"%d Download %s -> %s", Form1->FFileIndex,
						strSrcFilePath.c_str(), strTrgFilePath.c_str()), __LINE__);
					if ( !Form1->DownloadFile( SrcFilePath, SrcFileName, TrgFilePath, TrgFileName, false ) )
					{
						m_TransferFiles->Items[ Form1->FFileIndex ]->result = RESULT_FAIL;
						String strMsg = String( ).sprintf( L"'%s' 파일을 다운로드 할 수 없습니다.\r\n%s",
							TrgFileName.c_str( ), SysErrorMessage( GetLastError( ) ).c_str( ) );
						Form1->m_pUtils->LogMessage(strMsg, __LINE__);
					}
					else
					{
						m_TransferFiles->Items[ Form1->FFileIndex ]->result = RESULT_SUCCESS;
					}
				}


			}
			Form1->FFileIndex++ ;

		}
	}
	__finally
	{

	}
}

// ---------------------------------------------------------------------------
void __fastcall TDownloadThread::Execute( )
{
	Synchronize( DoControl );
}


void __fastcall TForm1::DownloadStart( TObject * Sender )
{
	if ( FFileIndex >= 0 )
	{
		if ( m_TransferFiles->Items[ FFileIndex ] != NULL )
		{
//			lblFileName->Caption = m_TransferFiles->Items[ FFileIndex ]->trg_file_name;
//
//			lblFileSize->Caption       = "";
//			lblFileStatus->Caption     = "";
			ProgressBar->Position = 0;

		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::DownloadStop( TObject * Sender )
{
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::DownloadProgressStatus( TObject * Sender, DWORD AMax, DWORD APosition, DWORD AStatusCode, String AStatusText,
	Boolean & AAbort )
{
	Double Percent;
	ProgressBar->Max      = AMax;
	ProgressBar->Position = APosition;

	/*
	if ( ProgressBar->Max > 0 )
	{
		Percent = ( double )ProgressBar->Position / ( double )ProgressBar->Max * 100;
		// lblFileProgress->Caption.sprintf(L"%d%%", Percent * 100);
		lblFileProgress->Caption = FormatFloat( "#,0%;-#,0%;", Percent );

	}

	// lblFileSize->Caption.sprintf(L"%s", GetSizeString(APosition, AMax));
	// lblFileStatus->Caption.sprintf(L"%s", AStatusText);

	lblFileSize->Caption = GetTransferSizeString( APosition, AMax );
	lblFileStatus->Caption = AStatusText;
	*/
	Application->ProcessMessages( );
}

// ---------------------------------------------------------------------------
Boolean __fastcall TForm1::DownloadFile( String AURL, String ASrcFileName, String ATrgFilePath, String ATrgFileName, Boolean AIsUnZip )
{
	Boolean Result = false;

	if ( ( AURL == L"" ) || ( ATrgFileName == L"" ) )
	{
		return Result;
	}

	if ( AURL[ AURL.Length( ) ] != L'/' )
	{
		AURL = AURL + L"/";
	}

	// Local 디렉토리가 지정이 안되어 있으면 temp 폴더로
	if ( ATrgFilePath == "" )
	{
		wchar_t Buf[ MAX_PATH ] =
		{
			0,
		} ;
		int Len = GetTempPath( MAX_PATH, Buf );
		ATrgFilePath = String( Buf );
	}

	if ( ATrgFilePath[ ATrgFilePath.Length( ) ] != L'\\' )
	{
		ATrgFilePath = ATrgFilePath + L"\\";
	}

	String ServerFileName = AURL + ASrcFileName;
	String LocalFileName = ATrgFilePath + ATrgFileName;

	if ( !DirectoryExists( ATrgFilePath ) )
	{
		if ( !ForceDirectories( ATrgFilePath ) )
		{
			// throw new Exception(L"Cannot create directory " + ATrgFilePath);
			return Result;
		}
	}

	// TGMURLDownload *URLDownload = new TGMURLDownload();
	try
	{
		m_URLDownload->OnStart          = DownloadStart;
		m_URLDownload->OnStop           = DownloadStop;
		m_URLDownload->OnProgressStatus = DownloadProgressStatus;
		m_URLDownload->URL              = ServerFileName;

		Result = m_URLDownload->Download( LocalFileName );
		if ( !Result )
		{
			// throw new Exception(ServerFileName + L" Download Error");
			return Result;
		}
	}
	__finally
	{
		// delete URLDownload;
	}

	if ( ( AIsUnZip ) && ( FileExists( LocalFileName ) ) )
	{
		wchar_t Buf[ MAX_PATH ] =
		{
			0,
		} ;
		int    Len = GetTempPath( MAX_PATH, Buf );
		String SystemPath = String( Buf );

		if ( SystemPath[ SystemPath.Length( ) ] != L'\\' )
		{
			SystemPath = SystemPath + L"\\";
		}

		// if (!FileExists(SystemPath + L"UNZDLL.DLL")) {
		// Result =  ((URLDownloadToFile(NULL, String(AURL + L"UNZDLL.DLL").c_str(), String(SystemPath + L"UNZDLL.DLL").c_str(), 0, NULL)) == S_OK);			Result = true;
		// }
	}
	return Result;
}

Boolean __fastcall TForm1::IsDownloadCancel( )
{
	MessageBeep( MB_ICONQUESTION );
	if ( MessageBox( Handle, L"다운로드를 중지하시겠습니까?.", Application->Title.c_str( ), MB_OKCANCEL + MB_ICONQUESTION ) == ID_OK )
	{
		return true;
	}
	else
	{
		return false;
	}
}
