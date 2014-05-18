
// HaveARestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HaveARest.h"
#include "HaveARestDlg.h"
#include "afxdialogex.h"
#include "CommonAPI.h"
#include "Screen.h"
#include "C_Timer.h"
#include "AppFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWnd *global_pointer_cwnd = NULL;
CListBox *global_pointer_listbox_message =NULL ;
CProgressCtrl *global_pointer_progressctrl = NULL;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHaveARestDlg dialog
CHaveARestDlg::CHaveARestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHaveARestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_controlChanged = false;
}

void CHaveARestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHaveARestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_Start, &CHaveARestDlg::OnBnClickedStart)
    ON_BN_CLICKED(IDC_Stop, &CHaveARestDlg::OnBnClickedStop)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDCANCEL, &CHaveARestDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_About, &CHaveARestDlg::OnBnClickedAbout)
    ON_BN_CLICKED(IDC_Apply, &CHaveARestDlg::OnBnClickedApply)
    ON_EN_CHANGE(IDC_workingTime, &CHaveARestDlg::OnControlChanged)
    ON_EN_CHANGE(IDC_restTime, &CHaveARestDlg::OnControlChanged)
    ON_BN_CLICKED(IDC_startAutomaticallyAtIntermittent, &CHaveARestDlg::OnControlChanged)
    ON_BN_CLICKED(IDC_alertTenSecondsLeft, &CHaveARestDlg::OnControlChanged)
    ON_BN_CLICKED(IDC_startAndMinimize, &CHaveARestDlg::OnControlChanged)
    ON_BN_CLICKED(IDC_startScreenSaver, &CHaveARestDlg::OnBnClickedstartscreensaver)
    ON_MESSAGE(WM_HOTKEY, &CHaveARestDlg::OnHotKey)
    ON_BN_CLICKED(IDC_Save, &CHaveARestDlg::OnBnClickedSave)
END_MESSAGE_MAP()


// CHaveARestDlg message handlers

