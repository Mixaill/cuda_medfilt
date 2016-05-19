if defined ENVIRONMENTREADY goto begin

if defined VS140COMNTOOLS set MSVCDIR=%VS140COMNTOOLS%
if defined VS140COMNTOOLS set GENERAT="Visual Studio 14 2015"

set ENVIRONMENTREADY=1

call "%MSVCDIR%\vsvars32.bat"

:begin

rmdir /S /Q 32_msvs_2015
mkdir 32_msvs_2015
cd 32_msvs_2015

cmake -G %GENERAT% ../..
cmake --build . --config RelWithDebInfo
cmake --build . --config RelWithDebInfo --target install

cd ..