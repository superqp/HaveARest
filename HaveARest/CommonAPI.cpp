#include"StdAfx.h"
#include"CommonAPI.h"
#include"HaveARest.h"
#include"HaveARestDlg.h"
#include"AppFile.h"

#include<regex>
#include<functional>

ATOM global_id_screen_saver, global_id_main_window;

//uType: MB_ICONEXCLAMATION MB_ICONWARNING MB_ICONINFORMATION MB_ICONASTERISK MB_ICONQUESTION MB_ICONSTOP 
// MB_ICONERROR MB_ICONHAND
// MB_ABORTRETRYIGNORE MB_CANCELTRYCONTINUE MB_HELP MB_OK MB_OKCANCEL MB_RETRYCANCEL MB_YESNO MB_YESNOCANCEL
//Return value: IDABORT IDCANCEL IDCONTINUE IDIGNORE IDNO IDOK IDRETRY IDTRYAGAIN IDYES
void MessageBoxWarning(const CString &message)
{
	MessageBox((GetDesktopWindow()), message, L"WARNING", MB_OK|MB_ICONWARNING);
}

void MessageBoxInfo(const CString &message)
{
	MessageBox(GetDesktopWindow(), message, L"INFO", MB_OK|MB_ICONINFORMATION);
}

void MessageBoxError(const CString &message)
{
    MessageBox(GetDesktopWindow(), (LPCTSTR)(message), L"ERROR", MB_OK|MB_ICONERROR);
}

void FormatTime(const int time, CString &string_time)
{
    if ( time < 0 )
        return;

    CString string_temp = L"";
    int int_day;
    int int_hour;
    int int_minute;
    int int_second;
    int_day = time/(3600*24);
    int_hour = (time%(3600*24))/3600;
    int_minute = (time%3600)/60;
    int_second = time%60;

    if ( 0 < int_day )
    {
        string_temp.Format(L"%02d", int_day);
        string_time += string_temp;
        string_time += L"d ";
    }

    if ( 0 < int_hour || 0 < int_day)
    {
        string_temp.Format(L"%02d", int_hour);
        string_time += string_temp;
        string_time += L"h ";
    }

    string_temp.Format(L"%02d", int_minute);
    string_time +=string_temp;
    string_time += L"m ";

    string_temp.Format(L"%02d", int_second);
    string_temp += L"s";
    string_time += string_temp;
}

bool IsTheAppStartWithWindows()
{
    CString file_path, file_path_registry;
    DWORD path_size = MAX_PATH;
    int status;

    status = GetModuleFileNameW(NULL, file_path.GetBufferSetLength(MAX_PATH+1), MAX_PATH);
    file_path.ReleaseBuffer();
    if ( 0 == status )
        return false;
    
    status = RegGetValue( HKEY_NAME, HKEY_SUBKEY, HKEY_VALUE_NAME, RRF_RT_REG_SZ, NULL, 
        file_path_registry.GetBufferSetLength(MAX_PATH+1), &path_size);
    file_path_registry.ReleaseBuffer();
    if ( ERROR_SUCCESS != status )
        return false;

    if ( 0 != file_path.CompareNoCase(file_path_registry) )
        return false;

    return true;
}

bool StartWithWindows(CString &str_info)
{
    HKEY   registry_key = NULL;
    CString file_path = L"";
    int status;

    status = GetModuleFileNameW(NULL, file_path.GetBufferSetLength(MAX_PATH+1), MAX_PATH);
    file_path.ReleaseBuffer();
    if ( 0 == status )
    {
        str_info = L"ERROR: Get Module File Name unsuccessfully!";
        return false;
    }

    CFileFind file_find;
    status = file_find.FindFile(file_path);
    file_find.Close();
    if ( false == status )
    {
        str_info = L"ERROR: Can not find file: " + file_path;
        return false;
    }

    if ( ERROR_SUCCESS == RegOpenKeyExW(HKEY_NAME, HKEY_SUBKEY, 0, KEY_WRITE, &registry_key) )
    {
        // Unicode charset, so need to multiply 2.
        status = RegSetValueExW(registry_key, HKEY_VALUE_NAME, 0, HKEY_VALUE_TYPE, (const BYTE *)(file_path.GetString()), file_path.GetLength()*2); 
    }
    RegCloseKey(registry_key);
    if ( ERROR_SUCCESS != status )
    {
         str_info = L"ERROR: Add registry value unsuccessfully!";
         return false;
    }

    str_info = L"Start program with windows automatically: ";
    str_info += file_path;
    return true;
}

