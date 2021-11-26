@echo off

set _BUILD=Release
set _TOOLCHAIN=MINGW64
set _BUILD_SCRIPT=build-msys-cmake.sh
set _SCRIPT_PATH=%~dp0
set _PACKAGE_TARGET_NAME=%~1

%_SCRIPT_PATH%build-windows-msys2.bat %_BUILD% %_TOOLCHAIN% %_BUILD_SCRIPT% %_PACKAGE_TARGET_NAME%