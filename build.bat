@echo off
::Set Environments for X86_64 build
cd %~dp0
call "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
where cl.exe link.exe

::Set Environment Variables
set NAME=WslReverse.exe
set BINDIR="bin"
set CFLAGS=/c /nologo /O1 /MD /W4 /Fo%BINDIR%\\
set LFLAGS=/nologo /MACHINE:X64
set LIBS=Ole32.lib Shell32.lib NtDll.lib

::Disable warnings
set CCOPT=/wd"4201" /wd"4214"

::Build
rd /s /q %BINDIR%
mkdir %BINDIR%
cl.exe %CFLAGS% %CCOPT% src\*.c
link.exe %LFLAGS% %LIBS% %BINDIR%\*.obj /OUT:%BINDIR%\%NAME%

dir /B %BINDIR%\*.exe
pause
exit /b

::END#