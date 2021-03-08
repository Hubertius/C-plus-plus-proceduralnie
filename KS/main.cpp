/**
 * @file PROJEKT
 * @author Kamil Stachurski
 * @date 19-02-2021
 * @brief EX 2 - Systemy operacyjne II, ćwiczenie 2 - LAB
 */


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <windows.h>
#include <sstream>


#define BUFFERSIZE 1000
using namespace std;
INT NUM;
CRITICAL_SECTION CriticalSection;
HANDLE ghMutex;

class ParamsForThreads {
public:
    vector<int> numbers;
};

void generateRandomNumbers(vector<int> &randomNumbers,const int n){
    const int min = 1;
    const int max = 100;
    for(auto i = 0; i < n; i++){
        randomNumbers.push_back((rand()% max) + min);
    }
}

void saveNumbersToFile(vector<int> &numbers,const string filename){

    HANDLE f = CreateFileA(filename.c_str(),
                           GENERIC_WRITE, // open for writing
                           0,
                           nullptr,
                           CREATE_ALWAYS,
                           FILE_ATTRIBUTE_NORMAL,
                           nullptr);
    if (f == INVALID_HANDLE_VALUE){
        cerr << "Error during creation file" << endl;
    }else{
        string s;

        for(auto x:numbers) {

            s += to_string(x);
            s += ",";
        }
        WriteFile(f, s.c_str(), s.size(),NULL,NULL);
    }
    CloseHandle(f);
}

string readNumbersFromFile(string filename){
    HANDLE f;
    f = CreateFile(filename.c_str(),
                   GENERIC_READ, // open for reading
                   0,
                   nullptr,
                   OPEN_EXISTING,
                   FILE_ATTRIBUTE_NORMAL,
                   nullptr);

    char   ReadBuffer[BUFFERSIZE] = {0};
    OVERLAPPED ol = {0};

    if(f == INVALID_HANDLE_VALUE){
        cerr << "FILE NOT FOUND" << endl;
    }else {
        ReadFileEx(f, ReadBuffer, BUFFERSIZE - 1, &ol, nullptr);

        CloseHandle(f);
        cout<<"2"<<endl;
        return ReadBuffer;
    }
    return "err";
}

DWORD WINAPI MIN(LPVOID lpParam) {

    DWORD dwWaitResult = WaitForSingleObject(ghMutex, INFINITE);


    if (dwWaitResult == WAIT_OBJECT_0) {
        cout<<".";
        auto *param = reinterpret_cast<ParamsForThreads*>(lpParam);
        cout << param->numbers[0] << endl;
        int min = param->numbers[0];

        for (auto num:param->numbers) {
            if (num < min) min = num;
            Sleep(3);
        }
        EnterCriticalSection(&CriticalSection);
        cout << endl << "Min is: " << min << endl;
        ReleaseMutex(ghMutex);
        LeaveCriticalSection(&CriticalSection);
        return 0;
    }

    cout << "Error with thread synchronization!" << endl;
    return -1;
}
DWORD WINAPI MAX(LPVOID lpParam) {

    DWORD dwWaitResult = WaitForSingleObject(ghMutex, INFINITE);


    if (dwWaitResult == WAIT_OBJECT_0) {
        cout << ";";

        auto *param = reinterpret_cast<ParamsForThreads*>(lpParam);

        int max = param->numbers[0];

        for (auto num:param->numbers) {
            if (num > max) max = num;
            Sleep(3);
        }
        EnterCriticalSection(&CriticalSection);
        cout << endl << "Min is: " << max << endl;
        ReleaseMutex(ghMutex);
        LeaveCriticalSection(&CriticalSection);
        return 0;
    }
    cout << "Error with thread synchronization!" << endl;
    return -1;
}
DWORD WINAPI AVG(LPVOID lpParam) {

    DWORD dwWaitResult = WaitForSingleObject(ghMutex, INFINITE);


    if (dwWaitResult == WAIT_OBJECT_0) {
        cout << ",";
        auto *param = reinterpret_cast<ParamsForThreads*>(lpParam);

        int sum = 0;
        for (auto num:param->numbers) {
            sum += num;
            Sleep(3);
        }
        float avg = sum / param->numbers.size();
        EnterCriticalSection(&CriticalSection);
        cout << endl << "Min is: " << avg << endl;
        ReleaseMutex(ghMutex);
        LeaveCriticalSection(&CriticalSection);
        return 0;
    }
    cout << "Error with thread synchronization!" << endl;
    return -1;
}
DWORD WINAPI SET(LPVOID lpParam) {

    auto *param = reinterpret_cast<ParamsForThreads*>(lpParam);
    string fileNumbers = readNumbersFromFile("Numbers.txt");
    string temp;
    for(auto ele:fileNumbers){
        if(ele == ','){
            param->numbers.push_back(stoi(temp));
            temp = "";
        }else{
            temp += ele;
        }
    }

    ReleaseMutex(ghMutex);
    return 0;
}



