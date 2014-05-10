#include"StdAfx.h"
#include"Screen.h"
#include"CommonAPI.h"
#include"HaveARest.h"
#include"HaveARestDlg.h"
#include"C_Timer.h"

const wchar_t WINDOW_CLASS_NAME[] = L"Screen Saver Window";

#define RGB_MUTABLE             RGB(RGB_R, RGB_G, RGB_B)
short int RGB_R = 0; // the value of monochrome  is in range of 0~255. it may be enough to assign 8-bit to the variable.
short int RGB_G = 120;
short int RGB_B = 200;
unsigned short int OPERATE_FLAG = 0x0111; // 0x01__ represent for red plus, 0x0__ represent for red minus; 0x0_1_ represent for green plus,
                                                                     // 0x0_0_ represent for green minus; 0x0__1 represent for blue plus, 0x0__0 represent for blue minus.

CScreen screen;

BEGIN_MESSAGE_MAP(CScreen, CWnd)  // Message map is not effect.
    ON_BN_CLICKED(ID_QUIT_BUTTON, &CScreen::OnBnClickedQuit)
END_MESSAGE_MAP()

void CScreen::init()
{
    m_int_screen_x = GetSystemMetrics(SM_CXSCREEN);
    m_int_screen_y = GetSystemMetrics(SM_CYSCREEN);

    //ZeroMemory(&m_windowClass, sizeof(WNDCLASSEX));
    m_windowClass.style = 0;
    m_windowClass.lpfnWndProc = WindowProcedure;
    m_windowClass.cbClsExtra = 0;
    m_windowClass.cbWndExtra = 0;
    m_windowClass.hInstance = m_hInstance;
    m_windowClass.hIcon = NULL;
    m_windowClass.hCursor = LoadCursor(m_hInstance, IDC_ARROW); // IDC_WAIT  IDC_APPSTARTING
    m_windowClass.hbrBackground = (HBRUSH)(GetStockObject(BLACK_BRUSH));
    m_windowClass.lpszClassName = NULL;
    m_windowClass.lpszClassName = WINDOW_CLASS_NAME;

    RegisterClass(&m_windowClass);
    m_hWnd = CreateWindowEx(
                      WS_EX_TOPMOST|WS_EX_TOOLWINDOW,  // DWORD dwExStyle  
                      WINDOW_CLASS_NAME,  // LPCTSTR lpClassName
                      NULL,                               // LPCTSTR lpWindowName
                      WS_POPUP,                     // DWORD dwStyle
                      0,                                     // int x
                      0,                                     // int y
                      m_int_screen_x,                                 // int nWidth
                      m_int_screen_y,                                 // int nHeight
                      NULL,                              // HWND hWndParent
                      NULL,                              // HMENU hMenu
                      m_hInstance,                   // HINSTANCE hInstance
                      NULL);                             // LPVOID lpParam
    if ( NULL == m_hWnd )
    {
        MessageBoxError(L"Error occurs when create window! Please Restart this program.");
        exit(0);
    }

    m_dc.Attach(::GetDC(m_hWnd));
    m_brush_black.CreateSolidBrush(RGB(0, 0, 0)); // the screen saver is black background.

    // Create font.
    m_font_big.CreateFont(
          24,                                                             // nHeight
          12,                                                               // nWidth
          0,                                                               // nEscapement
          0,                                                               // nOrientation
          FW_BOLD,                                                  // nWeight: FW_REGULAR, FW_NORMAL, FW_BOLD, FW_DONTCARE
          FALSE,                                                        // bItalic
          FALSE,                                                        // bUnderline
          0,                                                               // cStrikeOut
          ANSI_CHARSET,                                          // nCharSet
          OUT_DEFAULT_PRECIS,                               // nOutPrecision
          CLIP_DEFAULT_PRECIS| CLIP_LH_ANGLES,   // nClipPrecision: CLIP_LH_ANGLES
          DEFAULT_QUALITY,                                    // nQuality
          DEFAULT_PITCH | FF_SWISS,                       // nPitchAndFamily
          L"Arial");                                                     // lpszFacename

    m_font_small.CreateFont(
        14,
        0,
        0,
        0,
        FW_REGULAR,
        FALSE,
        FALSE,
        0,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH,
        L"Arial");

    m_dc.SetTextColor(RGB_SKY_BLUE);         //0xB48246, 0xE16941, RGB(200, 180, 123)
    m_dc.SetBkMode(TRANSPARENT);
    m_dc.SelectObject(&m_font_big);

    CRect quit_button_rect;
    //ICONMETRICS iconmetrics;
    //iconmetrics.cbSize = sizeof(ICONMETRICS);
    //if ( SystemParametersInfo(SPI_GETICONMETRICS, iconmetrics.cbSize, &iconmetrics, 0) )
    //{
    //    quit_button_rect.left = m_int_screen_x - iconmetrics.iHorzSpacing/2;
    //    quit_button_rect.top = m_int_screen_y - iconmetrics.iVertSpacing/2;
    //}
    //else
    //{
        quit_button_rect.left = m_int_screen_x - SM_CXICON-6;
        quit_button_rect.top = 0;
    //}
    quit_button_rect.right = m_int_screen_x;
    quit_button_rect.bottom =  SM_CYICON+6;
    m_quit_button.Create(NULL, WS_CHILD|BS_PUSHLIKE|BS_ICON|BS_DEFPUSHBUTTON, 
        quit_button_rect, CWnd::FromHandle(m_hWnd), ID_QUIT_BUTTON); // |WS_TABST
    if ( NULL == m_quit_button.GetIcon() )
       m_quit_button.SetIcon(::LoadIcon(NULL, IDI_ERROR));
    
    GetTimeZoneInformation(&m_time_zone);
}

