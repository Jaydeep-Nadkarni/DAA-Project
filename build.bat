@echo off
REM ======================================================================================
REM BUILD SCRIPT: Mumbai Local Transportation Management System (Windows)
REM ======================================================================================
REM This batch file compiles the project on Windows using g++ (MinGW/MSYS2)
REM ======================================================================================

echo.
echo ========================================================================
echo    MUMBAI LOCAL TRANSPORTATION MANAGEMENT SYSTEM - BUILD SCRIPT
echo ========================================================================
echo.

REM Check if g++ is available
where g++ >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: g++ compiler not found!
    echo Please install MinGW or MSYS2 and add to PATH
    echo.
    echo Download from:
    echo   MinGW: https://sourceforge.net/projects/mingw/
    echo   MSYS2: https://www.msys2.org/
    pause
    exit /b 1
)

echo [1/3] Compiler found: g++
g++ --version | findstr "g++"
echo.

REM Create obj directory if it doesn't exist
if not exist obj mkdir obj

echo [2/3] Compiling source files...
echo.

REM Compile all source files
g++ -c src\main.cpp -I include -o obj\main.o -std=c++11 -O2 -Wall
if %ERRORLEVEL% NEQ 0 goto :error

g++ -c src\station.cpp -I include -o obj\station.o -std=c++11 -O2 -Wall
if %ERRORLEVEL% NEQ 0 goto :error

g++ -c src\graph.cpp -I include -o obj\graph.o -std=c++11 -O2 -Wall
if %ERRORLEVEL% NEQ 0 goto :error

g++ -c src\ticketing.cpp -I include -o obj\ticketing.o -std=c++11 -O2 -Wall
if %ERRORLEVEL% NEQ 0 goto :error

g++ -c src\scheduling.cpp -I include -o obj\scheduling.o -std=c++11 -O2 -Wall
if %ERRORLEVEL% NEQ 0 goto :error

g++ -c src\queue_manager.cpp -I include -o obj\queue_manager.o -std=c++11 -O2 -Wall
if %ERRORLEVEL% NEQ 0 goto :error

g++ -c src\analytics.cpp -I include -o obj\analytics.o -std=c++11 -O2 -Wall
if %ERRORLEVEL% NEQ 0 goto :error

g++ -c src\csv_manager.cpp -I include -o obj\csv_manager.o -std=c++11 -O2 -Wall
if %ERRORLEVEL% NEQ 0 goto :error

echo.
echo [3/3] Linking executable...

REM Link all object files
g++ obj\*.o -o commute.exe -std=c++11
if %ERRORLEVEL% NEQ 0 goto :error

echo.
echo ========================================================================
echo    BUILD SUCCESSFUL!
echo ========================================================================
echo.
echo Executable created: commute.exe
echo.
echo To run the program, type: commute.exe
echo.
pause
exit /b 0

:error
echo.
echo ========================================================================
echo    BUILD FAILED!
echo ========================================================================
echo.
echo Please check the error messages above and fix any compilation issues.
echo.
pause
exit /b 1
