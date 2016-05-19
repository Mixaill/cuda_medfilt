@echo off

if defined ENVIRONMENTREADY goto begin

if defined VS120COMNTOOLS set MSVCDIR=%VS120COMNTOOLS%
if defined VS120COMNTOOLS set GENERAT="Visual Studio 12 2013 Win64" 

set ENVIRONMENTREADY=1

call "%MSVCDIR%..\..\VC\bin\amd64\vcvars64.bat"

:begin

rmdir /S /Q 64_msvs_msvc2013
mkdir 64_msvs_msvc2013
cd 64_msvs_msvc2013

cmake -G %GENERAT% ../..
cmake --build . --config RelWithDebInfo
cmake --build . --config RelWithDebInfo --target install

cd ..