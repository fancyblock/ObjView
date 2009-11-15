// OBJ_VIEWDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OBJ_VIEW.h"
#include "OBJ_VIEWDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// COBJ_VIEWDlg dialog




COBJ_VIEWDlg::COBJ_VIEWDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COBJ_VIEWDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COBJ_VIEWDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COBJ_VIEWDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE( WM_FILEIN, OnFileIn )
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_LINEMODE, &COBJ_VIEWDlg::OnBnClickedLinemode)
	ON_BN_CLICKED(IDC_SOLIDMODE, &COBJ_VIEWDlg::OnBnClickedSolidmode)
	ON_BN_CLICKED(IDC_POINTMODE, &COBJ_VIEWDlg::OnBnClickedPointmode)
	ON_BN_CLICKED(IDC_BACKCULL, &COBJ_VIEWDlg::OnBnClickedBackcull)
	ON_BN_CLICKED(IDC_FRONTCULL, &COBJ_VIEWDlg::OnBnClickedFrontcull)
	ON_BN_CLICKED(IDC_NOCULL, &COBJ_VIEWDlg::OnBnClickedNocull)
	ON_BN_CLICKED(IDC_OPEN_MODEL, &COBJ_VIEWDlg::OnBnClickedOpenModel)
END_MESSAGE_MAP()


// COBJ_VIEWDlg message handlers

BOOL COBJ_VIEWDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	//创建渲染窗口
	m_RenderWnd.CreateWnd( this, IDC_GL_WND, L"GL_RENDER_WND" );
	//初始渲染状态
	m_RenderWnd.SetRenderState( ePolyMode, eLineMode );
	m_RenderWnd.SetRenderState( eCullMode, eNonCull );
	( (CButton*)GetDlgItem( IDC_LINEMODE ) )->SetCheck( 1 );
	( (CButton*)GetDlgItem( IDC_NOCULL ) )->SetCheck( 1 );

	m_RenderWnd.Update();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COBJ_VIEWDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COBJ_VIEWDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COBJ_VIEWDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//有文件拖入时
LRESULT COBJ_VIEWDlg::OnFileIn( WPARAM wParam, LPARAM lParam )
{
	wchar_t* fileName;

	fileName = (wchar_t*)wParam;

	if(	!OpenFile( fileName ) )
	{
		MessageBox( L"It's not a OBJ Model file!", L"Error", MB_OK );
	}

	return 0;
}

//Set the Line Mode
void COBJ_VIEWDlg::OnBnClickedLinemode()
{
	CButton* otherRadio = (CButton*)GetDlgItem( IDC_SOLIDMODE );
	otherRadio->SetCheck( 0 );
	otherRadio = (CButton*)GetDlgItem( IDC_POINTMODE );
	otherRadio->SetCheck( 0 );

	m_RenderWnd.SetRenderState( ePolyMode, eLineMode );
	m_RenderWnd.Update();

}

//Set the Solid Mode
void COBJ_VIEWDlg::OnBnClickedSolidmode()
{
	CButton* otherRadio = (CButton*)GetDlgItem( IDC_LINEMODE );
	otherRadio->SetCheck( 0 );
	otherRadio = (CButton*)GetDlgItem( IDC_POINTMODE );
	otherRadio->SetCheck( 0 );

	m_RenderWnd.SetRenderState( ePolyMode, eSolidMode );
	m_RenderWnd.Update();

}

//Set the Point Mode
void COBJ_VIEWDlg::OnBnClickedPointmode()
{
	CButton* otherRadio = (CButton*)GetDlgItem( IDC_LINEMODE );
	otherRadio->SetCheck( 0 );
	otherRadio = (CButton*)GetDlgItem( IDC_SOLIDMODE );
	otherRadio->SetCheck( 0 );

	m_RenderWnd.SetRenderState( ePolyMode, ePointMode );
	m_RenderWnd.Update();

}

//Back Cull
void COBJ_VIEWDlg::OnBnClickedBackcull()
{
	CButton* otherRadio = (CButton*)GetDlgItem( IDC_FRONTCULL );
	otherRadio->SetCheck( 0 );
	otherRadio = (CButton*)GetDlgItem( IDC_NOCULL );
	otherRadio->SetCheck( 0 );

	m_RenderWnd.SetRenderState( eCullMode, eBackCull );
	m_RenderWnd.Update();

}

//Cull Front
void COBJ_VIEWDlg::OnBnClickedFrontcull()
{
	CButton* otherRadio = (CButton*)GetDlgItem( IDC_BACKCULL );
	otherRadio->SetCheck( 0 );
	otherRadio = (CButton*)GetDlgItem( IDC_NOCULL );
	otherRadio->SetCheck( 0 );

	m_RenderWnd.SetRenderState( eCullMode, eFrontCull );
	m_RenderWnd.Update();
}

//Non Cull
void COBJ_VIEWDlg::OnBnClickedNocull()
{
	CButton* otherRadio = (CButton*)GetDlgItem( IDC_BACKCULL );
	otherRadio->SetCheck( 0 );
	otherRadio = (CButton*)GetDlgItem( IDC_FRONTCULL );
	otherRadio->SetCheck( 0 );

	m_RenderWnd.SetRenderState( eCullMode, eNonCull );
	m_RenderWnd.Update();
}

//打开模型
void COBJ_VIEWDlg::OnBnClickedOpenModel()
{
	CFileDialog openFile( true, NULL, NULL, OFN_FILEMUSTEXIST, L"Obj Model|*.obj||" );

	openFile.DoModal();

	CString path = openFile.GetPathName();

	if(	!OpenFile( path.GetString() ) )
	{
		MessageBox( L"It's not a OBJ Model file!", L"Error", MB_OK );
	}

}

//open a model file
bool COBJ_VIEWDlg::OpenFile( const wchar_t* file )
{
	CFile theFile;

	theFile.Open( file, CFile::modeRead );
	m_modelData.Clear();
	bool result = m_objReader( theFile, m_modelData );
	theFile.Close();

	if( result )
	{
		m_RenderWnd.CompileModel( m_modelData );
		m_RenderWnd.Update();

		CString info;
		info.Format( L"共有三角形%d个", m_modelData.m_Facets.size() );
		SetDlgItemText( IDC_FACECOUNT, info );
	}

	return result;
}
