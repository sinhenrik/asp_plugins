@echo off
if "%1" == "" goto noargs

nmake clean
qmake -spec C:\Qt\5.4\msvc2013_64\mkspecs\winrt-x64-msvc2013 -config %1nmake
nmake

goto done

:noargs
echo "No Arguments, Not Nice"

:done


