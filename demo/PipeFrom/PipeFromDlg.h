// pipiSonDlg.h : header file
//

#pragma once


// CPipeFromDlg dialog
class CPipeFromDlg : public CDialog
{
// Construction
public:
	CPipeFromDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PIPISON_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	static UINT PipiThread(LPVOID lpParam);


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnLoop();
	DECLARE_MESSAGE_MAP()

	bool m_bLoop;

	CPipe m_pipe;
};
