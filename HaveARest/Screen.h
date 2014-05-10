#define RGB_BLUE                    RGB(135, 206, 235)
#define RGB_LIGHT_GREY         RGB(211, 211, 211)
#define RGB_GREY                    RGB(190, 190, 190)
#define RGB_ROYAL_BLUE         RGB(58, 95, 205)
#define RGB_SKY_BLUE             RGB(154, 192, 200)
#define CAPTION_QUIT_BUTTON L"QUIT"
#define ID_QUIT_BUTTON 2000

class CScreen: public CWnd
{
private:
    int m_int_screen_x;
    int m_int_screen_y;
    HINSTANCE m_hInstance;
    WNDCLASS m_windowClass;
    HWND m_hWnd;
    CDC m_dc;    
    CBrush m_brush_black;
    CButton m_quit_button;
    CFont m_font_big;
    CFont m_font_small;
    TIME_ZONE_INFORMATION m_time_zone;
    bool m_screenStarted;

    // The folllowing variables are mutable.
    CSize m_size_text;
    POINT m_point_text;
    SYSTEMTIME m_local_time;
    HWND m_lastWindowhWnd;

public:
    CScreen(): m_hInstance(NULL), m_lastWindowhWnd(NULL), m_screenStarted(FALSE) { }
    ~CScreen(){ end(); cease(); }
    void init();
    void start(const unsigned int duration_time);
    void end();
    void cease();
    bool getScreenStartedStatus() const { return m_screenStarted; }
    void showStaticMessage();
    void showLocalTime();
    void showScreenSaverWindow();
    void showDynamicMessage(CString &message);
    void showDynamicMessage(const CString &message, int position_x = 50, int position_y = 0);
    afx_msg void OnBnClickedQuit();

    DECLARE_MESSAGE_MAP()
};

extern CScreen screen;

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

