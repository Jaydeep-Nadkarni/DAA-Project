# PowerShell script to open commute.exe in a new window
# Usage: .\run.ps1

$projectPath = Get-Location

# Open a new PowerShell window and run commute.exe
Start-Process "$projectPath\commute.exe"