bool DontStartWithWindows(CString &str_info)
{
    if ( ERROR_SUCCESS == RegDeleteKeyValueW(HKEY_NAME, HKEY_SUBKEY, HKEY_VALUE_NAME) )
    {
        str_info = L"Dont start program with windows automatically.";
        return true;
    }
    str_info = L"ERROR: Remove registry value unsuccessfully!";
    return false;
}

void ComputeRemainingTime(const int elapse_time, const int total_time, CString &string_time)
{
    string_time = L"";

    if ( elapse_time > total_time ) // elapse_time needs to be less than total_time.
        return;

    FormatTime(total_time - elapse_time, string_time);
}

void CheckStringTimeValidity(CString &str_number, int &time)
{
    // If string number is larger than MAX_MINUTES, that is 1000*24*60(1000 days), then it is invalidate.
    if ( _ttoi(str_number) > MAX_MINUTES )
    {
        str_number.Format(L"%d", MAX_MINUTES);
    }
    else if ( _ttoi(str_number) <= 0 )
    {
        str_number = L"1";
    }
    time = _ttoi(str_number);
}

void CheckStringTimeValidity(std::string &str_number, int &time)
{
    if ( atoi(str_number.c_str()) > MAX_MINUTES )
    {
        time = MAX_MINUTES;
    }
    else if ( atoi(str_number.c_str()) < 0 )
    {
        time = 0;
    }
    else
    {
        time = atoi(str_number.c_str());
    }
}

CString AddGlobalHotKey(HWND hwnd)
{
    CString str_error = L"";
    global_id_screen_saver = GlobalAddAtom(GLOBAL_HOT_KEY_SCREEN_SAVER_STRING);
    global_id_main_window = GlobalAddAtom(GLOBAL_HOT_KEY_MAIN_WINDOW_STRING);
    if ( 0 == global_id_screen_saver || 0 == global_id_main_window )
    {
        if ( 0 == global_id_screen_saver )
            str_error = L"Get Hot Key ID of Screen Saver ";
        else
            str_error = L"Get Hot Key ID of Main Window ";
    }

    if ( 0 == RegisterHotKey(hwnd, global_id_screen_saver, GLOBAL_HOT_KEY_MODIFIER, GLOBAL_HOT_KEY_SCREEN_SAVER_VIRTUAL_KEY) )
        str_error += L" Screen Saver & ";

    if ( 0 == RegisterHotKey(hwnd, global_id_main_window, GLOBAL_HOT_KEY_MODIFIER, GLOBAL_HOT_KEY_MAIN_WINDOW_VIRTUAL_KEY) )
        str_error += L" Main Window ";

    return str_error;
}

void DeleteGlobalHotKey(HWND hwnd)
{
    UnregisterHotKey(hwnd, global_id_screen_saver);
    UnregisterHotKey(hwnd, global_id_main_window);
    
    GlobalDeleteAtom(global_id_screen_saver);
    GlobalDeleteAtom(global_id_main_window);
}

