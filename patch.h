#ifndef PATCH_H_INCLUDED
#define PATCH_H_INCLUDED
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

// швабракадабра
const int vn = 19;
const std::string VER[] = {
    "1.0", "1.01", "1.02", "1.1", "1.11", "1.2", "1.21", "1.22", "1.3", "1.4", "1.41", "1.5", "1.51", "1.6", "1.7", "1.71", "1.8", "1.81", "1.811", "1.9", "1.91", "1.92", "1.93"
};
const bool SUPPORT[vn] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

extern std::fstream file;
extern unsigned obj;
extern int ver, lib;

int Patch(unsigned n = obj, bool visual = 1);
unsigned Approx(bool type);
void Write(int i, void f(char* a) = {}, int n = 4);
void Write(int i, std::string a);

// EXTRA
#if BUILD_DEBUG
void getPows();
bool CheckApprox(unsigned n);
void Dword(char* a);
void Mov(char* a);
void Cmp(char* a);
#endif // BUILD_DEBUG

#endif // PATCH_H_INCLUDED
