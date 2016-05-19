@echo off

if defined ENVIRONMENTREADY goto begin

if defined VS120COMNTOOLS set MSVCDIR=%VS120COMNTOOLS%

set ENVIRONMENTREADY=1

call "%MSVCDIR%\vsvars32.bat"

:begin

rmdir /S /Q 32_msvs_2013_nmake
mkdir 32_msvs_2013_nmake
cd 32_msvs_2013_nmake

cmake -G "NMake Makefiles" ../..
cmake --build . --config RelWithDebInfo
cmake --build . --config RelWithDebInfo --target install

cd ..