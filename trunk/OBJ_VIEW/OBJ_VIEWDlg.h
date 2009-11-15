// OBJ_VIEWDlg.h : header file
//

#pragma once
#include "GL_WND.h"
#include "ObjReader.h"


// COBJ_VIEWDlg dialog
class COBJ_VIEWDlg : public CDialog
{
// Construction
public:
	COBJ_VIEWDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_OBJ_VIEW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnFileIn( WPARAM, LPARAM );			//有文件拖入时
	afx_msg void OnBnClickedLinemode();
	afx_msg void OnBnClickedSolidmode();
	afx_msg void OnBnClickedPointmode();
	afx_msg void OnBnClickedBackcull();
	afx_msg void OnBnClickedFrontcull();
	afx_msg void OnBnClickedNocull();
	afx_msg void OnBnClickedOpenModel();
	DECLARE_MESSAGE_MAP()

	//open the model file
	bool OpenFile( const wchar_t* file );

	GL_WND m_RenderWnd;									//渲染窗口
	Model m_modelData;									//store the model data
	ObjReader m_objReader;								//read the obj file
	
};
 