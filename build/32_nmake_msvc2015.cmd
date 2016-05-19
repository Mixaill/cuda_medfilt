@echo off

if defined ENVIRONMENTREADY goto begin

if defined VS140COMNTOOLS set MSVCDIR=%VS140COMNTOOLS%

set ENVIRONMENTREADY=1

call "%MSVCDIR%\vsvars32.bat"

:begin

rmdir /S /Q 32_msvs_2015_nmake
mkdir 32_msvs_2015_nmake
cd 32_msvs_2015_nmake

cmake -G "NMake Makefiles" ../.. -DCMAKE_BUILD_TYPE="RelWithDebInfo"
cmake --build .
cmake --build . --target install

cd ..