@echo off

cd FatToni
call ..\lib\buildwin64 FatToniPlugin.pro %1
cd ..

pause

cd Grain
call ..\lib\buildwin64 GrainPlugin.pro %1
cd ..

pause

cd SiliconBonk
call ..\lib\buildwin64 SiliconBonkPlugin.pro %1
cd ..

pause

cd WaveletDenoise
call ..\lib\buildwin64 DenoisePlugin.pro %1
cd ..

pause

cd WaveletSharpen
call ..\lib\buildwin64 SharpenPlugin.pro %1

pause

cd ..