
// HaveARest.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "HaveARest.h"
#include "HaveARestDlg.h"
#include "CommonAPI.h"
#include "C_Timer.h"
#include "Screen.h"
#include "AppFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHaveARestApp

BEGIN_MESSAGE_MAP(CHaveARestApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHaveARestApp construction

CHaveARestApp::CHaveARestApp(): m_started(FALSE), m_uint_elapse_second(0), m_maxListBoxScrollWidth(0)
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CHaveARestApp object

CHaveARestApp haveARest;

properties props = { 0x0000000000011111, // unvisited_bits
                                     { //prop: prop_name, prop_value
                                      {WORKING_TIME_PROP, WORKING_TIME_DEF},
                                      {REST_TIME_PROP, REST_TIME_DEF},
                                      {START_AUTOMATICALLY_AT_INTERMITTENT_PROP, START_AUTOMATICALLY_AT_INTERMITTENT_DEF},
                                      {ALERT_TEN_SECONDS_LEFT_PROP, ALERT_TEN_SECONDS_LEFT_DEF},
                                      {START_AND_MINIMIZE_MAIN_WINDOW_PROP, START_AND_MINIMIZE_MAIN_WINDOW_DEF}
                                     }
                                    };

// CHaveARestApp initialization

BOOL CHaveARestApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CHaveARestDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CHaveARestApp::set()
{
    if( NULL == global_pointer_cwnd )
        return;

    CString string_temp;

    // Set working time. //time*60  -> second.
    global_pointer_cwnd->GetDlgItem(IDC_workingTime)->GetWindowText(string_temp); 
    CheckStringTimeValidity(string_temp, props.prop[WORKING_TIME_NUMBER].prop_value);
    global_pointer_cwnd->GetDlgItem(IDC_workingTime)->SetWindowText(string_temp);

    // Set rest time.
    global_pointer_cwnd->GetDlgItem(IDC_restTime)->GetWindowTextW(string_temp);
    CheckStringTimeValidity(string_temp, props.prop[REST_TIME_NUMBER].prop_value);
    global_pointer_cwnd->GetDlgItem(IDC_restTime)->SetWindowText(string_temp);

    // Set starting variable.
    if ( BST_CHECKED == global_pointer_cwnd->IsDlgButtonChecked(IDC_startAutomaticallyAtIntermittent) )
        props.prop[START_AUTOMATICALLY_AT_INTERMITTENT_NUMBER].prop_value = true;
    else
        props.prop[START_AUTOMATICALLY_AT_INTERMITTENT_NUMBER].prop_value = false;

    if ( BST_CHECKED == global_pointer_cwnd->IsDlgButtonChecked(IDC_alertTenSecondsLeft) )
        props.prop[ALERT_TEN_SECONDS_LEFT_NUMBER].prop_value = true;
    else
        props.prop[ALERT_TEN_SECONDS_LEFT_NUMBER].prop_value = false;

    if ( BST_CHECKED == global_pointer_cwnd->IsDlgButtonChecked(IDC_startAndMinimize) )
        props.prop[START_AND_MINIMIZE_MAIN_WINDOW_NUMBER].prop_value = true;
    else
        props.prop[START_AND_MINIMIZE_MAIN_WINDOW_NUMBER].prop_value = false;
}

//When start button is pressed, this function will be invoked. This function will start 
// startTimer() -> MARSHAL_TIMER -> WORKING_TIMER(temporary, killed by itself) -> REST_TIMER(killed by itself)
bool CHaveARestApp::startTimer()
{
    if ( m_started )
        stopTimer();

    if ( NULL == global_pointer_cwnd )
        return false;

	bool status = true;
    unsigned int marshalTime = (props.prop[WORKING_TIME_NUMBER].prop_value + props.prop[REST_TIME_NUMBER].prop_value) *60*1000;
    unsigned int workingTime = (props.prop[WORKING_TIME_NUMBER].prop_value) *60* 1000;

    if ( props.prop[START_AUTOMATICALLY_AT_INTERMITTENT_NUMBER].prop_value )
    {
	    SetTimer(global_pointer_cwnd->m_hWnd, MARSHAL_TIMER, marshalTime, MarshalTimer); 
    }
    SetTimer(global_pointer_cwnd->m_hWnd, WORKING_TIMER, workingTime, WorkingTimer);
    SetTimer(global_pointer_cwnd->m_hWnd, TALLY_WORKING_TIMER, ONE_SECOND, TallyWokingTimer);
    clearElapseSecond();

    m_started = true;
    
	return status;
}

void CHaveARestApp::startScreenSaverTimer()
{
    SetTimer(global_pointer_cwnd->m_hWnd, SCREEN_SAVER_TIMER, ONE_SECOND, ScreenSaverTimer);
}

void CHaveARestApp::interruptTimer()
{
    if ( global_pointer_cwnd )
    {
        KillTimer(global_pointer_cwnd->m_hWnd, REST_TIMER);
        KillTimer(global_pointer_cwnd->m_hWnd, SCREEN_SAVER_TIMER);
    }
    screen.end();
}

bool CHaveARestApp::stopTimer()
{
	bool status = true;

	// If the timer is not running, KillTimer() will return false.
    if (global_pointer_cwnd)
    {
	    KillTimer(global_pointer_cwnd->m_hWnd, MARSHAL_TIMER);
	    KillTimer(global_pointer_cwnd->m_hWnd, WORKING_TIMER);
        KillTimer(global_pointer_cwnd->m_hWnd, TALLY_WORKING_TIMER);
        KillTimer(global_pointer_cwnd->m_hWnd, REST_TIMER);
        KillTimer(global_pointer_cwnd->m_hWnd, SCREEN_SAVER_TIMER);
    }
    clearElapseSecond();
    screen.end();
    m_started = false;

	return status;
}

void CHaveARestApp::restartTimer()
{
	stopTimer();
	startTimer();
}

int CHaveARestApp::showMessage(CString cstring,  int index)
{
    if ( NULL == global_pointer_cwnd || NULL == global_pointer_listbox_message ) // && IsIconic(global_pointer_cwnd->m_hWnd)
        return -1;

    int iRet = -1;
    int iLastIndex = global_pointer_listbox_message->GetCount();
    if ( 0 > index || iLastIndex == index )
    {
       global_pointer_listbox_message->InsertString(iLastIndex, cstring);
       global_pointer_listbox_message->SetCaretIndex(iLastIndex-1);
       iRet = iLastIndex;
    }
    else if ( iLastIndex > index )
    {
        global_pointer_listbox_message->DeleteString(index);
        global_pointer_listbox_message->InsertString(index, cstring);
        iRet = index;
    }

    // Show horizontal scroll bar
    CClientDC cdc(global_pointer_cwnd);
	SIZE size;
	int scroll_width;
	scroll_width=GetSystemMetrics(SM_CXVSCROLL);
	GetTextExtentPoint(cdc.m_hDC, cstring, cstring.GetLength(), &size);
	scroll_width=max(scroll_width, (int)size.cx);
    m_maxListBoxScrollWidth=max(scroll_width, m_maxListBoxScrollWidth);
    global_pointer_cwnd->SendDlgItemMessage(IDC_Message, LB_SETHORIZONTALEXTENT, m_maxListBoxScrollWidth, 0);

    return iRet;
}

void CHaveARestApp::showProgressBar(unsigned int uint_a)
{
    if ( uint_a < 0 )
        return ;
    if ( global_pointer_progressctrl )
       global_pointer_progressctrl->SetPos(uint_a);
}

void CHaveARestApp::showTimeText(int time)
{
    if ( time < 0 )
        return;
    if ( global_pointer_cwnd )
    {
        CString str_time;
        FormatTime(time, str_time);
        global_pointer_cwnd->GetDlgItem(IDC_RemainingTimeText)->SetWindowTextW(str_time);
    }
}

void CHaveARestApp::countElapseSecond()
{
    m_uint_elapse_second++;
}

 void CHaveARestApp::clearElapseSecond()
 {
     m_uint_elapse_second = 0;
 }

 void CHaveARestApp::informStatusPerSecond(int time, bool rest)
 {
     if ( (NULL == global_pointer_cwnd) || ! IsWindowVisible(global_pointer_cwnd->m_hWnd) || IsIconic(global_pointer_cwnd->m_hWnd) ) // If the window is minized, it is no need to show the tatus.
        return ;

     // The status is rest.
     if ( true == rest )
     {
        if( global_pointer_cwnd )
        {
            global_pointer_cwnd->GetDlgItem(IDC_Status)->SetWindowText(L"RESTING");
            showTimeText(time);
            showProgressBar(0);
        }
         return;
     }

     // The status is working.
     if ( (10 == ((props.prop[WORKING_TIME_NUMBER].prop_value)*60 - m_uint_elapse_second)) 
         && (true == props.prop[ALERT_TEN_SECONDS_LEFT_NUMBER].prop_value) )
         MessageBeep(MB_ICONINFORMATION); //MB_ICONWARNING  MB_ICONASTERISK, 0xFFFFFFFF After queuing the sound, the MessageBeep function returns control 
                                                                     // to the calling function and plays the sound asynchronously.

     showProgressBar( (props.prop[WORKING_TIME_NUMBER].prop_value)*60 - m_uint_elapse_second );

     CString string_time;
     ComputeRemainingTime(m_uint_elapse_second, (props.prop[WORKING_TIME_NUMBER].prop_value)*60, string_time);
     global_pointer_cwnd->GetDlgItem(IDC_RemainingTimeText)->SetWindowTextW(string_time);
     global_pointer_cwnd->GetDlgItem(IDC_Status)->SetWindowText(L"WORKING");
 }
 
 /*
  * This function will handle conf file, assigning(obtaining) or saving. Saving is not enabled by default.
  */
 bool CHaveARestApp::handleOfConfFile(bool assigning, bool saving)
 {
     // Set the visited bits, so make sure all the properties will be properly visited.
     props.unvisited_bits = DEFAULT_UNVISITED_BITS;

     using namespace std;
     string str_file_data;
     string str_file_data_saving;

     str_file_data.clear();
     str_file_data_saving.clear();
     ReadDataFromFile((appFile.getConfPathName()).GetString(), str_file_data);
     // If read conf file failed, it needs no going further step.
     if ( str_file_data.empty() )
     {
         //haveARest.showMessage(L"Read conf file failed!");
         if ( false == saving )
            return false;
     }
     else
     {
        // append a LINE_FEED at the last of the string.
        // If the last character of string data is not LINE_FEED, the loop will break, that is it will not handle the last line which not contains LINE_FEED.
         int i_length = str_file_data.length();
         if ( 0 < i_length && LINE_FEED != str_file_data[i_length-1] )
        {
             str_file_data.append(1, LINE_FEED);
        }
        long int old_pos = 0;
        long int new_pos = 0;
        string str_line;
        i_length = str_file_data.length();
        while( new_pos <= i_length )
        {
             // If all props has been visited, it needs no finding in string now.
            if ( 0 == props.unvisited_bits )
            {
                str_file_data_saving = str_file_data_saving + str_file_data.substr(old_pos);
                break;
            }

            // find LINE_FEED, and get line.
            new_pos = str_file_data.find(LINE_FEED, old_pos);
            if ( string::npos != new_pos )
            {   // Obtain substring by each line.
                str_line = str_file_data.substr(old_pos, new_pos - old_pos+1); 
            }
            else
            {
                break;
            }
            // Locate the position to the first position of next line. 
            // Also this will indicate the time the loop exits. So don't place it behind if-block which contains "continue".
            old_pos = new_pos+1;

            // Parse this substring, and check if it meets form prop_name: prop_value.
            string str_prop_name;
            string str_prop_value;
            static const std::basic_string <char>::size_type npos = -1;
            std::basic_string <char>::size_type separator_position = 0;
            separator_position = str_line.find_first_of(PROP_DELIMITER);
            if ( npos != separator_position ) // Has found delimiter.
            {
                str_prop_value = str_line.substr( separator_position+1 );
                str_prop_name = str_line.substr( 0, separator_position );
            }
            else // This substring is not a prop line. Skip it.
            {
                // Save the line which is not a prop line.
                str_file_data_saving = str_file_data_saving + str_line;
                continue;
            }

            int i_prop_number = -1;
            //Not matched. Skip it.
            if ( ! isPropertyMatched(str_prop_name, i_prop_number) )
            {
                // Save the line which is not matched by property name.
                str_file_data_saving = str_file_data_saving + str_line;
                continue;
            }

            if ( true == assigning )  // obtain value from string line
            {
                // Check the string value and assign it to property's value.
                int i_prop_value = 0;
                TrimString(str_prop_value);
                ConvertStringToUpper(str_prop_value);
                if ( 0 == str_prop_value.compare(PROP_VALUE_TRUE) )
	            {
		            i_prop_value = true;
	            }
	            else if ( 0 == str_prop_value.compare(PROP_VALUE_FALSE) )
	            {
		            i_prop_value = false;
	            }
                else
                {
                     CheckStringTimeValidity(str_prop_value, i_prop_value);
                }
                props.prop[i_prop_number].prop_value = i_prop_value;
            }
        }
     }

     // If it needs to save the modified file data, then converge all the properties, append file data, and save to file.
     if ( true == saving )
     {
         string str_converged_props;
         convergePropsToString(str_converged_props);
         str_file_data_saving = str_converged_props + str_file_data_saving;
         if ( !SaveDataToFile(appFile.getConfPathName(), str_file_data_saving) )
         {
             return false;
         }
     }

     return true;
 }

 void CHaveARestApp::convergePropsToString(std::string &str_props)
 {
     using namespace std;
     str_props.clear();

     int i_prop_number = 0;
     for( ; i_prop_number < PROPERTY_AMOUNT; i_prop_number++ )
     {
         str_props.append(props.prop[i_prop_number].prop_name);
         str_props.append(1, PROP_DELIMITER);
         str_props.append( to_string(_ULonglong(props.prop[i_prop_number].prop_value)) );
         str_props.append(1, LINE_FEED);
     }
 }

 bool CHaveARestApp::isPropertyMatched(std::string &prop_name, int &prop_number)
 {
     bool status = false;

    TrimString(prop_name);
    ConvertStringToUpper(prop_name);

    // Assign value to each property, according to its name.
    if ( (props.unvisited_bits & WORKING_TIME_UNVISITED_BIT)  && 0 == prop_name.compare(WORKING_TIME_PROP) )
    {
       //haveARest.showMessage(CString((prop_name+str_prop_value).c_str()));
       prop_number = WORKING_TIME_NUMBER;
       status = true;

       // Set the unvisited bit to zero. To indicate this prop has been visited.
       props.unvisited_bits = props.unvisited_bits & (~WORKING_TIME_UNVISITED_BIT);
    }
	else if ( (props.unvisited_bits & REST_TIME_UNVISITED_BIT)  && 0 == prop_name.compare(REST_TIME_PROP) )
    {
        //haveARest.showMessage(CString((prop_name+str_prop_value).c_str()));
        prop_number = REST_TIME_NUMBER;
        status = true;

        props.unvisited_bits = props.unvisited_bits & (~REST_TIME_UNVISITED_BIT);
    }
    else if ( (props.unvisited_bits & START_AUTOMATICALLY_AT_INTERMITTENT_UNVISITED_BIT)  && 
                 0 == prop_name.compare(START_AUTOMATICALLY_AT_INTERMITTENT_PROP) )
    {
        //haveARest.showMessage(CString((prop_name+str_prop_value).c_str()));
        prop_number = START_AUTOMATICALLY_AT_INTERMITTENT_NUMBER;
        status = true;

        props.unvisited_bits = props.unvisited_bits & (~START_AUTOMATICALLY_AT_INTERMITTENT_UNVISITED_BIT);
    }
    else if ( (props.unvisited_bits & ALERT_TEN_SECONDS_LEFT_UNVISITED_BIT)  && 
                0 == prop_name.compare(ALERT_TEN_SECONDS_LEFT_PROP) )
    {
        //haveARest.showMessage(CString((prop_name+str_prop_value).c_str()));
        prop_number = ALERT_TEN_SECONDS_LEFT_NUMBER;
        status = true;

        props.unvisited_bits = props.unvisited_bits & (~ALERT_TEN_SECONDS_LEFT_UNVISITED_BIT);
    }
    else if ( (props.unvisited_bits & START_AND_MINIMIZE_MAIN_WINDOW_UNVISITED_BIT)  && 
                 0 == prop_name.compare(START_AND_MINIMIZE_MAIN_WINDOW_PROP) )
   {
        //haveARest.showMessage(CString((prop_name+str_prop_value).c_str()));   
        prop_number = START_AND_MINIMIZE_MAIN_WINDOW_NUMBER;
        status = true;

        props.unvisited_bits = props.unvisited_bits & (~START_AND_MINIMIZE_MAIN_WINDOW_UNVISITED_BIT);
    }

     return status;
 }



