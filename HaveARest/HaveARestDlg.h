
// HaveARestDlg.h : header file
//

#pragma once

#define TIME_INITIAL_STRING L"00h 00m 00s"

// CHaveARestDlg dialog
class CHaveARestDlg : public CDialogEx
{
// Construction
public:
	CHaveARestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_HAVEAREST_DIALOG };

private:
    bool m_controlChanged; // save the status of  the control which has been changed.

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
    void additionalInit();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedStart();
    afx_msg void OnBnClickedStop();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedAbout();
    afx_msg void OnBnClickedApply();
    BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnControlChanged(); 
    afx_msg void OnBnClickedstartscreensaver();
    afx_msg LONG OnHotKey(WPARAM wParam,LPARAM lParam);
    void showMainWindow(HWND hwnd = NULL);
    void hideMainWindow();
    afx_msg void OnBnClickedSave();
};

extern CWnd *global_pointer_cwnd;
extern CListBox *global_pointer_listbox_message;
extern CProgressCtrl *global_pointer_progressctrl;
