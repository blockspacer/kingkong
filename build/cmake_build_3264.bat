@echo off

set PATH=%PATH%;../build/cmake/windows/bin
cd ..
mkdir tmp_3264

cd tmp_3264
cmake .. -G"Visual Studio 16 2019" -A Win32 -DCMAKE_CONFIGURATION_TYPES=Debug;Release;Release_MD
pause
@echo on
