@echo off

if defined ENVIRONMENTREADY goto begin

if defined VS120COMNTOOLS set MSVCDIR=%VS120COMNTOOLS%

set ENVIRONMENTREADY=1

call "%MSVCDIR%..\..\VC\bin\amd64\vcvars64.bat"

:begin

rmdir /S /Q 64_nmake_msvc2013
mkdir 64_nmake_msvc2013
cd 64_nmake_msvc2013

cmake -G "NMake Makefiles" ../.. -DCMAKE_BUILD_TYPE="RelWithDebInfo"
cmake --build .
cmake --build . --target install

cd ..