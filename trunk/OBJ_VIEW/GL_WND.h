#pragma once

#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>
#include "Model.h"


// GL_WND

//�Զ�����Ϣ�������ļ����Ͻ���ʱ�����������ڵ�
#define WM_FILEIN	WM_USER + 1

enum RenderState
{
	ePolyMode = 0,
	eCullMode,
};

enum RenderStateValue
{
	eLineMode = 0,
	eSolidMode,
	ePointMode,

	eBackCull,
	eFrontCull,
	eNonCull,
};


class GL_WND : public CWnd
{
	DECLARE_DYNAMIC(GL_WND)

public:
	GL_WND();
	virtual ~GL_WND();
	void CreateWnd( CWnd* parent, unsigned int ID, LPCTSTR wndClass );
	void Update();									//���»���
	void SetRenderState( RenderState rs, RenderStateValue val );
													//RenderState
	void CompileModel( Model& model );				//����ģ��

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDropFiles( HDROP hDropInfo );
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
	afx_msg void OnRButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnMouseMove( UINT nFlags, CPoint point );
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

protected:
	RECT m_wndRect;									//��������
	wchar_t m_fileNameBuffer[512];					//�ļ�������

	HDC m_hdc;
	HGLRC m_hrc;

	GLuint m_Model;									//ģ�͵���ʾ�б�

protected:
	void initGL();									//OpenGL��ʼ��

};


