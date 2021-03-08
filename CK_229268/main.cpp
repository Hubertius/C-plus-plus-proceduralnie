/*
 * zadanie ostatnie
 * podsumowanie
 * Cezary Karczewski 229268
 * so2
 * */

#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <sstream>

CRITICAL_SECTION CriticalSection;
HANDLE ghMutex;
DWORD threadId[3];
HANDLE thread_handlers[3];


//KLASA Z DANYMI DO WATKU
class ParamsForThreads {
public:
    std::vector<int> numbers;
    int count;
    int min;
    int max;
    double avg;
};
//GENEROWANIE LOSOWO LICZB
void generate_random_number(std::vector<int> &numbers, const int n, const int min, const int max) {
    for (auto i = 0; i < n; i++) {
        numbers.push_back(min + std::rand() % max);
    }
}
//ZAPIS DO PLIKU
void save_numbers_to_file(std::vector<int> &numbers) {
    std::string textToSave;
    int sizeOfNumbers = numbers.size();
    for (auto i = 0; i < sizeOfNumbers; i++) {
        textToSave += std::to_string(numbers[i]);
        if (i < sizeOfNumbers - 1) {
            textToSave += ",";
        }
    }

    HANDLE fileHandle = CreateFile(
            "data.txt",
            GENERIC_WRITE,
            0, //shareMode
            nullptr,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
    );

    if (fileHandle == INVALID_HANDLE_VALUE) {
        return;
    }

    if (WriteFile(fileHandle, textToSave.c_str(), textToSave.size(), nullptr, nullptr)) {
        std::cout << "Poprawnie zapisano dane";
    } else {
        std::cout << "Nie udalo sie zapisac danych";
    }

    Sleep(2000);

    CloseHandle(fileHandle);
}

//ODCZYTYWANIE Z PLIKU
void readFile(HANDLE fileHandle, ParamsForThreads *paramsForThreads) {
    DWORD size, read;

    size = GetFileSize(fileHandle, nullptr);

    if (size == 0xFFFFFFFF){
        CloseHandle(fileHandle);
    }

    auto data = (LPSTR)GlobalAlloc(GPTR, size + 1);
    if (ReadFile(fileHandle, data, size, &read, nullptr) == false){
        CloseHandle(fileHandle);
    }

    data[size] = 0;

    std::string fileText(data);
    GlobalFree(data);

    std::stringstream ss(fileText);
    for (int i; ss >> i;)
    {
        paramsForThreads->numbers.push_back(i);
        if (ss.peek() == ',')
            ss.ignore();
    }

    paramsForThreads->count = paramsForThreads->numbers.size();
}

// FUN MIN
DWORD WINAPI mini(LPVOID lpParam){

    DWORD dwWaitResult = WaitForSingleObject(ghMutex, INFINITE);

    if(dwWaitResult == WAIT_OBJECT_0) {

        ParamsForThreads *param = reinterpret_cast<ParamsForThreads*>(lpParam);

        auto mini = param->numbers.at(0);
        for(auto i:param->numbers){

            if(i < mini ){
                mini = i;
            }
            Sleep(1);
        }

        EnterCriticalSection(&CriticalSection);
        param->min = mini;
        std::cout<<"Min "<<param->min<<std::endl;
        LeaveCriticalSection(&CriticalSection);
        ReleaseMutex(ghMutex);
        return 0;

    }
    std::cout<<"Error with thread synchronization!"<<std::endl;
    return -1;
}

//FUN MAXI
DWORD WINAPI maxi(LPVOID lpParam){

    DWORD dwWaitResult = WaitForSingleObject(ghMutex, INFINITE);

    if (dwWaitResult == WAIT_OBJECT_0) {
        ParamsForThreads *param = reinterpret_cast<ParamsForThreads*>(lpParam);

        auto maximum = param->numbers.at(0);
        for(auto i:param->numbers){

            if(i > maximum){
                maximum = i;
            }
            Sleep(1);
        }
        EnterCriticalSection(&CriticalSection);
        param->max = maximum;
        std::cout<<"Max "<<param->max<<std::endl;
        LeaveCriticalSection(&CriticalSection);
        ReleaseMutex(ghMutex);

        return 0;
    }
    std::cout<<"Error with thread synchronization!"<<std::endl;
    return -1;
}

