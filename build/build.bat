@echo off
setlocal

REM Verify required directories exist
if not exist "C:\Users\user\Qt\6.8.0\mingw_64" (
    echo Error: Qt directory not found
    exit /b 1
)
if not exist "C:\Users\user\Qt\Tools\mingw1310_64" (
    echo Error: MinGW directory not found
    exit /b 1
)

REM Set Qt and MinGW paths
set QT_DIR=C:\Users\user\Qt\6.8.0\mingw_64
set MINGW_DIR=C:\Users\user\Qt\Tools\mingw1310_64
set PATH=%PATH%;%QT_DIR%\bin;%MINGW_DIR%\bin

REM Set working directory to build output
cd /d "%~dp0"

REM Build project
echo Building project...
"C:\Program Files\CMake\bin\cmake.exe" --build "C:/Users/user/Projects/USB-Password-Manager/build/Desktop_Qt_6_8_0_MinGW_64_bit-Debug" --target all
if errorlevel 1 (
    echo Build failed - Check CMake output for details
    exit /b 1
)

REM Deploy Qt dependencies
echo Deploying Qt dependencies...
if not exist "nativegui.exe" (
    echo Error: nativegui.exe not found
    exit /b 1
)

REM Verify critical DLLs
if not exist "libspdlogd.dll" (
    echo Error: libspdlogd.dll missing after deployment
    exit /b 1
)

"%QT_DIR%\bin\windeployqt.exe" "nativegui.exe"
if errorlevel 1 (
    echo Qt deployment failed
    exit /b 1
)

echo Build and deployment completed successfully

REM Run the application
echo Starting application...
start /b "" "nativegui.exe"
if not errorlevel 0 (
    echo Failed to start application: Error %errorlevel%
    exit /b 1
)

endlocal
