/*
 * @file main.cpp
 * @author Hubert Kopeć
 * @date 29/01/2021
 * @brief EX 2 - Systemy operacyjne II, Program po LAB3
 */

#include <windows.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>
#include <sstream>

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
    else {
      std::string textToSave("A "); // tworzenie stringa
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
  char buffer[1000];
  DWORD size_of_file = GetFileSize(file, nullptr);
  bool check_for_existence  = ReadFile(
    file,
    buffer,
    size_of_file,
    nullptr,
    nullptr
  );
  if( check_for_existence == false || *(buffer + 0) != 'A'  )
  {
    std::cout << "Error! Plik nie istnieje (albo nie ma litery A na poczatku)!" << std::endl;
    return 1;
  }
  else
  {
    int count_all_numbers = 0;
    int add = 0;
    std::stringstream  numbers(std::string(buffer + 1));
    for(int i; numbers >> i;)
    {
      add += i;
      count_all_numbers++;
      if( numbers.peek() == ',' )
      {
        numbers.ignore();
      }
      std::cout << i << std::endl;
    }
    std::cout << "Srednia: " << add / count_all_numbers << std::endl;
  }
  CloseHandle(file);
  return 0;
}

/* WERSJA OBYDWU PONIŻSZYCH FUNKCJI Z TREŚCI ZADANIA NA WIKAMPIE

int process1(){
    std::string file_name = "log.txt";
    DWORD dwBytesWritten = 0;
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
        std::cout << "Error!" << std::endl;
        return 1;
    }
    else
    {
      if( !process2(file) )
      {
        std::cout << "Something went wrong! Error!" << std::endl;
        return 1;
      }
      std::string buffer;
      std::cout << "Enter text: ";
      getline(std::cin, buffer);
      WriteFile(
        file,
        buffer.c_str(),
        buffer.length(),
        &dwBytesWritten,
        NULL
      );
      process2(file);
    }
    CloseHandle(file);

    return 0;
}

bool process2(HANDLE file){
  char * buffer;
  DWORD size_of_file = GetFileSize(file, nullptr);
  bool check_for_existence  = ReadFile(
    file,
    buffer,
    size_of_file,
    nullptr,
    nullptr
  );
  if( !check_for_existence )
  {
    std::cout << "Error! File doesn't exist!" << std::endl;
    return check_for_existence;
  }
  std::cout << "Size of the file: " << size_of_file << std::endl;
  return check_for_existence;
}

*/

int main()
{

  process1(100);
  process2();


  /* KOD ODNOSZĄCY SIĘ DO TREŚCI ZADANIA NA WIKAMP

  std::vector<int> numbers;
  generate_random_numbers(numbers, 100);
  for(auto const& number: numbers) // number to zmienna iteracyjna, numbers zmienna typu wektorowego
  {
    std::cout << ", " << number << std::endl;
  }
  std::string fileName = "numbers.txt";
  save_numbers_to_file(numbers, fileName);
  if( process1() )
  {
    std::cout << "Process 1 and 2 ended successfully." << std::endl;
  }

  */
  return 0;
}
