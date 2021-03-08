/**
 * @file main.cpp
 * @author Kamil Michalski
 * @date 17/02/2021
 * @brief EX 3 - Systemy operacyjne II, ćwiczenie podsumowujące - LAB
 */

#include <iostream>
#include <vector>
#include <Windows.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <accctrl.h>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <ctime>

#define READFILE_BUFFER_SIZE 10000
#define MAX_RAND 100
#define MIN_RAND 1

CRITICAL_SECTION CriticalSection;

void generateRandomNumbers(std::vector<int> &vecNumbers, int n) {

    srand(time(nullptr));

    for (auto i = 0; i < n; i++) {
        vecNumbers.push_back(((std::rand() % MAX_RAND) + MIN_RAND));
    }
}

int save_numbers_to_file(std::vector<int> &vecNumbers, const std::string &fileName) {

    HANDLE numFileHandler = CreateFile(fileName.c_str(),
                                       GENERIC_WRITE,
                                       0,
                                       nullptr,
                                       CREATE_ALWAYS,
                                       FILE_ATTRIBUTE_NORMAL,
                                       nullptr);

    if (numFileHandler == INVALID_HANDLE_VALUE) {
        std::cerr << "Creation file error" << std::endl;
        return 1;
    } else {
        std::string textToSaveInFile("A ");

        std::for_each(vecNumbers.begin(),
                      vecNumbers.end(),
                      [&textToSaveInFile](const int &num) {
                          textToSaveInFile += std::to_string(num);
                          textToSaveInFile += ",";
                      });

        WriteFile(numFileHandler,
                  textToSaveInFile.c_str(),
                  textToSaveInFile.size(),
                  nullptr,
                  nullptr);

        CloseHandle(numFileHandler);
        return 0;
    }
}

int process1(int n) {

    std::vector<int> vecNumbers;

    generateRandomNumbers(vecNumbers, n);

    save_numbers_to_file(vecNumbers, ".\\numbers.txt");
    return 0;
}

DWORD WINAPI minimum(LPVOID vector) {

    auto *vecNumbers = reinterpret_cast<std::vector<int> *>(vector);
    int min = (*vecNumbers)[0];

    for (auto number : *vecNumbers)
        min = min > number ? number : min;

    EnterCriticalSection(&CriticalSection);
    std::cout << "Minimum: " << min << std::endl;
    LeaveCriticalSection(&CriticalSection);
    return 0;
}

DWORD WINAPI maximum(LPVOID vector) {

    auto *vecNumbers = reinterpret_cast<std::vector<int> *>(vector);
    int max = (*vecNumbers)[0];

    for (auto number : *vecNumbers)
        max = max < number ? number : max;

    EnterCriticalSection(&CriticalSection);
    std::cout << "Maximum: " << max << std::endl;
    LeaveCriticalSection(&CriticalSection);
    return 0;
}

DWORD WINAPI average(LPVOID vector) {

    auto *vecNumbers = reinterpret_cast<std::vector<int> *>(vector);
    int sum = 0;
    int numberOfNumbers = 0;

    for (auto number : *vecNumbers) {
        sum += number;
        numberOfNumbers++;
    }

    EnterCriticalSection(&CriticalSection);
    std::cout << "Average: " << sum / numberOfNumbers << std::endl;
    LeaveCriticalSection(&CriticalSection);
    return 0;
}

int process2() {

    HANDLE numFileHandler;

    do {
        numFileHandler = CreateFile("numbers.txt",
                                    GENERIC_READ,
                                    0,
                                    nullptr,
                                    OPEN_ALWAYS,
                                    FILE_ATTRIBUTE_NORMAL,
                                    nullptr);
    } while (numFileHandler == INVALID_HANDLE_VALUE);

    DWORD fileSize = GetFileSize(numFileHandler, nullptr);
    char readFileBuffer[READFILE_BUFFER_SIZE];

    ReadFile(numFileHandler,
             readFileBuffer,
             fileSize,
             nullptr,
             nullptr);

    if (readFileBuffer[0] != 'A') {
        std::cout << "Missing 'A' key at the beginning of the file" << std::endl;
    } else {
        std::stringstream readBufferStream((std::string(&(readFileBuffer)[1])));
        int number;
        std::vector<int> vecNumbers;

        while (readBufferStream >> number) {
            vecNumbers.emplace_back(number);

            if (readBufferStream.peek() == ',') {
                readBufferStream.ignore();
            }
        }

        if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400))
            return 0;

        LPVOID threadParam = reinterpret_cast<LPVOID>(&vecNumbers);
        HANDLE threadHandlers[3];
        DWORD threadId[3];

        threadHandlers[0] = CreateThread(nullptr,
                                         0,
                                         average,
                                         threadParam,
                                         0,
                                         (LPDWORD) &threadId[0]);

        threadHandlers[1] = CreateThread(nullptr,
                                         0,
                                         maximum,
                                         threadParam,
                                         0,
                                         (LPDWORD) &threadId[1]);

        threadHandlers[2] = CreateThread(nullptr,
                                         0,
                                         minimum,
                                         threadParam,
                                         0,
                                         (LPDWORD) &threadId[2]);

        if (threadHandlers[0] == nullptr || threadHandlers[1] == nullptr || threadHandlers[2] == nullptr) {
            std::cout << "At least one of the threads was not created" << std::endl;
            ExitProcess(3);
        }

        WaitForMultipleObjects(3, threadHandlers, TRUE, INFINITE);

        DeleteCriticalSection(&CriticalSection);
        CloseHandle(threadHandlers[0]);
        CloseHandle(threadHandlers[1]);
        CloseHandle(threadHandlers[2]);
    }

    CloseHandle(numFileHandler);
    return 0;
}

int main(int argc, char **argv) {

    if (argc > 1 && isdigit(*argv[1])) {
        STARTUPINFO startupInfo;
        ZeroMemory(&startupInfo, sizeof(startupInfo));
        startupInfo.cb = sizeof(startupInfo);

        PROCESS_INFORMATION processInfo;
        ZeroMemory(&processInfo, sizeof(processInfo));

        CreateProcess(nullptr,
                      argv[0],
                      nullptr,
                      nullptr,
                      FALSE,
                      NORMAL_PRIORITY_CLASS,
                      nullptr,
                      nullptr,
                      &startupInfo,
                      &processInfo);

        process1(atoi(argv[1]));

        WaitForSingleObject(processInfo.hProcess, INFINITE);
    } else {
        process2();
    }

    return 0;
}