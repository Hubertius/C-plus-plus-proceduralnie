/*
 * @file main.cpp
 * @author Hubert Kopeć
 * @date 19/02/2021
 * @brief EX 1 - Systemy operacyjne II, Program po LAB1 i LAB2
 */

#include <iostream>
#include <fstream>
#include <windows.h>
#include <windowsx.h>
#include <string>
#include <algorithm>




int main(int argc, char * argv [], char * envp[])
{
    using namespace std;
    string file_name = "data.txt";
    HANDLE file =
    CreateFile(file_name.c_str(),
               GENERIC_READ | GENERIC_WRITE,
               FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
               0,
               CREATE_ALWAYS,
               FILE_ATTRIBUTE_NORMAL,
               0);
    if (file == INVALID_HANDLE_VALUE)
    {
        cout << "Error!" << endl;
        return 1;
    }
    bool check_for_hack = false;
    for(size_t i = 0; i < argc; i++)
    {
      cout << argv[i] << endl;
      if( argv[i] == "hack")
      {
        check_for_hack == true;
      }
    }
    while(*envp++)
    {
      string envpString;
      if(  envp != NULL )
      {
        envpString = *envp;
        cout << envpString << endl;
      }
      DWORD dwNoByteWritten = 0;
      if( envp == NULL )
      {
        break;
      }
      if( check_for_hack )
      {
        string user = "USER";
        string password = "PASSWORD";
        for(auto & c: envpString)
        {
          c = toupper(c);
        }
        int position_user = envpString.find(user);
        int position_password = envpString.find(password);
        if( position_user == string::npos  && position_password == string::npos )
        {
          continue;
        }
      }
      string string_to_char_pointer = envpString + "\n";
      if( string_to_char_pointer != "\0" )
      {
        char * text = (char *)string_to_char_pointer.c_str();
        WriteFile(file, text , strlen(text), &dwNoByteWritten, 0);
      }
    }
    CloseHandle(file);
    return 0;
}