int process1(){
    cout << "Starting as process 1" << endl;

    vector<int> randomNumbers;

    generateRandomNumbers(randomNumbers, NUM);
    saveNumbersToFile(randomNumbers, "Numbers.txt");
    cout << readNumbersFromFile("Numbers.txt") << endl;

    return 0;

}

int process2(){
    cout << "Starting as process 2" << endl;
    string numbers = readNumbersFromFile("Numbers.txt");
    if(numbers == "err"){
        return 1;
    }else {
    return 0;
    }
}


int main(int argc, char **argv, char **envp) {
    srand(time(nullptr));
    if(argc > 1){
        // process 1
        NUM = atoi(argv[1]);
        if (NUM <=0){
            cout << "Plese enter a whole number greater or equal 1 as argument" << endl;
            return 1;
        }
        process1();
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si,sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi,sizeof(pi));

        if(!CreateProcessA(NULL,argv[0],NULL,
                           NULL,false,NORMAL_PRIORITY_CLASS,NULL,NULL,&si,&pi)){
            cout << "failed" << endl;
        }else{
            cout <<"success"<<endl;
        }
        Sleep(2000);

    }else{
        process2();
        DWORD thread_id[4];
        HANDLE thread_handlers[4];

        ParamsForThreads dataC;


        if (!InitializeCriticalSectionAndSpinCount(&CriticalSection,
                                                   0x00000400)) {
            return 0;
        }

        ghMutex = CreateMutex(
                NULL,              // default security attributes
                FALSE,             // initially not owned
                "m1");             // unnamed mutex

        if (ghMutex == NULL) {
            printf("CreateMutex error: %d\n", (int) GetLastError());
            return 1;
        }


        auto thread_param = reinterpret_cast<LPVOID>(&dataC);
        WaitForSingleObject(ghMutex, INFINITE);

        thread_handlers[3] = CreateThread(
                NULL,                   // default security attributes
                0,                      // use default stack size
                SET,                     // thread function name
                thread_param,                   // argument to thread function
                0,                      // use default creation flags
                (LPDWORD) &thread_id[3]);                 // returns the thread identifier

        thread_handlers[0] = CreateThread(
                NULL,                   // default security attributes
                0,                      // use default stack size
                MAX,                     // thread function name
                thread_param,                   // argument to thread function
                0,                      // use default creation flags
                (LPDWORD) &thread_id[0]);                 // returns the thread identifier

        thread_handlers[1] = CreateThread(
                NULL,                   // default security attributes
                0,                      // use default stack size
                MIN,                     // thread function name
                thread_param,                   // argument to thread function
                0,                      // use default creation flags
                (LPDWORD) &thread_id[1]);                 // returns the thread identifier
        thread_handlers[2] = CreateThread(
                NULL,                   // default security attributes
                0,                      // use default stack size
                AVG,                     // thread function name
                thread_param,                   // argument to thread function
                0,                      // use default creation flags
                (LPDWORD) &thread_id[2]);


        if (!(thread_handlers[0] && thread_handlers[1] && thread_handlers[2]&& thread_handlers[3])) {
            cout << "Create Thread" << endl;
            ExitProcess(3);
        }

        WaitForSingleObject(thread_handlers[2], INFINITE);
        ReleaseMutex(ghMutex);
        WaitForMultipleObjects(3, thread_handlers, TRUE, INFINITE);
        DeleteCriticalSection(&CriticalSection);
        CloseHandle(thread_handlers[0]);
        CloseHandle(thread_handlers[1]);
        CloseHandle(thread_handlers[2]);
        CloseHandle(thread_handlers[3]);
        CloseHandle(ghMutex);

        return 0;
    }
    // compiled using  g++ -Wall main.cpp -o main.exe
}
