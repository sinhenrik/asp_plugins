@echo off
if "%1" == "" goto noargs

nmake clean
qmake -spec C:\Qt\5.4\msvc2013\mkspecs\winrt-x86-msvc2013 -config %1
nmake

goto done

:noargs
echo "No Arguments, Not Nice"

:done

