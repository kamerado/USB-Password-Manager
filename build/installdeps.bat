@echo off
setlocal

REM Set download URL and target path
set DOWNLOAD_URL=https://download.qt.io/official_releases/online_installers/qt-unified-windows-x64-online.exe
set TARGET_PATH=%~dp0qt-unified-windows-x64-online.exe

REM Download Qt installer using PowerShell
echo Downloading Qt installer...
powershell -Command "Invoke-WebRequest -Uri %DOWNLOAD_URL% -OutFile %TARGET_PATH%"

if errorlevel 1 (
    echo Download failed
    exit /b 1
)

REM Run the installer with parameters
echo Running Qt installer...
"%TARGET_PATH%" --root %userprofile%\Qt --accept-licenses --default-answer --confirm-command install qt.qt6.680.win64_mingw qt.qt6.680.addons.qtwebsockets

if errorlevel 1 (
    echo Installation failed
    exit /b 1
)

REM Navigate to libs directory and initialize git submodules
echo Initializing git submodules...
cd /d "%~dp0..\libs"
git submodule update --init --recursive

if errorlevel 1 (
    echo Git submodule initialization failed
    exit /b 1
)

echo Installation completed successfully
endlocal