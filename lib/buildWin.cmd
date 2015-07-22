@echo off

if not exist "%1%" goto noproject
if "%2" == "" goto noargs

set CURR_DIR=%CD%
echo Building from %CURR_DIR%

set PROJECT=%~f1

if not exist "..\Builds" mkdir "..\Builds"
if not exist "..\Builds\Windows32" mkdir "..\Builds\Windows32"

rem 
cd "..\Builds\Windows32"

echo Building %PROJECT% in %CD%

qmake %PROJECT% -r -spec win32-msvc2013

if "%2" == "debug" C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Debug
if "%2" == "release" C:\Qt\Tools\QtCreator\bin\jom.exe -f Makefile.Release

cd %CURR_DIR%

goto done

:noargs
echo No Build Arguments, type release or debug
goto done

:noproject
echo First argument must be project
goto done

:done


