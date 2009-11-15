// GL_WND.cpp : implementation file
//
#include "stdafx.h"
#include "OBJ_VIEW.h"
#include "GL_WND.h"
#include "math.h"
#include "ArcBall.h"


//����任
Matrix4fT   Transform   = {  1.0f,  0.0f,  0.0f,  0.0f,				
0.0f,  1.0f,  0.0f,  0.0f,
0.0f,  0.0f,  1.0f,  0.0f,
0.0f,  0.0f,  0.0f,  1.0f };

//��һ�ε���ת
Matrix3fT   LastRot     = {  1.0f,  0.0f,  0.0f,					
0.0f,  1.0f,  0.0f,
0.0f,  0.0f,  1.0f };

//��ε���ת
Matrix3fT   ThisRot     = {  1.0f,  0.0f,  0.0f,					
0.0f,  1.0f,  0.0f,
0.0f,  0.0f,  1.0f };

ArcBallT    ArcBall(640.0f, 480.0f);				                // �����켣����
Point2fT    MousePt;												// ��ǰ�����λ��
bool        isClicked  = false;										// �Ƿ������
bool        isRClicked = false;										// �Ƿ��һ����
bool        isDragging = false;					                    // �Ƿ��϶�


// GL_WND

IMPLEMENT_DYNAMIC(GL_WND, CWnd)

GL_WND::GL_WND()
{
}

GL_WND::~GL_WND()
{
}


BEGIN_MESSAGE_MAP(GL_WND, CWnd)
	ON_WM_DROPFILES()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// GL_WND message handlers


//����GL����
void GL_WND::CreateWnd( CWnd* parent, unsigned int ID, LPCTSTR wndClass )
{
	CWnd* hwPrev = 0;

	//�����Ǹ���ܿؼ���λ�ô�С��������
	parent->GetDlgItem( ID )->GetWindowRect( &m_wndRect );
	parent->ScreenToClient( &m_wndRect );
	parent->GetDlgItem( ID )->ShowWindow( SW_HIDE );
	parent->GetDlgItem( ID )->DestroyWindow();


	Create( wndClass, L"", WS_CHILD, m_wndRect, parent, ID );

	SetDlgCtrlID( ID );

	ShowWindow( SW_SHOW );
	UpdateWindow();

	//�����ļ��Ϸ�
	DragAcceptFiles();

}


//���ļ����Ͻ���
void GL_WND::OnDropFiles( HDROP hDropInfo )
{
	CWnd* parent;
	parent = GetParent();

	DragQueryFile( hDropInfo, 0, m_fileNameBuffer, 512 );
	DragFinish( hDropInfo );

	::SendMessage( parent->m_hWnd, WM_FILEIN, (WPARAM)m_fileNameBuffer, 0 );

}


//���ڴ���ʱ����
int GL_WND::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_hdc = ::GetDC( m_hWnd );

	initGL();

	return 0;
}


//��ʼ��OpenGL
void GL_WND::initGL()
{
	PIXELFORMATDESCRIPTOR pfd = {0};
	pfd.nSize       = sizeof(pfd);
	pfd.nVersion    = 1;
	pfd.dwFlags     = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType  = PFD_TYPE_RGBA;
	pfd.cColorBits  = 24;
	pfd.cDepthBits  = 24;
	pfd.iLayerType  = PFD_MAIN_PLANE;
	int iFormat     = ChoosePixelFormat(m_hdc, &pfd);
	SetPixelFormat(m_hdc, iFormat, &pfd);

	m_hrc = wglCreateContext( m_hdc );
	wglMakeCurrent( m_hdc, m_hrc );

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glEnable(GL_COLOR_MATERIAL);
	glClearDepth( 1 );
	glDisable(GL_FOG);
	glDisable(GL_DITHER);
	glClearColor( 0.0f, 0.0f, 0.0f, 0.f );

	//�����ӿ�
	int width,height;
	width = m_wndRect.right-m_wndRect.left;
	height = m_wndRect.bottom-m_wndRect.top;
	glViewport( 0, 0, width, height );

	//����ͶӰ����
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 90.0f, (GLfloat)width/(GLfloat)height ,0.1f ,5000 );

	//����ģ�;���
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void GL_WND::OnPaint()
{
	if( !IsIconic() )
	{
		CWnd::OnPaint();	//���û����佫������Ⱦ
		Update();
	}
}


void GL_WND::Update()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

	///------------tmp
	glTranslated( 0, 0, -5 );

	//�켣��任
	glMultMatrixf( Transform.M );

	if( glIsList( m_Model ) )
	{
		glCallList( m_Model );
	}

	SwapBuffers( m_hdc );
}

//RenderState
void GL_WND::SetRenderState( RenderState rs, RenderStateValue val )
{
	switch( rs )
	{
	case ePolyMode:
		if( val == eLineMode )
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		}else if( val == eSolidMode )
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		}else if( val == ePointMode )
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
		}
		break;
	case eCullMode:
		if( val == eBackCull )
		{
			glEnable( GL_CULL_FACE );
			glCullFace( GL_BACK );
		}else if( val == eFrontCull )
		{
			glEnable( GL_CULL_FACE );
			glCullFace( GL_FRONT );
		}else if( val == eNonCull )
		{
			glDisable( GL_CULL_FACE );
		}
		break;
	default:
		break;
	}
}

//����ģ��
void GL_WND::CompileModel( Model& model )
{
	//�����ʾ�б��Ѿ����ڣ�ɾ��
	if( glIsList( m_Model ) )
	{
		glDeleteLists( m_Model, 1 );
	}

	m_Model = glGenLists( 1 );

	int i,j;
	Vertex* vtx;
	//����ģ�ͣ�������ʾ�б�
	glNewList( m_Model, GL_COMPILE );
	glBegin( GL_TRIANGLES );
	for( i = 0; i < model.m_Facets.size(); i++ )
	{
		vtx = model.m_Facets[i].m_vtx;
		for( j = 0; j < 3; j++ )
		{
			glNormal3fv( vtx[j].m_normal.mm );
			glTexCoord2fv( vtx[j].m_uv.mm );
			glVertex3fv( vtx[j].m_position.mm );
		}
	}
	glEnd();
	glEndList();

}

//���������
void GL_WND::OnLButtonDown(UINT nFlags, CPoint point)
{
	isDragging = true;						// �����϶�Ϊ����Ϊtrue
	LastRot = ThisRot;
	MousePt.s.X = point.x;
	MousePt.s.Y = point.y;
	ArcBall.click(&MousePt);
}

//����ƶ�
void GL_WND::OnMouseMove( UINT nFlags, CPoint point )
{
	if( isDragging )
	{
		Quat4fT     ThisQuat;
		MousePt.s.X = point.x;
		MousePt.s.Y = point.y;
		ArcBall.drag(&MousePt, &ThisQuat);						// ���¹켣��ı���
		Matrix3fSetRotationFromQuat4f(&ThisRot, &ThisQuat);		// ������ת��
		Matrix3fMulMatrix3f(&ThisRot, &LastRot);				
		Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);

		Update();
	}
}

//������̧��
void GL_WND::OnLButtonUp( UINT nFlags, CPoint point )
{
	isDragging = false;
}

//����Ҽ����
void GL_WND::OnRButtonDown( UINT nFlags, CPoint point )
{
	Matrix3fSetIdentity(&LastRot);								
	Matrix3fSetIdentity(&ThisRot);								
	Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);
}
