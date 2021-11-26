@echo off

rem cd %~dp0

set CHERE_INVOKING=1
set MSYSTEM=%~1
set _TERM=%~2
set _SH=%~3
set _ARGS=%~4

REM If MSYSTEM is empty, uses MSYS.
REM Other valid values for MSYSTEM are MINGW32 and MINGW64.
if "x%MSYSTEM%" == "x" set MSYSTEM=MSYS

REM If _TERM is empty we use the default Windows terminal, not sure what it is called.
REM Another valid value is "mintty", but you can use a custom terminal too.
if "x%_TERM%" == "xmintty" set _TERM=%MSYS2_PATH%/usr/bin/mintty --option AppId=msys2 -i %MSYS2_PATH%/msys2.ico

REM If _SH is empty or equal to "bash", we use an interactive, login bash shell.
REM Alternatively, you can use a custom shell.
rem if "x%_SH%" == "x" set _SH=%MSYS2_PATH%/usr/bin/bash -li
rem if "x%_SH%" == "xbash" set _SH=%MSYS2_PATH%/usr/bin/bash --login -i

%_TERM% %_SH% %_ARGS%