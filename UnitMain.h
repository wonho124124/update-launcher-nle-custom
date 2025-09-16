//---------------------------------------------------------------------------

#ifndef UnitMainH
#define UnitMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdExplicitTLSClientServerBase.hpp>
#include <IdFTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>

#include <stdio.h>
#include "UtilsBCB.h"
#include "GMUtil.h"
#include "GMURLDownload.h"

#define RESULT_SUCCESS L"001"
#define RESULT_FAIL L"002"
#define RESULT_ETC L"003"

enum ttType{ttASCII, ttBinary};

typedef struct _CONFIG
{
	// Execute module name
	String MODULE_NAME;
	String DOWNLOAD_METHOD; // FTP, HTTP

	// FTP server info
	String HOST;
	String PORT;
	String USERNAME;
	String PASSWORD;
	bool   PASSIVE;
	int    TRANSFER_TYPE;

	// HTTP server info
	String URL_ROOT;

	// server path
	String VERSION_FILE;
	String UPDATE_FILE;

	// 권한상승
	bool    RUNASSPC;
    String  RUNASSPC_CMD;

}CONFIG, *PCONFIG;


// 다운로드 쓰레드
class TDownloadThread : public TThread
{
private:
	TTransferFiles *m_TransferFiles;

	void __fastcall DoControl();
protected:
	virtual void __fastcall Execute();
public:
	__fastcall TDownloadThread(TTransferFiles *TransferFiles);
	__fastcall ~TDownloadThread();
};

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label2;
	TLabel *Label1;
	TLabel *lblCurrentVersion;
	TLabel *lblUpdateVersion;
	TMemo *txtLog;
	TProgressBar *ProgressBar;
	TButton *btnUpdate;
	TButton *btnCancel;
	TTimer *tmrAutoStart;
	TIdFTP *IdFTP1;
	void __fastcall tmrAutoStartTimer(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall IdFTP1Work(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCount);
	void __fastcall IdFTP1WorkBegin(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCountMax);
	void __fastcall IdFTP1WorkEnd(TObject *ASender, TWorkMode AWorkMode);
	void __fastcall btnUpdateClick(TObject *Sender);
	void __fastcall IdFTP1Connected(TObject *Sender);
	void __fastcall IdFTP1Disconnected(TObject *Sender);


private:	// User declarations


	CONFIG 		m_stConfig;
	String      m_strRemotePath;

	TTransferFiles *	m_TransferFiles;
	TGMURLDownload *	m_URLDownload;

    String m_RetrieveCurrentDir;

public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	__fastcall ~TForm1();

    void __fastcall LoadConfig();
	void __fastcall LogMessage(String strMessage, int nLine);
	int __fastcall GetBuildVersion(String strVersionInfo);
    bool __fastcall FTP_Connect();
    bool __fastcall FTP_Disconnect();
	bool __fastcall FTP_Download(String strSourcePath, String strTargetPath);
	bool __fastcall HTTP_Download(String strSourcePath, String strTargetPath);
	LONGLONG __fastcall GetServerFileSize(String strFilePath);
	void __fastcall ExecuteExeModule();
	void __fastcall ExecuteUpdateMonitoringModule(String remoteVer = L"");

	void __fastcall DownloadStart(TObject *Sender);
	void __fastcall DownloadStop(TObject *Sender);
	void __fastcall DownloadProgressStatus(TObject *Sender, DWORD AMax, DWORD APosition, DWORD AStatusCode, String AStatusText, Boolean& AAbort);
	void __fastcall WindowFunc(Winapi::Messages::TMessage &msg);
    Boolean __fastcall DownloadFile(String AURL, String ASrcFileName, String ATrgFilePath, String ATrgFileName, Boolean AIsUnZip);
	Boolean __fastcall IsDownloadCancel();

	TUtils*     m_pUtils;
	int FFileIndex;
	Boolean FIsCancel;

};


//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
