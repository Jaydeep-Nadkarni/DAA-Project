@echo off
REM Build and run `commute.exe` in a new console window (Windows)

REM Change to script directory (project root)
cd /d "%~dp0"

echo Building project...
g++ src/*.cpp -I include -o commute.exe -std=c++11
if errorlevel 1 (
    echo.
    echo Build failed. Fix the errors and try again.
    pause
    exit /b 1
)

echo Launching program in a new window...
REM Start a new cmd window, change to project folder and run the exe, keep window open after program exits
start "Commute" cmd /k "cd /d \"%~dp0\" && .\commute.exe & echo. & pause"

exit /b 0
