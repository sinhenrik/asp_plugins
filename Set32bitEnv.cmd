echo off

echo "Calling QT 5.4 32-bit setup"
call "C:\Qt\5.4\msvc2013\bin\qtenv2.bat"

echo "Calling MSVC 2013 32-bit setup"
Call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86


cd %~f0\..