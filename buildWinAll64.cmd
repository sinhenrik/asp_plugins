call "C:\Program Files\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86
rem cd aLiCE
rem call ..\lib\buildwin64 %1
cd ..\FatToni
call ..\lib\buildwin64 %1
rem cd ..\Gelatin
rem call ..\lib\buildwin64 %1
cd ..\Grain
call ..\lib\buildwin64 %1
cd ..\SiliconBonk
call ..\lib\buildwin64 %1
cd ..\WaveletDenoise
call ..\lib\buildwin64 %1
cd ..\WaveletSharpen
call ..\lib\buildwin64 %1
pause