void CScreen::start(const unsigned int duration_time)
{
    if ( 0 > duration_time )
        return;

    ChangeRGBValue(RGB_R, RGB_G, RGB_B, OPERATE_FLAG);
    m_dc.SetTextColor(RGB_MUTABLE);
    showScreenSaverWindow();

    if ( 0 == duration_time )
        return;

    CString str_format_time(L"");
    FormatTime(duration_time, str_format_time);
    m_size_text = m_dc.GetTextExtent(str_format_time);
    m_point_text.x = m_int_screen_x/2-m_size_text.cx/2;
    m_point_text.y = m_int_screen_y/2-m_size_text.cy/2;

    m_dc.FillRect(CRect(m_point_text.x, m_point_text.y, m_point_text.x+m_size_text.cx, m_point_text.y+m_size_text.cy), &m_brush_black);
    m_dc.TextOut(m_point_text.x, m_point_text.y, str_format_time);
}

void CScreen::end()
{
    //::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
    ::SetForegroundWindow(m_lastWindowhWnd);
    ::SetActiveWindow(m_lastWindowhWnd);
    ::SetFocus(m_lastWindowhWnd);
    ::AnimateWindow(m_hWnd, ONE_SECOND, AW_BLEND|AW_HIDE);   //::ShowWindow(m_hWnd, SW_HIDE);
    m_screenStarted = false;
}

void CScreen::cease()
{
    end();

    m_font_big.DeleteObject();
    m_font_small.DeleteObject();
    m_brush_black.DeleteObject();
    m_dc.Detach();
    m_dc.DeleteDC();

    ::DestroyWindow(m_hWnd);
    UnregisterClass(WINDOW_CLASS_NAME, m_hInstance);
}

void CScreen::showStaticMessage()
{
    CString quit_message;
    quit_message.Format(L"%c", GLOBAL_HOT_KEY_SCREEN_SAVER_VIRTUAL_KEY);
    quit_message = L"and " + quit_message;
    quit_message += L" to quit";
    if ( MOD_SHIFT & GLOBAL_HOT_KEY_MODIFIER )
    {
        quit_message = L"SHIFT " + quit_message;
    }
    if ( MOD_ALT & GLOBAL_HOT_KEY_MODIFIER )
    {
        quit_message = L"ALT " + quit_message;
    }
    if ( MOD_CONTROL & GLOBAL_HOT_KEY_MODIFIER )
    {
        quit_message = L"Press CTRL " + quit_message;
    }
    else
    {
        quit_message = L"Press " + quit_message;
    }
    m_dc.TextOut(0, 0, L"Have A Rest");
    m_dc.SetTextColor(RGB_GREY);
    m_dc.SelectObject(&m_font_small);
    m_size_text = m_dc.GetTextExtent(quit_message);
    m_dc.TextOut(0, m_int_screen_y-m_size_text.cy, quit_message);
    m_dc.SetTextColor(RGB_MUTABLE);
    m_dc.SelectObject(&m_font_big);
}