BOOL CHaveARestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	// TODO: Add extra initialization here
	// Global variable should be assigned value firstly.
    global_pointer_cwnd = this;
    global_pointer_listbox_message = (CListBox *)this->GetDlgItem(IDC_Message);
    global_pointer_progressctrl = (CProgressCtrl *)this->GetDlgItem(IDC_remainingTime);

  	//ShowWindow(SW_MINIMIZE); 

    additionalInit(); // MUST NOT put this function into OnPaint().

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHaveARestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
    else if ( SC_MINIMIZE == nID )
    {
        hideMainWindow();
    }
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHaveARestDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHaveARestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Additional initialisation.
void CHaveARestDlg::additionalInit()
{
	//File Handle, try to read configuration from conf file.
    if ( true == appFile.init() )
    {
        // check if the app has been run
        if ( IsAppHasRun(appFile.getAppName()) )
        {
            CString windowName, oldWindowName;
            CWnd *oldCwnd = NULL;
            GetWindowText(windowName);
            oldWindowName = windowName;
            windowName += L" - VICE";
            SetWindowText(windowName);
            oldCwnd = FindWindow(NULL, oldWindowName);
            showMainWindow(oldCwnd->m_hWnd);
            CDialogEx::OnCancel();
        }

        if ( true == appFile.isConfFileExist() )
        {
            int index = haveARest.showMessage(L"Assigning configuration to program ...");
            if ( haveARest.handleOfConfFile(true) )
                haveARest.showMessage(L"Assigning configuration to program ... OK", index);
            else
                haveARest.showMessage(L"Assigning configuration to program ... FAILED!", index);
		}
		else
		{
			haveARest.showMessage(L"WARNING: configuration file not exists.");
            haveARest.showMessage(L"Using default configuration.");
		}
	}
    else
    {
		haveARest.showMessage(L"WARNING: CAppFile Class Init Failed!");
    }

    // Set control state of main window.
	CString cstr_value;
    cstr_value.Format(L"%d", props.prop[WORKING_TIME_NUMBER].prop_value);
	GetDlgItem(IDC_workingTime)->SetWindowText(cstr_value.GetString());
    cstr_value.Format(L"%d", props.prop[REST_TIME_NUMBER].prop_value);
	GetDlgItem(IDC_restTime)->SetWindowTextW(cstr_value.GetString());
    CheckDlgButton(IDC_startAutomaticallyAtIntermittent, props.prop[START_AUTOMATICALLY_AT_INTERMITTENT_NUMBER].prop_value);
    CheckDlgButton(IDC_alertTenSecondsLeft, props.prop[ALERT_TEN_SECONDS_LEFT_NUMBER].prop_value);
    CheckDlgButton(IDC_startAndMinimize, props.prop[START_AND_MINIMIZE_MAIN_WINDOW_NUMBER].prop_value);

    if ( true == appFile.isConfFileExist() )
        GetDlgItem(IDC_Save)->EnableWindow(false);

    // The value of control above has been changed by system, it is no use to set m_controlChanged.
    m_controlChanged = false; 

    if ( true == IsTheAppStartWithWindows() )
        CheckDlgButton(IDC_startAutomaticallyAtWindowsLogon, true);

    screen.init(); // inintialize CScreen object. It will use this dialog as parent, so it needs to be initialized after this dialog has been initialized. 

    // Register global hot key.
    CString str_hot_key = AddGlobalHotKey(this->m_hWnd);
    if ( ! str_hot_key.IsEmpty() ) // add global hot key for starting screen saver.
    {
        str_hot_key = L"WARNING: Register Global Hot Key:  " + str_hot_key;
        str_hot_key = str_hot_key + L"Failed!";
        haveARest.showMessage(str_hot_key);
    }

    // Start and minimize the main window automatically if it needs.
    if ( props.prop[START_AND_MINIMIZE_MAIN_WINDOW_NUMBER].prop_value )
    {
        OnBnClickedStart();
        ShowWindow(SW_MINIMIZE); // hideMainWindow();
    }
}


void CHaveARestDlg::OnBnClickedStart()
{
    // TODO: Add your control notification handler code here
    // If the timer has been started and the control's status has not been changed, then drop out.
    if ( true == haveARest.getStartStatus() && false == m_controlChanged)
        return;

    if ( m_controlChanged )
    {
        haveARest.set();
    }

    // haveARest.set() will change the control's value, then system will send a control changed message, so this variable needs to be recovered here.
    m_controlChanged = false; 

    if ( global_pointer_progressctrl )
        global_pointer_progressctrl->SetRange(0, (props.prop[WORKING_TIME_NUMBER].prop_value)*60);
	haveARest.startTimer();
    GetDlgItem(IDC_Start)->SetWindowTextW(L"Start");
    haveARest.showMessage(L"Have A Rest has been started.");
    if ( props.prop[START_AND_MINIMIZE_MAIN_WINDOW_NUMBER].prop_value )
    {
        hideMainWindow();
    }
}


void CHaveARestDlg::OnBnClickedStop()
{
	if ( false == haveARest.getStartStatus() )
		return;

	haveARest.stopTimer();
    if ( global_pointer_progressctrl )
        global_pointer_progressctrl->SetPos(0);
    GetDlgItem(IDC_Status)->SetWindowTextW(L"IDLING");
    GetDlgItem(IDC_Start)->SetWindowTextW(L"Start");
    GetDlgItem(IDC_RemainingTimeText)->SetWindowTextW(TIME_INITIAL_STRING);
    haveARest.showMessage(L"Have A Rest has been stopped.");
}


void CHaveARestDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    if (haveARest.getStartStatus() &&  IDNO == MessageBox(L"EXIT?", L"Have A Rest", MB_YESNO|MB_ICONQUESTION) )
        return;

    DeleteGlobalHotKey(this->m_hWnd);
    OnBnClickedStop();
    CDialogEx::OnCancel();
    //exit(0);
}


