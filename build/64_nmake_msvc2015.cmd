@echo off

if defined ENVIRONMENTREADY goto begin

if defined VS140COMNTOOLS set MSVCDIR=%VS140COMNTOOLS%

set ENVIRONMENTREADY=1

call "%MSVCDIR%..\..\VC\bin\amd64\vcvars64.bat"

:begin

rmdir /S /Q 64_nmake_msvc2015
mkdir 64_nmake_msvc2015
cd 64_nmake_msvc2015

cmake -G "NMake Makefiles" ../.. -DCMAKE_BUILD_TYPE="RelWithDebInfo"
cmake --build .
cmake --build . --target install

cd ..