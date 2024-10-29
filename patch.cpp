#include "patch.h"
#define NOP (string){0x0, (char)0xBF}
using namespace std;

fstream file;
unsigned obj = 16384;
int ver = 0, lib = 0;
vector<unsigned> Pow = {1};

void getPows() {
    if(Pow.size() > 1) return;
    for(int i = 0; i < 31; i++) {
        Pow.push_back(Pow[i] << 1);
    }
}

bool CheckApprox(unsigned n) {
    if(obj > INT_MAX) return obj;
    if(n < 256) return 1;
    getPows();
    int base = Pow[upper_bound(Pow.begin(), Pow.end(), n) - Pow.begin() - 1] >> 7;
    return !(n % base);
}

unsigned Approx(bool type) {
    if(obj > INT_MAX) return obj;
    if(obj < 256) return obj + type;
    getPows();
    int base = Pow[upper_bound(Pow.begin(), Pow.end(), obj) - Pow.begin() - 1] >> 7;
    return obj - obj % base + (type ? base : 0);
}

// Changes DWORD value
void Dword(char* a) {
    int sum = 0, n = obj;
    for(int i = 0; i < 8; i++) {
        if(i % 2) {
            sum += n % 16 << 4;
            *a++ = sum;
        } else {
            sum = n % 16;
        }
        n >>= 4;
    }
}

// MOW.W instruction patch
void Mov(char* a) {
    obj = min(obj, 65535U);
    unsigned char reg = *(a + 3);
    if((*a) >> 4 != 4) {
        if(ver > 0) {
            *(a + 4) = *(a + 2);
            *(a + 5) = *(a + 3);
        }
        reg = *(a + 1);
    }
    reg %= 0x10;
    *a++ = 0x40 + (obj >> 12);
    *a++ = obj % 4096 < 2048 ? 0xF2 : 0xF6;
    *a++ = obj % 256;
    *a = ((obj >> 8) % 8 << 4) + reg;
}

// CMP.W instruction patch
void Cmp(char* a) {
    getPows();
    unsigned char reg = *a;
    reg %= 0x10;
    *a++ = 0xB0 + reg;
    if(obj < 256) {
        *a++ = 0xF1;
        *a++ = obj;
        *a++ = 0xF;
    } else {
        *a++ = obj < 0xFF0000 ? 0xF5 : 0xF1;
        int base = Pow[upper_bound(Pow.begin(), Pow.end(), obj) - Pow.begin() - 1];
        if(obj < INT_MAX) *a++ = obj / (base >> 7) - ((lower_bound(Pow.begin(), Pow.end(), base >> 9) - Pow.begin() + 1) % 2 << 7);
        else *a++ = 0x0;
        *a++ = 0x7F - ((upper_bound(Pow.begin(), Pow.end(), obj >> 8) - Pow.begin() - 1) >> 1 << 4) - ((obj >> 24) ? 0x80 : 0x0);
    }
}

// A simple function for writing bytes!
void Write(int i, void f(char* a), int n) {
    file.seekg(i, ios::beg);
    char buf[n];
    file.read(buf, n);
    if(f) f(buf);
    file.seekp(i, ios::beg);
    file.write(buf, n);
}

void Write(int i, string a) {
    file.seekp(i, ios::beg);
    for(auto x : a) {
        file.put(x);
    }
}