void CHaveARestDlg::OnBnClickedAbout()
{
    // TODO: Add your control notification handler code here
    CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CHaveARestDlg::OnBnClickedApply()
{
    // Add or remove HKEY value into or from regedit.
    CString str_info = L"";
    if ( BST_CHECKED == IsDlgButtonChecked(IDC_startAutomaticallyAtWindowsLogon) )
    {
        if ( true == IsTheAppStartWithWindows() )
            return;

        StartWithWindows(str_info);
        haveARest.showMessage(str_info);
    }
    else
    {
        if ( false == IsTheAppStartWithWindows() )
            return;

        DontStartWithWindows(str_info);
        haveARest.showMessage(str_info);
    }
}

BOOL CHaveARestDlg::PreTranslateMessage(MSG* pMsg)
{
    if( pMsg->message == WM_KEYUP )
    {
        switch( pMsg->wParam )
        {
        case VK_ESCAPE:
            break;
        case VK_RETURN:
            if ( IsWindowVisible() )
            {
                GetDlgItem(IDC_Start)->SetFocus();
                OnBnClickedStart();
            }
            break;
        }
    }

    return 0;
}   

void CHaveARestDlg::OnControlChanged()
{
    m_controlChanged = true;
    if ( true == haveARest.getStartStatus() )
        GetDlgItem(IDC_Start)->SetWindowTextW(L"Restart");

    // enable save button
    GetDlgItem(IDC_Save)->EnableWindow(true);
}


void CHaveARestDlg::OnBnClickedstartscreensaver()
{
    // TODO: Add your control notification handler code here
    if ( true == screen.getScreenStartedStatus() )
       return;

    haveARest.interruptTimer();
    haveARest.startScreenSaverTimer();
    hideMainWindow();
}


LONG CHaveARestDlg::OnHotKey(WPARAM wParam,LPARAM lParam)
{
    UINT modifier = (UINT) LOWORD(lParam);
    UINT virtual_key = (UINT) HIWORD(lParam);

    if ( GLOBAL_HOT_KEY_MODIFIER == modifier )
    {
        if ( GLOBAL_HOT_KEY_SCREEN_SAVER_VIRTUAL_KEY == virtual_key )
        {
            if ( screen.getScreenStartedStatus() )
                haveARest.interruptTimer();
            else
                OnBnClickedstartscreensaver();
        }
        else if ( GLOBAL_HOT_KEY_MAIN_WINDOW_VIRTUAL_KEY == virtual_key )
        { 
            if ( ! IsWindowVisible() ) //IsIconic()
                showMainWindow();
            else
                hideMainWindow();
        }
    }

    return 0;
}

// AW_ACTIVATE AW_BLEND AW_CENTER AW_HIDE AW_HOR_POSITIVE AW_HOR_NEGATIVE AW_SLIDE AW_VER_POSITIVE AW_VER_NEGATIVE
void CHaveARestDlg::showMainWindow(HWND hwnd)
{
    if ( NULL == hwnd )
    {
        ShowWindow(SW_SHOW);
        SetForegroundWindow();
        //AnimateWindow(ONE_SECOND, AW_ACTIVATE|AW_BLEND);
    }
    else
    {
        ::ShowWindow(hwnd, SW_SHOW);
        ::SetForegroundWindow(hwnd);
    }
}

void CHaveARestDlg::hideMainWindow()
{
      ShowWindow(SW_HIDE); // SW_MINIMIZE
      //AnimateWindow(ONE_SECOND, AW_HIDE|AW_BLEND);
}


void CHaveARestDlg::OnBnClickedSave()
{
    haveARest.set();
    m_controlChanged = false; 
    if ( true == haveARest.handleOfConfFile(false, true) )
    {
        haveARest.showMessage(L"Save configuration to file successfully.");
        GetDlgItem(IDC_Save)->EnableWindow(false);
    }
    else
    {
        haveARest.showMessage(L"WARNING: Save configuration to file failed!");
    }
}
