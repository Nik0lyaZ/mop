#include "main.h"
using namespace std;
int main()
{
    string s;
    cout << "Game version?\n";
    do {
        getline(cin, s);
        if(s.size() < 3) {
            s = '0' + s;
            if(stoi(s) > 0 && stoi(s) <= vn)
                ver = stoi(s) - 1;
            else {
                cout << "Invalid version number. (1-" << vn << ")\n";
                continue;
            }
        } else {
            while(s[s.size() - 1] == '0' && s[s.size() - 2] != '.')
                s.erase(s.size() - 1);

            ver = (find(VER, VER + vn, s) - VER);
            if(!(ver - vn)) {
                cout << "Invalid version name. (1.0, ..., " << VER[vn - 1] << ")\n";
                continue;
            }
        }
        if(SUPPORT[ver]) break;
            else cout << "Currently unsupported, contact Nikolyas.\n";
    }
    while(1);


    cout << "New object limit?\n";
    cin >> obj;
    if(obj >= INT_MAX) cout << "NOTICE: This limit will work differently depending on lib and version.\n";
    if(obj > 65535) cout << "NOTICE: Some libraries cap the limit at 65535.\n";
    if(obj > 16384) cout << "NOTICE: Objects placed after 16384 will be invisible in editor.\n";

    const string LIB[3] = {"armeabi", "armeabi-v7a", "x86"};
    const string name = ver < 14 ? "libgame.so" : "libcocos2dcpp.so"; // Different name since 1.7
    string path = name;
    bool found = 0, local = 0, patched = 0, visual = 1;
    unsigned approx = obj;
    file.open(path, ios::in | ios::out | ios::binary);
    if(file) local = 1;
        else file.close();
    for(lib = 0; lib < 3; ++lib) {
        if(!local) {
            path = LIB[lib] + '/' + name;
            file.open(path, ios::in | ios::out | ios::binary);
        } else if(ver >= 11) {
            cout << "Is this armeabi, armeabi-v7a or x86? (1-3)\n";
            do cin >> lib;
            while(lib < 1 || lib > 3);
            --lib;
        }
        if(!file) {
            file.close();
            file.open("lib/" + path, ios::in | ios::out | ios::binary);
        }
        if(file) {
            found = 1;
            int result = Patch();
            switch(result) {
                case 1:
                    patched = 1;
                    break;
                case 2:
                    cout << "Approximation problem found in " + path + '\n';
                    if(approx == obj) {
                        bool type;
                        cout << "0 - use " << Approx(0) << ", 1 - use " << Approx(1) << '\n';
                        cin >> type;
                        cout << "Do you want to use approximations in the object counter and popup as well? (0-1)\n";
                        cin >> visual;
                        approx = Approx(type);
                    }
                    if(Patch(approx, visual) == 1) patched = 1;
                    else cout << "Could not apply the approximations\n";
                    break;
                default:
                    cout << "Could not patch " << path << "\nResult: " << result << '\n';
            }
        } else {
            cout << "Could not find " << path << '\n';
        }
        file.close();
        if(local || ver < 11) break;
    }

    if(found && !patched) cout << "Failed. Please contact Nikolyas for more info.\n";
    else if(found || local) pause("Success! " + PAUSE_TEXT, 1);
    else cout << "Failed. Make sure there is a lib folder or file!\n";
    pause();
}
