### Max object patcher 1.811 ### (ver 8)
1. Extract Geometry Dash apk with WinRAR, 7-zip or your APK tool
2. Put mop executable in the game files and run it
3. Choose the game version by typing 1.0, 1.01, 1.02, etc.
4. Specify the object limit. New objects after 16384 are invisible in editor
5. Sign apk with an APK tool and enjoy!

### Notice ###
The project is being discontinued as I'm working on a GUI version called mop_2.
However, the console version will still receive bugfixes.
Thanks for using mop :P

### Extra ###
- Supported GD versions: 1.00 - 1.811 (Android)
- Run mop32 on 32-bit Windows
- Using a high object limit may lead to issues
- Share feedback with @nikolyas on Discord

### Changelog ###
2025-06-30:
- The object limit for 1.6 armeabi-v7a library uses proper values now!
- Added .gitignore for future commits

2025-06-21:
- Added the missing 1.811 address to onDuplicate array for armeabi-v7a  

2024-10-25:
- Minor fixes to 1.50-1.811 armeabi-v7a code
- Wokring on mop_2! It currently supports versions up to 1.6

2024-09-22:
- I'm dumb because I messed up the switch case thing

2024-09-14:
- Updated warnings when using a high object limit

2024-08-25:
- The limit of 10 objects works properly now
- Added debug related thingy you will never need

2024-08-10:
- Fixed armeabi-v7a patch fail in 1.6

2024-08-03 (1.811):
- Added pause.cpp and pause.h, improved pause function
- Added main.h for the debug check (#define BUILD_DEBUG)
- Full zero object support!
- Now you don't have to run the program in the lib directory
- Changed success message
- Fixed bugs in 1.50-1.51 x86 library
- Fixed approximation bug in code
- Versions 1.800 - 1.811 are supported now

https://drive.google.com/drive/folders/1cQoe9X9RORDKVlq_WoOCJgwMB52dDFB1