// mAX oBJECT pATCH :)))
int Patch(unsigned n, bool visual) {
    if(ver < 0 || ver >= vn) return 0;
    unsigned orig = obj;
    if(visual) obj = n;
    switch(lib) {
        case 0:
            if(ver < 8) { // 1.00 - 1.22 (1.00 is compiled differently)
                Write((int[]){0x1C4DAC, 0x14464C, 0x148844, 0x14B454, 0x14D7E0, 0x14F56C, 0x14F884, 0x14FADC}[ver], Mov, ver == 0 ? 4 : 6); // Popup (EditorUI::showMaxError)
                --obj; // The simplest and laziest solution, LOL!
                Write((int[]){0x1C4348, 0x147048, 0x14B240, 0x14DE84, 0x150210, 0x151FB0, 0x1522C8, 0x152520}[ver], Dword); // Object limit (EditorUI::onCreate)
                Write((int[]){0x298B11, 0x1830BF, 0x18A38F, 0x18F027, 0x19341F, 0x195467, 0x195787, 0x1959EF}[ver], {3}); // 100kB level upload fix
                Write((int[]){0x298B27, 0x1830D5, 0x18A3A5, 0x18F03D, 0x193435, 0x19547D, 0x19579D, 0x195A05}[ver], {3}); // (cocos2d::CCString::initWithFormatAndValist)
            } else if(ver < 11) { // 1.3 - 1.41 (Popup uses DWORD)
                Write((int[]){0x1544B0, 0x15861C, 0x15864C}[ver - 8], Dword); // Popup
                --obj;
                Write((int[]){0x156BD8, 0x15BE78, 0x15BEA8}[ver - 8], Dword); // Limit
                Write((int[]){0x1A27D7, 0x1A9BDF, 0x1A9C0F}[ver - 8], {3});
                Write((int[]){0x1A27ED, 0x1A9BF5, 0x1A9C25}[ver - 8], {3});
            } else if(ver < 13){ // 1.50 - 1.51 (Multiple libraries, fixed level uploading)
                Write((int[]){0x1482B8, 0x148478}[ver - 11], Dword);
                --obj;
                Write((int[]){0x149E74, 0x14A034}[ver - 11], Dword);
                Write((int[]){0x14897C, 0x148B3C}[ver - 11], Dword); // EditorUI::onDuplicate
            } else if(ver < 14) { // 1.6 (Popup uses MOV, added object counter)
                Write((int[]){0x168E3A}[ver - 13], Mov); // Object counter (EditorPauseLayer::init)
                Write((int[]){0x150CA4}[ver - 13], Mov);
                --obj;
                Write((int[]){0x1529E4}[ver - 13], Dword);
                Write((int[]){0x1513A4}[ver - 13], Dword);
            } else { // 1.70 - ? (EVERYTHING is DWORD)
                Write((int[]){0x1BA5A4, 0x1BA768, 0x1FB6CC, 0x1FB6CC, 0x1FBAA8}[ver - 14], Dword);
                Write((int[]){0x19DF54, 0x19E058, 0x1DA9C4, 0x1DA9BC, 0x1DADA0}[ver - 14], Dword);
                --obj;
                Write((int[]){0x19FD80, 0x19FF1C, 0x1E2184, 0x1E2180, 0x1E255C}[ver - 14], Dword);
                Write((int[]){0x19E888, 0x19E9BC, 0x1E2BB0, 0x1E2BAC, 0x1E2F7C}[ver - 14], Dword);
            }
            break;
        case 1:
            if(ver < 11) return 0; // Pre-1.5 versions don't have armeabi-v7a lib
            else if(ver < 13) {
                Write((int[]){0x145EEE, 0x1460AA}[ver - 11], Mov); // Popup
                Write((int[]){0x147B1A, 0x147CD6}[ver - 11], (obj <= 0 ? NOP : (string){0x2, (char)0xDD})); // Zero object toggle (Create)
                Write((int[]){0x14659E, 0x14675A}[ver - 11], (obj <= 0 ? NOP : (string){char(0xEA), (char)0xDD})); // (Duplicate)
                if(obj) --obj;
                Write((int[]){0x147B12, 0x147CCE}[ver - 11], Mov); // Limit
                Write((int[]){0x146598, 0x146754}[ver - 11], Mov);
                // What am I doing with my life?....
            } else if(ver < 14) { // This ver and lib uses CMP.W instruction
                if(!CheckApprox(n)) return 2;
                Write((int[]){0x166130}[ver - 13], Mov); // Counter
                Write((int[]){0x14E2C2}[ver - 13], Mov); // Popup
                Write((int[]){0x150050}[ver - 13], (obj <= 0 ? NOP : (string){0x2, (char)0xDD}));
                Write((int[]){0x14E982}[ver - 13], (obj <= 0 ? NOP : (string){char(0xEB), (char)0xDB}));
                obj = n; // The use of approximation
                if(obj) --obj;
                Write((int[]){0x15004A}[ver - 13], Cmp); // Create
                Write((int[]){0x14E97E}[ver - 13], Cmp); // Duplicate
            } else { // 1.7 and newer
                Write((int[]){0x1B683A, 0x1B6A0A, 0x1F6462, 0x1F645A, 0x1F67DE}[ver - 14], Mov);
                Write((int[]){0x19A996, 0x19AAA6, 0x1D6282, 0x1D6276, 0x1D65EA}[ver - 14], Mov);
                string BLE;
                switch(ver) { // you wont believe me but it's more convenient than making a temporary array!!!!
                    case 14:
                    case 15: BLE = {0x2, (char)0xDD}; break;
                    case 16:
                    case 17:
                    case 18: BLE = {0x3, (char)0xDD};
                } // Should have done everything as simple as this :sob:
                Write((int[]){0x19C80E, 0x19C9B2, 0x1DDABC, 0x1DDAB4, 0x1DDE28}[ver - 14], (obj <= 0 ? NOP : BLE));
                switch(ver) {
                    case 14: BLE = {(char)0xE8, (char)0xDD}; break;
                    case 15: BLE = {(char)0xCA, (char)0xDD}; break;
                    case 16:
                    case 17:
                    case 18: BLE = {(char)0xE5, (char)0xDD};
                }
                Write((int[]){0x19B2A8, 0x19B3F4, 0x1DE48C, 0x1DE484, 0x1DE7F8}[ver - 14], (obj <= 0 ? NOP : BLE));
                if(obj) --obj;
                Write((int[]){0x19C806, 0x19C9AA, 0x1DDAB6, 0x1DDAAE, 0x1DDE22}[ver - 14], Mov);
                Write((int[]){0x19B2A2, 0x19B3EE, 0x1DE486, 0x1DE47E}[ver - 14], Mov);
            }
            break;
        case 2:
            if(ver < 11) return 0; // Pre-1.5 versions don't have x86 lib
            else if(ver < 13) {
                Write((int[]){0x19966A, 0x19987A}[ver - 11], Dword); // Popup
                --obj;
                Write((int[]){0x19D3B6, 0x19D5C6}[ver - 11], Dword); // Limit
                Write((int[]){0x19A4A7, 0x19A6B7}[ver - 11], Dword);
            } else if(ver < 14) {
                Write((int[]){0x1DA8B7}[ver - 13], Dword); // Counter
                Write((int[]){0x1A516A}[ver - 13], Dword); // Popup
                --obj;
                Write((int[]){0x1A9106}[ver - 13], Dword); // Create
                Write((int[]){0x1A5FB7}[ver - 13], Dword); // Duplicate
            } else {
                Write((int[]){0x217776, 0x217986, 0x263AC6, 0x263A16, 0x263EA6}[ver - 14], Dword);
                Write((int[]){0x1DA88A, 0x1DA8EA, 0x22C7EE, 0x21E98A, 0x21EDFA}[ver - 14], Dword);
                --obj;
                Write((int[]){0x1DE526, 0x1DE6A6, 0x22C7EE, 0x22C73E, 0x22CBAE}[ver - 14], Dword);
                Write((int[]){0x1DBC97, 0x1DBCFB, 0x22DC17, 0x22DB67, 0x22DFD7}[ver - 14], Dword);
            }
            break;
        default:
            obj = orig;
            return 0;
    }
    obj = orig;
    return 1;
}

/* Let's pretend this has never happened and the file is:
    #include <iostream>
    using namespace std;
    int main() {
        cout << "Hello world!" << endl;
    }
*/// ok? :P
