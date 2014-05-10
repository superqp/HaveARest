#include<string>

#include "Properties.h"

// HaveARest.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// CHaveARestApp:
// See HaveARest.cpp for the implementation of this class
//

class CHaveARestApp : public CWinApp
{
private:
    bool m_started; // Index whether the Timer has started.
    int m_uint_elapse_second; // store the elapse seconds since the working timer started.
    int m_maxListBoxScrollWidth;

public:
	CHaveARestApp();
    ~CHaveARestApp() { stopTimer(); }
    void set();
    bool getStartStatus(){ return m_started; }
    const int getElapseSecond() const {  return m_uint_elapse_second; }
    bool startTimer();
    void startScreenSaverTimer();
    void interruptTimer(); // Interrupt current intermittent. Not impact next intermittent.
    bool stopTimer();
	void restartTimer();
    int showMessage(CString cstring, int index = -1);
    void showProgressBar(unsigned int uint_a);
    void showTimeText(int time = 0);
    void countElapseSecond();
    void clearElapseSecond(); // When a new intermittent has been started, this function will be called.
    void informStatusPerSecond(int time = 0, bool rest = false);
    bool handleOfConfFile(bool assigning, bool saving = false);
    void convergePropsToString(std::string &str_props);
    bool isPropertyMatched(std::string &prop_name, int &prop_number);

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CHaveARestApp haveARest;

struct _property_node_
{
    const std::string prop_name;
    int prop_value;
};

typedef struct _property_set_
{
    unsigned int unvisited_bits;
    struct _property_node_ prop[PROPERTY_AMOUNT];
}properties;

extern properties props;

