/**
 * @file main.cpp
 * @author Przemysław Olszewski
 * @date 17.02.2021
 * @brief EX 3 - Systemy operacyjne II, podsumowanie laboratorium - LAB
 */

#include <iostream>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <cstring>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;
CRITICAL_SECTION CriticalSection;


//  generator liczb pseudolosowych
void generate_random_numbers(vector<int> &numbers, int n) {
    srand((int) time(NULL));
    const int min = 1;
    const int max = 100;
    int randomNumber;

    for (auto i = 0; i < n; ++i) {
        randomNumber = (rand() % max) + min;
        numbers.push_back(randomNumber);
    }
}


// funkcja zapisująca wektor liczb do pliku
void save_numbers_to_file(vector<int> &numbers, string filename) {
    HANDLE hFile = CreateFileA(filename.c_str(),
                               GENERIC_WRITE,
                               0,
                               nullptr,
                               CREATE_ALWAYS,
                               FILE_ATTRIBUTE_NORMAL,
                               nullptr);


    if (hFile == INVALID_HANDLE_VALUE) {
        cerr << "Error during creation file! Error: " << GetLastError() << endl;
    } else {
        string textToSave("A ");
        for_each(numbers.begin(), numbers.end(), [&textToSave](const int &n) {  //&textToSave  adres naszego stringa
            textToSave += to_string(n);
            textToSave += ",";
        });

        BOOL bFile = WriteFile(hFile,
                               textToSave.c_str(),
                               textToSave.size(),
                               nullptr,
                               nullptr);

        if (bFile == FALSE) {
            cout << "WriteFile failed!. Error: " << GetLastError() << endl;
        }
        CloseHandle(hFile);
    }
}


// srednia
DWORD WINAPI average(LPVOID lpParam) {
    vector<int> *myVector = reinterpret_cast<vector<int> *>(lpParam);
    int sum = 0;
    int counter = 0;
    int average = 0;

    for (auto x: *myVector) {
        sum = sum + x;
        counter++;
    }

    average = sum / counter;
    EnterCriticalSection(&CriticalSection);
    cout << "Average: " << average << endl;
    LeaveCriticalSection(&CriticalSection);

    return 0;
}


// minimum
DWORD WINAPI min(LPVOID lpParam) {
    vector<int> *myVector = reinterpret_cast<vector<int> *>(lpParam);
    int min = 105; // liczby 1-100, więc inicjalizuję liczbą spoza zakresu

    for (auto x: *myVector) {
        if (x < min) {
            min = x;
        }
    }

    EnterCriticalSection(&CriticalSection);
    cout << "Minimum: " << min << endl;
    LeaveCriticalSection(&CriticalSection);

    return 0;
}


// maksimum
DWORD WINAPI max(LPVOID lpParam) {
    vector<int> *myVector = reinterpret_cast<vector<int> *>(lpParam);
    int max = -1; // liczby 1-100, więc inicjalizuję liczbą spoza zakresu

    for (auto x: *myVector) {
        if (x > max) {
            max = x;
        }
    }

    EnterCriticalSection(&CriticalSection);
    cout << "Maximum: " << max << endl;
    LeaveCriticalSection(&CriticalSection);

    return 0;
}


// funkcja wywołująca generator oraz zapis liczb do pliku
int process1(int n) {
    cout << "Starting as process 1" << endl;

    vector<int> numbers;
    generate_random_numbers(numbers, n);
    save_numbers_to_file(numbers, ".\\file.txt");
    Sleep(1000);

    return 0;
}


// funkcja obliczająca i wyświetlająca minimum, maksimum oraz wartość średnią z liczb z pliku
int process2() {
    cout << "Starting as process 2" << endl;
    DWORD thread_id[3];
    HANDLE thread_handlers[3];

    HANDLE hFile;
    do {
        cout << "Waiting for a file handle in the process 2()..." << endl << endl;

        // uchwyt do pliku
        hFile = CreateFile(
                "file.txt",
                GENERIC_READ,
                0,
                nullptr,
                OPEN_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                nullptr
        );
    } while (hFile == INVALID_HANDLE_VALUE);

    // deklaracja i inicjalizacja zmiennej przechowującej zawartość pliku
    char chBuffer[500];
    DWORD dwNoByteToRead = GetFileSize(hFile, nullptr);
    DWORD dwNoByteRead = 0;
    BOOL bFile = ReadFile(
            hFile,
            chBuffer,
            dwNoByteToRead,
            &dwNoByteRead,
            nullptr);

    if (bFile == FALSE) {
        cout << "ReadFile failed! Error: " << GetLastError() << endl;
    }

    // przepisanie stringstream do vectora
    stringstream sStream(string(chBuffer + 1));
    vector<int> myVector;
    int n;
    while (sStream >> n) {
        myVector.push_back(n);
        if (sStream.peek() == ',') {
            sStream.ignore();
        }
    }

    // inicjalizacja sekcji krytycznej
    if (!InitializeCriticalSectionAndSpinCount(&CriticalSection,
                                               0x00000400)) {
        return 0;
    }

    // cast vector na LPVOID - parametr uruchamianych funkcji
    LPVOID parameter = reinterpret_cast<LPVOID>(&myVector);

    // URUCHAMIANIE WĄTKÓW WYWOWUJACYCH FUNKCJE OBLICZAJACE
    thread_handlers[0] = CreateThread(
            NULL,                       // default security attributes
            0,                          // use default stack size
            average,                    // thread function name
            parameter,                  // argument to thread function
            0,                          // use default creation flags
            (LPDWORD) &thread_id[0]);   // returns the thread identifier

    thread_handlers[1] = CreateThread(
            NULL,                       // default security attributes
            0,                          // use default stack size
            min,                        // thread function name
            parameter,                  // argument to thread function
            0,                          // use default creation flags
            (LPDWORD) &thread_id[1]);   // returns the thread identifier

    thread_handlers[2] = CreateThread(
            NULL,                       // default security attributes
            0,                          // use default stack size
            max,                        // thread function name
            parameter,                  // argument to thread function
            0,                          // use default creation flags
            (LPDWORD) &thread_id[2]);   // returns the thread identifier


    if (thread_handlers[0] == NULL || thread_handlers[1] == NULL || thread_handlers[2] == NULL) {
        cout << "Create Thread Error" << endl;
        ExitProcess(3);
    }

    //  oczekiwanie na wielokrotne zgloszenie
    WaitForMultipleObjects(3, thread_handlers, TRUE, INFINITE);

    // zamykanie uchwytów oraz usuwanie sekcji krytycznej
    DeleteCriticalSection(&CriticalSection);
    CloseHandle(thread_handlers[0]);
    CloseHandle(thread_handlers[1]);
    CloseHandle(thread_handlers[2]);
    CloseHandle(hFile);

    return 0;

}


int main(int argc, char *argv[]) {

    // sprawdza czy jest podany chociaż jeden parametr po nazwie pliku i jest on liczbą
    if (argc > 1 && isdigit(*argv[1])) {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        // wypełnia zerami podany obszar pamięci
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);

        ZeroMemory(&pi, sizeof(pi));

        // tworzy proces
        CreateProcess(
                nullptr,
                argv[0],
                nullptr,
                nullptr,
                FALSE,
                NORMAL_PRIORITY_CLASS,
                nullptr,
                nullptr,
                &si,
                &pi
        );
        int number = atoi(argv[1]);
        process1(number);
    } else {
        process2();
    }

    return 0;
}