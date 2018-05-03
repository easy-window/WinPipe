// pipiSonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PipeFrom.h"
#include "PipeFromDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define EXAMP_PIPE   _T("\\\\.\\pipe\\ReadPipe")

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


// CPipeFromDlg dialog




CPipeFromDlg::CPipeFromDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPipeFromDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bLoop = true;
}

void CPipeFromDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPipeFromDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CPipeFromDlg::OnBnClickedOk)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_LOOP, &CPipeFromDlg::OnBnClickedBtnLoop)
END_MESSAGE_MAP()


// CPipeFromDlg message handlers

BOOL CPipeFromDlg::OnInitDialog()
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

	OnBnClickedOk();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPipeFromDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPipeFromDlg::OnPaint()
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
HCURSOR CPipeFromDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPipeFromDlg::OnBnClickedOk()
{
	if(!m_pipe.Connect(EXAMP_PIPE))
	{
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
	return;
}

void CPipeFromDlg::OnDestroy()
{
	m_bLoop = false;
	m_pipe.Close();
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}


UINT CPipeFromDlg::PipiThread(LPVOID lpParam)
{
	CPipeFromDlg* pMain = (CPipeFromDlg*) lpParam;
	COLORREF dwColor = 0;

	while (pMain->m_bLoop) {
		COLORREF dwColor = RGB(
			pMain->GetDlgItemInt(IDC_EDIT_R),
			pMain->GetDlgItemInt(IDC_EDIT_G),
			pMain->GetDlgItemInt(IDC_EDIT_B)
		);
		int sleepTimes = 1000.0 / pMain->GetDlgItemInt(IDC_EDIT_TIMES);

		DWORD dwReturn = pMain->m_pipe.Send(&dwColor, sizeof(dwColor));;
		if(dwReturn != 0)
		{
			if (ERROR_NO_DATA == dwReturn) {
				pMain->GetDlgItem(IDOK)->EnableWindow(TRUE);
				pMain->OnBnClickedBtnLoop();
				break;
			}
			TRACE1("Not Write!%d\n", dwReturn);
		}
		else
		{
			pMain->SetDlgItemInt(IDC_EDIT_R, (GetRValue(dwColor) + 1) % 256);
			pMain->SetDlgItemInt(IDC_EDIT_G, (GetGValue(dwColor) + 1) % 256);
			pMain->SetDlgItemInt(IDC_EDIT_B, (GetBValue(dwColor) + 1) % 256);
		}
		Sleep(sleepTimes);
	}

	return 0;
}

void CPipeFromDlg::OnBnClickedBtnLoop()
{
	static bool bStart = true;
	if (bStart) {
		SetDlgItemText(IDC_BTN_LOOP, _T("Stop"));
		m_bLoop = true;
		AfxBeginThread(PipiThread, this);
	} else {
		SetDlgItemText(IDC_BTN_LOOP, _T("Start"));
		m_bLoop = false;
	}
	bStart = !bStart;
}
