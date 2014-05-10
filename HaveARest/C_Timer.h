#define MARSHAL_TIMER 1   // start every working_time+rest_time
#define WORKING_TIMER 2  // start by MARSHAL_TIMER, when working_time is out.
#define REST_TIMER 3          // start by WORKING_TIMER. It will run every seconds, until rest_time is out.
#define TALLY_WORKING_TIMER 4         // start automatically every intermittent. It will be killed by working timer and started by marshal timer.
#define TALLY_REST_TIMER 5  // This timer will update the rest status periodically.
#define SCREEN_SAVER_TIMER 6
#define ONE_SECOND 1000

VOID CALLBACK MarshalTimer ( HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
VOID CALLBACK WorkingTimer( HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
VOID CALLBACK RestTimer( HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
VOID CALLBACK TallyWokingTimer ( HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
VOID CALLBACK TallyRestTimer ( HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
VOID CALLBACK ScreenSaverTimer ( HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);

extern int global_int_rest_time;
