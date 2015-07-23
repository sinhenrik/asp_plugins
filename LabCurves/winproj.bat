@echo off

rem Remove old files
rmdir /S /Q build
del /Q lib*.*
rem Generate Visual Studio project
qmake.exe -o LabCurves.vcproj -t vclib LabCurves.pro
