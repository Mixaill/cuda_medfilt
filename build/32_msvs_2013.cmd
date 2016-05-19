if defined ENVIRONMENTREADY goto begin

if defined VS120COMNTOOLS set MSVCDIR=%VS120COMNTOOLS%
if defined VS120COMNTOOLS set GENERAT="Visual Studio 12 2013"

set ENVIRONMENTREADY=1

call "%MSVCDIR%\vsvars32.bat"

:begin

rmdir /S /Q 32_msvs_2013
mkdir 32_msvs_2013
cd 32_msvs_2013

cmake -G %GENERAT% ../..
cmake --build . --config RelWithDebInfo
cmake --build . --config RelWithDebInfo --target install

cd ..