@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
cl /EHsc /O2 /Fe:build.exe build.cpp
del /f/q build.obj >nul 2>&1
