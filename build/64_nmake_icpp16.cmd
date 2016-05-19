@echo off

if defined ENVIRONMENTREADY goto begin

if defined ICPP_COMPILER16 set INTELDIR=%VS140COMNTOOLS%

set ENVIRONMENTREADY=1

call %ICPP_COMPILER16%bin\compilervars.bat -arch intel64 vs2013
set CC=icl
set CXX=icl

:begin

rmdir /S /Q 64_nmake_icpp16
mkdir 64_nmake_icpp16
cd 64_nmake_icpp16

cmake -G "NMake Makefiles" ../.. -DCMAKE_BUILD_TYPE="RelWithDebInfo"
cmake --build .
cmake --build . --target install

cd ..