@echo off

cd FatToni
call ..\lib\buildwin FatToniPlugin.pro %1
cd ..

pause

cd Grain
call ..\lib\buildwin GrainPlugin.pro %1
cd ..

pause

cd SiliconBonk
call ..\lib\buildwin SiliconBonkPlugin.pro %1
cd ..

pause

cd WaveletDenoise
call ..\lib\buildwin DenoisePlugin.pro %1
cd ..

pause

cd WaveletSharpen
call ..\lib\buildwin SharpenPlugin.pro %1

pause

cd ..
