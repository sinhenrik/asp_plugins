echo off
cd FatToni
call ..\lib\buildwin %1
cd ..
pause
cd Grain
call ..\lib\buildwin %1
cd ..
pause
cd SiliconBonk
call ..\lib\buildwin %1
cd ..
pause
cd WaveletDenoise
call ..\lib\buildwin %1
cd ..
pause
cd WaveletSharpen %1
call ..\lib\buildwin

pause
