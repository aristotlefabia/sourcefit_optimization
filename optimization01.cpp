// optimization.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;


using namespace std;

/* Project : OMT (NEC Telecom Software Philippines Inc.) */
/*
 * System call has a cost which affects the performance. For this optimization
 * the sleep parameter was set to 0 inorder to finish I/O operation earlier.
 * Although using 10 ms in parameter will makes the program more responsive to user cancellation event, 
 * it incurs a performance degradation when there is no user cancellation. Changing the 10 ms to 0ms 
 * improves the performance if there is no user cancellation and user cancellation can still be handled.
 * 
 */


#define OPTIMIZED 1


HANDLE hRequestExitEvent = FALSE;

DWORD WINAPI ThreadFun1(LPVOID e)
{
    cout << "thread started..." << endl;
    HANDLE* quitEvent = (HANDLE*)e;
    string myText;

    DWORD dwStartTime = GetTickCount();

    ifstream MyReadFile("C:\\log\\enwik8\\enwik8");
    ofstream MyOutFile("C:\\log\\enwik8\\enwik8.log.bak");
    int line = 0;
    while (getline(MyReadFile, myText)) {
        MyOutFile << myText;
        line++;

#ifdef OPTIMIZED
        if (WaitForSingleObject(*quitEvent, 0) != WAIT_TIMEOUT) {
#else`
        if (WaitForSingleObject(*quitEvent, 10) != WAIT_TIMEOUT) {
#endif

            cout << "Cancelled" << endl;
            break;
        }
    
    }
    DWORD dwFinishTime = GetTickCount();
    
    MyReadFile.close();
    MyOutFile.close();

    cout << "ms passed: " << dwFinishTime - dwStartTime << endl;
    cout << "exiting thread " << endl;

   
    return 0;
}

void optimization_test01()
{
    HANDLE hThread;

    hRequestExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    DWORD dwThreadID;

    hThread = CreateThread(NULL, 0, ThreadFun1, (LPVOID)&hRequestExitEvent, 0, &dwThreadID);


    char ch = 'r';

    Sleep(10);

    do {
        cout << "\npress 'q' to quit : ";
        cin >> ch;

    } while (ch != 'q');

    SetEvent(hRequestExitEvent);

    WaitForSingleObject(hThread, INFINITE);
    printf("\nexiting...");
}


