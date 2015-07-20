echo off

echo Calling QT 5.4 64-bit setup
call "C:\Qt\5.4\msvc2013_64\bin\qtenv2.bat"
echo Calling MSVC 2013 32-bit setup
Call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86_amd64

cd %~f0\..