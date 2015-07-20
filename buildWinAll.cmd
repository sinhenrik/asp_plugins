call "C:\Program Files\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
rem cd aLiCE
rem call buildwin %1
cd ..\FatToni
call buildwin %1
rem cd ..\Gelatin
rem call buildwin %1
cd ..\Grain
call buildwin %1
cd ..\SiliconBonk
call buildwin %1
cd ..\WaveletDenoise
call buildwin %1
cd ..\WaveletSharpen
call buildwin
pause
