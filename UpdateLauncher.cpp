//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("UnitMain.cpp", Form1);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	HANDLE Mutex;

	try
	{
		const wchar_t ProgMutex[] =_T("KBS_UpdateLauncher");
		if((Mutex = OpenMutex(MUTEX_ALL_ACCESS, false, ProgMutex)) == NULL)
		{
			Mutex = CreateMutex(NULL, true, ProgMutex);
		}
		else
		{
			MessageDlg(_T("KBS_UpdateLauncher 프로그램이 이미 작동하고 있습니다."), mtError, TMsgDlgButtons() << mbOK, 0);
			return 0;
		}

		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TForm1), &Form1);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
