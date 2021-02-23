/*
 * @file main.cpp
 * @author Hubert Kopeć
 * @date 19/02/2021
 * @brief EX 2 - Systemy operacyjne II, Program po LAB3
 */

#include <windows.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <algorithm>
#include <string>
#include <sstream>
#include <ctime>
void generate_random_numbers(std::vector<int> & numbers, int n);
int save_numbers_to_file(std::vector<int> &numbers, std::string filename);
int process1(int n);
int process2();

/*

int process1();
bool process2(HANDLE file);

*/


void generate_random_numbers(std::vector<int> & numbers, int n) {
  const int min = 1;
  const int max = 100;
  srand(time(NULL));
  for(auto i = 0; i < n; i++)
    numbers.push_back(((std::rand() % max) + min)); // generowanie randomowej liczby i pushowanie jej do zmiennej typu wektorowego "numbers"
}

int save_numbers_to_file(std::vector<int> &numbers, std::string filename){
  HANDLE fileHandle = CreateFile(
            filename.c_str(), // c_str() powoduje konwersje na LCPWSTR
            GENERIC_WRITE,
            0, // nie jest współdzielone
            nullptr,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
    );

    if (fileHandle == INVALID_HANDLE_VALUE) {
      std::cerr << "Error during creation file" <<std::endl; // cerr to strumień błędów
      return 1;
    }
    else 
    {
    std::string textToSave; // tworzenie stringa
      std::for_each(numbers.begin(),
                    numbers.end(),
                    [&textToSave](const int &n) {              // [&testToSave] adres naszego string-a
                        textToSave += std::to_string(n);
                        textToSave += ",";  });
      WriteFile(fileHandle, textToSave.c_str(), textToSave.size(), nullptr, nullptr);
      CloseHandle(fileHandle);
    }

    return 0;
}

int process1(int n)
{
  std::cout << "Start of the process 1." << std::endl;
  std::vector<int> numbers;
  generate_random_numbers(numbers, n);
  std::string fileName = "numbers.txt";
  save_numbers_to_file(numbers, fileName);
  std::cout << "End of the process 1." << std::endl;
  return 0;

}

int process2()
{
  std::cout << "Start of the process 2." << std::endl;

  HANDLE file;
  do
  {
  file = CreateFile(
    "numbers.txt",
    GENERIC_READ,
    0,
    nullptr,
    OPEN_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,
    nullptr
  );
  }while(file == INVALID_HANDLE_VALUE);
  if (file == INVALID_HANDLE_VALUE)
  {
      std::cout << "Error!" << std::endl;
      return 1;
  }
  DWORD size_of_file = GetFileSize(file, nullptr);
  if( size_of_file == 0xFFFFFFFF )
  {
    std::cout << "Error of the size of file." << std::endl;
    CloseHandle(file);
    return 0;
  }
  auto data = (LPSTR) GlobalAlloc(GPTR, size_of_file + 1);
  DWORD read;
  bool check_for_existence  = ReadFile(
    file,
    data,
    size_of_file,
    &read,
    nullptr
  );
  if( check_for_existence == false )
  {
    std::cout << "Error! Plik nie istnieje." << std::endl;
    CloseHandle(file);
    return 1;
  }
  else
  {
    std::cout << "File accepted." << std::endl;
    data[size_of_file] = 0;
    double add = 0;
    std::string toString(data);
    std::stringstream  numbersFromFile(toString);

    GlobalFree(data);
    /*int i;
    for(i; numbers >> i;)
    {
        add += i;
        count_all_numbers++;
        if( numbers.peek() == ',' )
        {
          numbers.ignore();
        }
        std::cout << i << std::endl;
    }*/
    std::vector<int> vector_data;
    for(int i; numbersFromFile >> i;)
    {
      vector_data.push_back(i);
      if( numbersFromFile.peek() == ',' )
      {
          numbersFromFile.ignore();
      }
    }
    int count = 0;
    for(int i = 0; i < vector_data.size(); i++)
    {
      add += vector_data[i];
      count++;
    }
    std::cout << "Srednia: " << add / count << std::endl;
  
  }
  CloseHandle(file);
  return 0;
}


int main(int argc, char **argv, char **envp)
{
  if( argc < 2 )
  {
    process2();
  }
  else
  {
    int n;
    try
    {
      n = std::stoi(argv[1]);
    }
    catch(const std::invalid_argument & error)
    {
      std::cout << "Error with the argument!" << std::endl;
      return 1;
    }
        process1(n);
        STARTUPINFO startupinfo;
        PROCESS_INFORMATION process_information;

        ZeroMemory(&startupinfo, sizeof(startupinfo));
        startupinfo.cb = sizeof(startupinfo);
        ZeroMemory(&process_information, sizeof(process_information));

        CreateProcess(
                nullptr,
                argv[0],
                nullptr,
                nullptr,
                0,
                NORMAL_PRIORITY_CLASS,
                nullptr,
                nullptr,
                &startupinfo,
                &process_information
        );
        WaitForSingleObject(process_information.hProcess, INFINITE);
        CloseHandle(process_information.hProcess);
        CloseHandle(process_information.hThread);
    }
    return 0;    

}
