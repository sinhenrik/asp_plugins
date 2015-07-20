echo off
%~d0
cd %~f0\..
cd FatToni
call ..\lib\buildwin64 %1
cd ..
cd Grain
call ..\lib\buildwin64 %1
cd ..
cd SiliconBonk
call ..\lib\buildwin64 %1
cd ..
cd WaveletDenoise
call ..\lib\buildwin64 %1
cd ..
cd WaveletSharpen %1
call ..\lib\buildwin64
pause