void CScreen::showLocalTime()
{
    CString str_format_time(L"");
    CRect rect_local_time;
    str_format_time.Format(L"%02d:%02d", m_local_time.wHour, m_local_time.wMinute);
    str_format_time = str_format_time + L" ";
    //str_format_time += m_time_zone.StandardName;
    m_size_text = m_dc.GetTextExtent(str_format_time);
    rect_local_time.left = m_int_screen_x - m_size_text.cx;
    rect_local_time.top = m_int_screen_y - m_size_text.cy;
    rect_local_time.right = m_int_screen_x;
    rect_local_time.bottom = m_int_screen_y;
    m_dc.FillRect(rect_local_time, &m_brush_black);
    m_dc.TextOut(rect_local_time.left, rect_local_time.top, str_format_time);
}

void CScreen::showScreenSaverWindow()
{
    GetLocalTime(&m_local_time);

    if ( false == m_screenStarted )
    {
        m_lastWindowhWnd = ::GetForegroundWindow();
        ::AnimateWindow(m_hWnd, ONE_SECOND*5, AW_BLEND); //ShowWindow(m_hWnd, SW_SHOW);
        m_quit_button.ShowWindow(SW_SHOW);
        showStaticMessage();
        showLocalTime();
        ::UpdateWindow(m_hWnd);
        m_screenStarted = true;
    }

    if ( 0 == m_local_time.wSecond )
    {
        showLocalTime();
    }

    // Hide mouse
    if ( (0 == m_local_time.wSecond%10) && GetCursor() )
    {
        SetCursor(NULL);
    }

    ::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
    ::SetForegroundWindow(m_hWnd);
    ::SetActiveWindow(m_hWnd);
    ::SetFocus(m_hWnd);
}

void CScreen::showDynamicMessage(CString &message)
{
    m_dc.SetTextColor(RGB_GREY);
    m_dc.SelectObject(&m_font_small);
    m_size_text = m_dc.GetTextExtent(message);
    m_dc.FillRect(CRect(0, m_int_screen_y/2, m_size_text.cx, m_int_screen_y/2+m_size_text.cy), &m_brush_black);
    m_dc.TextOut(0, m_int_screen_y/2, message);
    m_dc.SetTextColor(RGB_MUTABLE);
    m_dc.SelectObject(&m_font_big);
}

void CScreen::showDynamicMessage(const CString &message, int position_x, int position_y)
{
    m_dc.SetTextColor(RGB_GREY);
    m_dc.SelectObject(&m_font_small);
    m_size_text = m_dc.GetTextExtent(message);
    m_dc.FillRect(CRect(position_x, position_y, position_x + m_size_text.cx, position_y + m_size_text.cy), &m_brush_black);
    m_dc.TextOut(position_x, position_y, message);
    m_dc.SetTextColor(RGB_MUTABLE);
    m_dc.SelectObject(&m_font_big);
}

void CScreen::OnBnClickedQuit()
{
    // The message of ON_BN_CLICKED is not effective.
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_KEYUP:
        {
            switch(wParam)
            {
            case VK_ESCAPE:
                haveARest.interruptTimer();
                break;

           //UINT modifier = (UINT) LOWORD(wParam);
           //UINT virtual_key = (UINT) HIWORD(wParam);
            // The global hot key will replace this combination key here.
            //case GLOBAL_HOT_KEY_SCREEN_SAVER_VIRTUAL_KEY:
            //    if ( (GetAsyncKeyState( VK_LCONTROL ) & 0x8000) && (GetAsyncKeyState( VK_SHIFT ) & 0x8000)  ) //
            //        haveARest.interruptTimer();
            //        //MessageBoxInfo(L"slfjk");
            //    break;

            default:
                break;
            }
        }
        break;
    case WM_CREATE:
        break;
    case WM_PAINT:
        screen.showStaticMessage();
        break;
    case WM_COMMAND:
        if ( (BN_CLICKED == HIWORD(wParam)) && (ID_QUIT_BUTTON == LOWORD(wParam)) )
        {
            haveARest.interruptTimer();
            //CString str_id; str_id.Format(L"%d", LOWORD(wParam));
            //TestFunction(str_id); // When click quit button, this function will be invoked.
        }
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    if ( GetAsyncKeyState( VK_MENU ) && ( VK_F4 == wParam ) ) // GetAsyncKeyState should be called high priority.
    {
        haveARest.interruptTimer();
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


   