@echo off

set _BUILD=%~1
set _TOOLCHAIN=%~2
set _BUILD_SCRIPT=%~3
set _PACKAGE_TARGET_NAME=%~4

set _PROJECT_PATH=%~dp0/..
set _SCRIPT_PATH=%~dp0

if not exist "%_PROJECT_PATH%/scripts/" (
    echo "Please run the script from the one level up of the project directory!"
    rem sleep 10000
    rem exit
)

rem cd %~dp0

rem Set a path to
set MSYS2_PATH=C:/msys64

if exist "%MSYS2_PATH%/" (
    echo "MSYS2 is found: %MSYS2_PATH%"
) else (
    echo "%MSYS2_PATH% is not found! Please install MSYS2"
)

rem for the shell to stay in the current working directory.
set CHERE_INVOKING=1

rem run
%_SCRIPT_PATH%/msys2_env.bat %_TOOLCHAIN% mintty "%MSYS2_PATH%/usr/bin/bash -li %_SCRIPT_PATH%/build_script_wrapper.sh" "%_PROJECT_PATH%/scripts/%_BUILD_SCRIPT% %_BUILD% %_PROJECT_PATH% %_PACKAGE_TARGET_NAME%"