void ChangeRGBValue(short int &red, short int &green, short int &blue, unsigned short int &flag)
{
    unsigned short int increment = 5;

    if ( flag & 0x0100 )
        red += increment;
    else
        red -= increment;

    if ( 10 > red || red > 255 )
    {
        if ( 10 > red )
        {
            flag = flag | 0x0100; // red plus.
            red = 10;
        }
        else
        {
            flag = flag & 0x0011; // red minus.
            red = 255;
        }
        if ( flag & 0x0010 )
            green += increment;
        else
            green -= increment;
    }

    if ( 30 > green || green > 255 )
    {
        if ( 30 > green )
        {
            flag = flag | 0x0010; // 0000 0000 0001 0000
            green = 30;
        }
        else
        {
            flag = flag & 0x0101;
            green = 255;
        }
        if ( flag & 0x0001 )
            blue += increment;
        else
            blue -= increment;
    }
    if ( 50 > blue || blue > 255 )
    {
        if ( 50 > blue )
        {
            flag = flag | 0x0001;
            blue = 50;
        }
        else
        {
            flag = flag & 0x0110;
            blue = 255;
        }
    }
}

bool IsStringMatched(const std::string pattern, const std::string target)
{
    using namespace std;
    std::tr1::regex regular_expression(pattern, std::tr1::regex_constants::extended|std::tr1::regex_constants::icase|std::tr1::regex_constants::nosubs);

    if ( true == std::tr1::regex_search(target, regular_expression) )
        return true;

    return false;
}

void ConvertStringToUpper(std::string &str)
{
    std::string::iterator str_iterator = str.begin();
    for ( ; str.end() != str_iterator; str_iterator++ )
    {
        *str_iterator = toupper(*str_iterator);
    }
}

void TrimString(std::string &str)
{
	using namespace std;

	std::string::iterator it = find_if(str.begin(), str.end(), not1(ptr_fun(isspace)));
	str.erase(str.begin(), it);

	std::string::reverse_iterator rev_it = find_if(str.rbegin(), str.rend(), not1(ptr_fun(isspace)));
	str.erase(rev_it.base(), str.end());
}

bool ReadDataFromFile(const CString &file_path, std::string &str_file_data) // Need to consider auto_ptr.
{
	using namespace std;
    bool status = false;
    char *buffer = NULL;
    size_t size = 0;

    ifstream in_file(file_path.GetString(), ifstream::binary| fstream::in); 
	if ( in_file.is_open() && in_file.good() )
    {
		in_file.seekg(0, in_file.end);
		size = in_file.tellg();
		in_file.seekg(0, in_file.beg);
		if ( 0 < size )
		{
			buffer = (char *)malloc(size+1); 
		}

		if ( buffer )
		{
			memset(buffer, 0, size+1);
            in_file.read(buffer, size);
            if ( in_file )
            {
                str_file_data.assign(buffer); 
                status = true;
            }
            free(buffer);
		}
	}
	in_file.close();
    
	return status;
}

bool SaveDataToFile(const CString &file_path, const std::string &str_data)
{
    using namespace std;
    ofstream out_file( (file_path.GetString()), ofstream::binary | fstream::out );

    if ( out_file.is_open() )
    {
        if ( out_file.good() )
        {
            out_file.write(str_data.c_str(), str_data.length());
        }
        out_file.close();
    }
    else
    {
        return false;
    }

    return true;
}

bool IsAppHasRun(PCWSTR processName)
{
    bool running = false;
    PROCESSENTRY32 processEntry;
    ZeroMemory(&processEntry, sizeof(processEntry));
    processEntry.dwSize = sizeof(processEntry);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (  INVALID_HANDLE_VALUE == snapshot )
    {
        return running;
    }

    if ( Process32First(snapshot, &processEntry) )
    {
        do 
        {
            if ( ( 0 == _wcsicmp(processEntry.szExeFile, processName) ) && ( GetCurrentProcessId() != processEntry.th32ProcessID ) ) // it is not current process.
            {
                running = true;
                break;
            }
        }while ( Process32Next(snapshot, &processEntry) );
    }

    CloseHandle(snapshot);

    return running;
}

