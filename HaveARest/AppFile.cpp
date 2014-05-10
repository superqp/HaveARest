#include"StdAfx.h"
#include"AppFile.h"
#include"CommonAPI.h"
#include<fstream>
#include<string>
#include"Properties.h"
#include"HaveARest.h"

CAppFile appFile;

bool CAppFile::init()
{
    CString file_path = L"";

    if ( 0 == GetModuleFileName(NULL, file_path.GetBufferSetLength(MAX_PATH+1), MAX_PATH) )
        return false;

    file_path.ReleaseBuffer();

    if ( file_path.IsEmpty() )
        return false;

    int char_position = 0;
    char_position = file_path.ReverseFind('\\');

    m_appPath = file_path.Left( char_position+1 );
    m_appName = file_path.Right( file_path.GetLength() - char_position - 1 );

    if ( m_appName.IsEmpty() )
        return false;

    char_position = m_appName.ReverseFind('.');
    m_appBaseName = m_appName.Left(char_position);
    m_confName = m_appName.Left( char_position + 1 );
    m_confName += CONFIGURATION_FILE_SUFFIX;

    return true;
}

bool CAppFile::isConfFileExist()
{
    bool exist = false;

	//// method 1
 //   std::ifstream conf_file((getConfPathName()).GetString());
 //   if (conf_file.good())
 //       exist = true;
 //   conf_file.close();

	// method 2
	if ( 0 == _waccess( (getConfPathName()).GetString(), 0 ) )
	{
		exist = true;
	}
    return exist;
}

// configuration format:
// prop: value
bool CAppFile::loadConfigurationFromFile()
{
    bool ret = false;
    std::ifstream conf_file((getConfPathName()).GetString());
    if (conf_file.good())
    {
        std::string str_prop;
        std::string str_value;
        static const std::basic_string <char>::size_type npos = -1;
        std::basic_string <char>::size_type separator_position = 0;
        while( 1 )
        {
            getline(conf_file, str_prop); 
            separator_position = str_prop.find_first_of(PROP_DELIMITER);
            if ( npos != separator_position )
            {
                str_value = str_prop.substr( separator_position+1 );
                str_prop = str_prop.substr( 0, separator_position );
                //haveARest.getValueFromConfFile(str_prop, str_value);
            }

            if ( conf_file.eof() )
            {
                ret = true;
                break;
            }
        }
    }
    conf_file.close();

    return ret;
}

bool CAppFile::saveConfigurationToFile()
{
    using namespace std;
    std::string str_file_data;
    long int size = 0;
    bool status = false;

    str_file_data.clear();
    size = ReadDataFromFile((getConfPathName()).GetString(), str_file_data);
    if ( 0 < size )
    {
        long int old_pos = 0;
        long int new_pos = 0;
        string str_prop;

        // append a LINE_FEED at last of the string.
        // Since the last character of string data is not LINE_FEED, the loop will break, that is it will not handle the last line which not contains LINE_FEED.
        if ( LINE_FEED != str_file_data[size-1] )
         {
             str_file_data.append(1, LINE_FEED);
             size++;
        }

        // get value from haveARest and save it to string.
        while( new_pos <= size ) // std::string::npos != new_pos.
        {
            // find LINE_FEED, and get line.
            new_pos = str_file_data.find(LINE_FEED, old_pos);
            if ( string::npos != new_pos )
            {
                str_prop = str_file_data.substr(old_pos, new_pos - old_pos); 
            }
            else
                break;

            //  set value to str_prop in "prop: value" according to haveARest.
            //haveARest.setValueToPropString(str_prop);
            str_file_data.replace(old_pos, new_pos - old_pos, str_prop);

            // The length of string str_prop may be changed, so we need to re-find the LINE_FEED.
            new_pos = str_file_data.find(LINE_FEED, old_pos);
            old_pos = new_pos +1;
        }
    }
    else // Assume that the file is not accessible, just get props from haveARest.
    {
        haveARest.convergePropsToString(str_file_data);
    }

    if ( ! str_file_data.empty() )
    {
        // save to file.
        status = SaveDataToFile( getConfPathName(), str_file_data );
    }

    return status;
}

