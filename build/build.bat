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
set PROJECT_DIR=C:\Users\user\Projects\USB-Password-Manager
set BUILD_DIR=%PROJECT_DIR%\build\Desktop_Qt_6_8_0_MinGW_64_bit-Debug

REM Create build directory if it doesn't exist
IF NOT EXIST "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
)

echo Installing dependencies...
call "%PROJECT_DIR%\build\installdeps.bat"

REM Set working directory to build output
cd /d "%~dp0"

REM Configure CMake with Qt settings
cd /d "%BUILD_DIR%"
cmake -G "MinGW Makefiles" ^
    -DCMAKE_PREFIX_PATH="%QT_DIR%" ^
    -DCMAKE_C_COMPILER="%MINGW_DIR%\bin\gcc.exe" ^
    -DCMAKE_CXX_COMPILER="%MINGW_DIR%\bin\g++.exe" ^
    -DCMAKE_MAKE_PROGRAM="%MINGW_DIR%\bin\mingw32-make.exe" ^
    -DCMAKE_BUILD_TYPE=Debug ^
    "%PROJECT_DIR%"
cd ..
REM Build project
echo Building project...
"C:\Program Files\CMake\bin\cmake.exe" --build "C:\Users\user\Projects\USB-Password-Manager\build\Desktop_Qt_6_8_0_MinGW_64_bit-Debug" --target all
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
if not exist "libspdlogd.a" (
    echo Error: libspdlogd.a missing after deployment
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
start /b "" "%PROJECT_DIR%\build\nativegui.exe"
if not errorlevel 0 (
    echo Failed to start application: Error %errorlevel%
    exit /b 1
)

endlocal
