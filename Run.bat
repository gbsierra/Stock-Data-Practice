@echo off

REM check if Python virtual environment exists
if not exist ".venv" (
    echo Python virtual environment not found. Creating one...
    python -m venv .venv
    echo Python virtual environment created!
)

REM activate virtual env
call .venv\Scripts\activate.bat

REM install required Python modules
pip install -r requirements.txt

REM compile
g++ main.cpp stockData.cpp

REM run compiled program
a.exe

pause