// pipiFatherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PipeMain.h"
#include "PipeMainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CpipiFatherDlg dialog

#define EXAMP_PIPE   _T("\\\\.\\pipe\\ReadPipe")


CPipeMainDlg::CPipeMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPipeMainDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPipeMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPipeMainDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CPipeMainDlg::OnBnClickedOk)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CpipiFatherDlg message handlers

BOOL CPipeMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_pipe.Create(EXAMP_PIPE, sizeof(COLORREF) * 2);
	m_pipe.SetRecvCallback(&RecvCallback, this);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPipeMainDlg::OnPaint()
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
HCURSOR CPipeMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPipeMainDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CPipeMainDlg::OnDestroy()
{
	m_pipe.Close();
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

void CPipeMainDlg::RecvCallback(void* Buffer, int nSize, LPVOID lpParam)
{
	if (NULL == Buffer || NULL == lpParam || nSize != sizeof(COLORREF)) return;
	COLORREF* pColor = (COLORREF*) Buffer;
	CPipeMainDlg* pMain = (CPipeMainDlg*)lpParam;
	pMain->SetDlgItemInt(IDC_EDIT_R, GetRValue(*pColor));
	pMain->SetDlgItemInt(IDC_EDIT_G, GetGValue(*pColor));
	pMain->SetDlgItemInt(IDC_EDIT_B, GetBValue(*pColor));

}