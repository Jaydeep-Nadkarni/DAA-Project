@echo off
REM Open Commute Application in New Window
REM This batch file launches the commute.exe in a separate window

cd /d "%~dp0"
start "" main.exe
