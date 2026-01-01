@echo off
REM Launcher for Mumbai Local Transportation Management System
REM Opens commute.exe in a new external console window (like Code::Blocks)

cd /d "%~dp0"

REM Check if executable exists
if not exist "commute.exe" (
    echo Executable not found. Building project...
    g++ src/*.cpp -I include -o commute.exe -std=c++11
    if errorlevel 1 (
        echo Build failed!
        pause
        exit /b 1
    )
)

REM Launch in new cmd window with UTF-8 encoding
start "Mumbai Local Transportation System" cmd /k "chcp 65001 >nul && commute.exe && echo. && pause"
exit /b 0
