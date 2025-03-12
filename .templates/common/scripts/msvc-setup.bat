@echo off

@REM [NOTE] Change the path to your Visual Studio 2022 installation
set "VS2022_PATH=%ProgramFiles%\Microsoft Visual Studio\2022"
if not exist "%VS2022_PATH%" (
    echo Visual Studio 2022 not found at: %VS2022_PATH%
    exit /b 1
)

if exist "%VS2022_PATH%\Community" (
    set "VS2022_PATH=%VS2022_PATH%\Community"
    goto END
)
if exist "%VS2022_PATH%\Professional" (
    set "VS2022_PATH=%VS2022_PATH%\Professional"
    goto END
)
if exist "%VS2022_PATH%\Enterprise" (
    set "VS2022_PATH=%VS2022_PATH%\Enterprise"
    goto END
)

echo No Valid Visual Studio 2022 edition (Community/Professional/Enterprise) found at: %VS2022_PATH%
exit /b 1

:END

set "ARCH=x86_amd64"

set "VCVARSALL=%VS2022_PATH%\VC\Auxiliary\Build\vcvarsall.bat"
echo "[msvc-setup.bat] Setting up Visual Studio environment for %ARCH%..."
call "%VCVARSALL%" %ARCH%