#include"StdAfx.h"
#include"C_Timer.h"
#include"HaveARest.h"
#include"Screen.h"
#include"HaveARestDlg.h"

int global_int_rest_time;

// MarshalTimer -> WorkingTimer -> RestTimer(killed by MarshalTimer or itself) 
VOID CALLBACK MarshalTimer ( HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
    unsigned int workingTime = (props.prop[WORKING_TIME_NUMBER].prop_value) *60* 1000;
    haveARest.clearElapseSecond();
    SetTimer(hwnd, WORKING_TIMER, workingTime, WorkingTimer);
    SetTimer(hwnd, TALLY_WORKING_TIMER, ONE_SECOND, TallyWokingTimer);
}

VOID CALLBACK WorkingTimer ( HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
    KillTimer(hwnd, WORKING_TIMER);
    KillTimer(hwnd, TALLY_WORKING_TIMER);
    global_int_rest_time = (props.prop[REST_TIME_NUMBER].prop_value)*60;
    SetTimer(hwnd, REST_TIMER, ONE_SECOND, RestTimer);
    SetTimer(hwnd, TALLY_REST_TIMER, ONE_SECOND, TallyRestTimer);
}

VOID CALLBACK RestTimer( HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
    if ( 0 < global_int_rest_time )
    {   
        screen.start(global_int_rest_time);
    }
    else
    {
        if ( false == props.prop[START_AUTOMATICALLY_AT_INTERMITTENT_NUMBER].prop_value )
            haveARest.stopTimer();
        else
            haveARest.interruptTimer();
    }
}

VOID CALLBACK TallyWokingTimer ( HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
    haveARest.countElapseSecond();
    haveARest.informStatusPerSecond();
}

VOID CALLBACK TallyRestTimer ( HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
    global_int_rest_time--;
    haveARest.informStatusPerSecond(global_int_rest_time, true);
    if ( 0 >= global_int_rest_time )
    {
        KillTimer(hwnd, TALLY_REST_TIMER);
    }
}

VOID CALLBACK ScreenSaverTimer ( HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{   // Keep the value to 0. Since RestTimer may start periodically.
    global_int_rest_time = 0;
    screen.start(0);
}