//FUN AVG
DWORD WINAPI average(LPVOID lpParam){

    DWORD dwWaitResult = WaitForSingleObject(ghMutex, INFINITE);

    if (dwWaitResult == WAIT_OBJECT_0) {
        ParamsForThreads *param = reinterpret_cast<ParamsForThreads*>(lpParam);

        auto sum = 0;
        int counter = 0;
        for(auto i:param->numbers){
            counter++;
            sum += i;
            Sleep(1);
        }
        double average = sum/(counter*1.0);
        EnterCriticalSection(&CriticalSection);
        param->avg = average;
        std::cout<<"Srednia "<<param->avg<<std::endl;
        LeaveCriticalSection(&CriticalSection);
        ReleaseMutex(ghMutex);
        return 0;
    }
    std::cout<<"Error with thread synchronization!"<<std::endl;
    return -1;
}

// PROCES 1
int process1(int n) {
    srand(time(nullptr));

    std::vector<int> numbers;
    const int min = 1;
    const int max = 100;

    generate_random_number(numbers, n, min, max);

    save_numbers_to_file(numbers);

    return 0;
}

//PROCES 2
int process2() {
    std::cout << "Proces 2 - czekam na plik" << std::endl;

    HANDLE fileHandle;
    do {
        fileHandle = CreateFile(
                "data.txt",
                GENERIC_READ,
                0, //shareMode
                nullptr,
                OPEN_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                nullptr
        );
    } while (fileHandle == INVALID_HANDLE_VALUE);

    std::cout << "Otworzylem plik" << std::endl;

    ParamsForThreads readData;

    readFile(fileHandle, &readData);
    CloseHandle(fileHandle);

    if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400)) {
        return 0;
    }

    ghMutex = CreateMutex(
            nullptr,            // default security attributes
            FALSE,             // initially not owned
            "m1");            // unnamed mutex


    if (ghMutex == nullptr){
        std::cout<<"CreateMutex error: "<<(int)GetLastError()<<std::endl;
        return 1;
    }

    auto thread_param = reinterpret_cast<LPVOID>(&readData);

    WaitForSingleObject(ghMutex, INFINITE);

    // watek min
    thread_handlers[0] = CreateThread(
            nullptr,                          // default security attributes
            0,                                   // use default stack size
            mini,                                         // thread function name
            thread_param,                                // argument to thread function
            0,                             // use default creation flags
            (LPDWORD)&threadId[0]);           // returns the thread identifier

    // watek max
    thread_handlers[1] = CreateThread(
            nullptr,                       // default security attributes
            0,                                // use default stack size
            maxi,                                      // thread function name
            thread_param,                             // argument to thread function
            0,                          // use default creation flags
            (LPDWORD)&threadId[1]);        // returns the thread identifier

    // watek srednia
    thread_handlers[2] = CreateThread(
            nullptr,                    // default security attributes
            0,                             // use default stack size
            average,                                // thread function name
            thread_param,                          // argument to thread function
            0,                       // use default creation flags
            (LPDWORD)&threadId[2]);     // returns the thread identifier



    if (!(thread_handlers[0] && thread_handlers[1] && thread_handlers[2])) {
        std::cout<<"i can't Create Thread"<<std::endl;
        ExitProcess(3);
    }

    ReleaseMutex(ghMutex);
    WaitForMultipleObjects(3, thread_handlers, TRUE, INFINITE);
    DeleteCriticalSection(&CriticalSection);
    CloseHandle(thread_handlers[0]);
    CloseHandle(thread_handlers[1]);
    CloseHandle(thread_handlers[2]);
    CloseHandle(ghMutex);

    return 0;
}
int main(int argc, char **argv) {
    if (argc < 2) {
        process2();
    } else {
        int n;

        try {
            n = std::stoi(argv[1]);
        } catch (const std::invalid_argument &e) {
            std::cout << "Niepoprawny argument programu";
            return 3;
        }

        process1(n);

        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

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


        WaitForSingleObject( pi.hProcess, INFINITE );
        CloseHandle( pi.hProcess );
        CloseHandle( pi.hThread );
    }

    return 0;
}
