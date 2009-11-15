// OBJ_VIEW.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "OBJ_VIEW.h"
#include "OBJ_VIEWDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COBJ_VIEWApp

BEGIN_MESSAGE_MAP(COBJ_VIEWApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// COBJ_VIEWApp construction

COBJ_VIEWApp::COBJ_VIEWApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only COBJ_VIEWApp object

COBJ_VIEWApp theApp;


// COBJ_VIEWApp initialization

BOOL COBJ_VIEWApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	//注册一个窗口类（用于GL窗口）
	WNDCLASS wndclsGL;
	::memset(&wndclsGL, 0, sizeof(WNDCLASS));
	wndclsGL.style         = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW|CS_OWNDC;
	wndclsGL.lpfnWndProc   = ::DefWindowProc;
	wndclsGL.hInstance     = AfxGetInstanceHandle();
	wndclsGL.hIcon         = 0;
	wndclsGL.hbrBackground = CreateSolidBrush( RGB(0,0,0) );
	wndclsGL.lpszMenuName  = 0;
	wndclsGL.hCursor       = ::LoadCursor(0,IDC_CROSS);
	wndclsGL.lpszClassName = L"GL_RENDER_WND";								//窗口类名
	AfxRegisterClass(&wndclsGL);

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CWinApp::OnIdle( 0 );

	COBJ_VIEWDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
