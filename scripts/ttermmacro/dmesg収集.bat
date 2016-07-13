@echo off

echo ----------------------------------------
echo.
echo     %0
echo.
echo ----------------------------------------

setlocal enabledelayedexpansion

set TTPMACRO=C:\tools\TeraTerm\ttpmacro.exe
set MACROPATH=%~dp0

set /p HDB_S=�J�n��ԍ�() : 
set /p HDB_E=�I����ԍ�() : 

if "%HDB_S%" == "" set HDB_S=1
if "%HDB_E%" == "" set HDB_E=1

if not exist C:\ftp\dmesg���W mkdir C:\ftp\dmesg���W
del /q C:\ftp\dmesg���W\*.*

for /l %%i in (%HDB_S%,1,%HDB_E%) do (
	set /a IP3=%%i / 100
	set /a IP4=%%i %% 100
	if !IP4! equ 0 set IP4=100

	rem SD�̃f�[�^�����W����
	call :DO_MAIN %%i
)

goto :EOF

:DO_MAIN
	echo %1 = 172.22.!IP3!.!IP4!
	echo %TTPMACRO% %MACROPATH%dmesg���W.ttl 172.22.!IP3!.!IP4! /Gnavi%1
	     %TTPMACRO% %MACROPATH%dmesg���W.ttl 172.22.!IP3!.!IP4! /Gnavi%1
	exit /b
