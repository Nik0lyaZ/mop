#ifndef PAUSE_H_INCLUDED
#define PAUSE_H_INCLUDED
#include <iostream>
#include <string>

#ifndef BUILD_WIN
const std::string PAUSE_TEXT = "Press Enter key to continue.";
#else
const std::string PAUSE_TEXT = "Press any key to continue.";
#endif

void pause(std::string text = PAUSE_TEXT, bool stop = 0);
void pause(int n, bool stop = 0);
void winpause(bool stop = 0);

#endif // PAUSE_H_INCLUDED
