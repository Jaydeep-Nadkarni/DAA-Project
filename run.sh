#!/bin/bash
# Bash script to open commute.exe in a new terminal window on WSL/Linux

cd "$(dirname "$0")"
./commute.exe &
