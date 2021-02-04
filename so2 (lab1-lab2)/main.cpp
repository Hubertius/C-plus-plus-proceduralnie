/*
 * @file main.cpp
 * @author Hubert Kopeć
 * @date 29/01/2021
 * @brief EX 1 - Systemy operacyjne II, Program po LAB1 i LAB2
 */

#include <iostream>
#include <fstream>
#include <windows.h>
#include <windowsx.h>
#include <string>




void openFileInNotepad(std::string name);

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
    for(size_t i = 0; i < argc; i++)
    {
      cout << argv[i] << endl;
    }
    bool check_for_notepad = false;
    while(*envp++)
    {
      cout << *envp << endl;
      DWORD dwNoByteWritten = 0;
      if( *envp == NULL )
      {
        break;
      }
      if( *envp == "NOTEPAD=TRUE" )
      {
        check_for_notepad = true;
      }
      string string_to_char_pointer = (string)*envp + "\n";
      if( string_to_char_pointer != "\0" )
      {
        char * text = (char *)string_to_char_pointer.c_str();
        WriteFile(file, text , strlen(text), &dwNoByteWritten, 0);
      }
    }
    CloseHandle(file);
    if( check_for_notepad == true )
    {
      openFileInNotepad(file_name);
    }
    return 0;
}







void openFileInNotepad(std::string name)
{
  std::string notepadFileName = "notepad\"" + name + "\"";
	system(notepadFileName.c_str());  //"It is used to pass the commands that can be executed in the command processor or the terminal of the operating system, and finally returns the command after it has been completed."
}
