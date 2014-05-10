#include<string>
#include<fstream>
#include<Windows.h>
#include<TlHelp32.h>

#define HKEY_NAME HKEY_CURRENT_USER
#define HKEY_SUBKEY L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define HKEY_VALUE_NAME L"HaveARest"
#define HKEY_VALUE_TYPE REG_SZ
#define MAX_MINUTES  1440000
#define GLOBAL_HOT_KEY_SCREEN_SAVER_STRING L"Have A Rest Screen Saver"
#define GLOBAL_HOT_KEY_MAIN_WINDOW_STRING L"Have A Rest Main Window"
#define GLOBAL_HOT_KEY_SCREEN_SAVER_VIRTUAL_KEY ('S') //0x53 S key
#define GLOBAL_HOT_KEY_MAIN_WINDOW_VIRTUAL_KEY ('W') 
#define GLOBAL_HOT_KEY_MODIFIER ( MOD_ALT ) //MOD_SHIFT MOD_CONTROL MOD_NOREPEAT MOD_SHIFT MOD_WIN

void MessageBoxWarning(const CString &message);
void MessageBoxInfo(const CString &message);
void MessageBoxError(const CString &message);
void FormatTime(const int time, CString &string_time);
bool IsTheAppStartWithWindows();
bool StartWithWindows(CString &str_info);
bool DontStartWithWindows(CString &str_info);
void ComputeRemainingTime(const int elapse_time, const int total_time, CString &string_time);
void CheckStringTimeValidity(CString &str_number, int &time);
void CheckStringTimeValidity(std::string &str_number, int &time);
CString AddGlobalHotKey(HWND hwnd);
void DeleteGlobalHotKey(HWND hwnd);
void ChangeRGBValue(short int &red, short int &green, short int &blue, unsigned short int &flag);
bool IsStringMatched(const std::string pattern, const std::string target);
void ConvertStringToUpper(std::string &str);
void TrimString(std::string &str);
bool ReadDataFromFile(const CString &file_path, std::string &str_file_data);
bool SaveDataToFile(const CString &file_path, const std::string &str_data);
bool IsAppHasRun(PCWSTR processName);

