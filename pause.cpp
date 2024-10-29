#include "pause.h"
using namespace std;

void pause(string text, bool stop) {
    cout << text;
    #ifndef BUILD_WIN
    cin.ignore();
    #elif BUILD_WIN
    system("pause >nul");
    #else
    system("read");
    #endif
    if(stop) exit(0);
}

void pause(int n, bool stop) {
    pause(to_string(n), stop);
}

void winpause(bool stop) {
    #ifdef BUILD_WIN
    system("pause");
    #else
    pause();
    #endif
    if(stop) exit(0);
}
