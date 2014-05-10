#define CONFIGURATION_FILE_SUFFIX L"conf"
#define LINE_FEED '\n' // 0x0A; '\r' 0x0D

class CAppFile
{
private:
    CString m_appPath; // The path maybe contain unicode character.
    CString m_appName;
    CString m_appBaseName;
    CString m_confName;

public:
    bool init();
    const CString getAppPath() const { return m_appPath; }
    const CString getAppName() const { return m_appName; }
    const CString getAppBaseName() const { return m_appBaseName; }
    const CString getAppPathName() const { return m_appPath+m_appName; }
    const CString getConfPathName() const { return m_appPath+m_confName; }
    bool isConfFileExist();
    bool loadConfigurationFromFile();
    bool saveConfigurationToFile();
};

extern CAppFile appFile;

//
// File operate function.
//
//open() Open file (public member function )
//is_open() Check if a file is open (public member function )
//close() Close file (public member function )
//getline() Get line
//read() Read block of data
//tellg() Get position in input sequence (public member function )
//seekg() Set position in input sequence 
//write()  Write block of data (public member function )
//tellp()   Get position in output sequence (public member function )
//seekp()  Set position in output sequence (public member function )
//flush()   Flush output stream buffer, update the content of file
//endl()   Insert newline and flush 
//good()   Check whether state of stream is good (public member function )
//eof()  Check whether eofbit is set
//clear()  Set error state flags 



