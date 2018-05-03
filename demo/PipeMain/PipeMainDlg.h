// pipiFatherDlg.h : header file
//

#pragma once


// CpipiFatherDlg dialog
class CPipeMainDlg : public CDialog
{
// Construction
public:
	CPipeMainDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PIPIFATHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	static void CALLBACK RecvCallback(void* Buffer, int nSize, LPVOID lpParam);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

	CPipe m_pipe;
};
