@echo off

echo ----------------------------------------
echo.
echo     %0
echo.
echo ----------------------------------------

setlocal enabledelayedexpansion

set TTPMACRO=C:\tools\TeraTerm\ttpmacro.exe
set MACROPATH=%~dp0

set /p HDB_S=開始台番号() : 
set /p HDB_E=終了台番号() : 

if "%HDB_S%" == "" set HDB_S=1
if "%HDB_E%" == "" set HDB_E=1

if not exist C:\ftp\dmesg収集 mkdir C:\ftp\dmesg収集
del /q C:\ftp\dmesg収集\*.*

for /l %%i in (%HDB_S%,1,%HDB_E%) do (
	set /a IP3=%%i / 100
	set /a IP4=%%i %% 100
	if !IP4! equ 0 set IP4=100

	rem SDのデータを収集する
	call :DO_MAIN %%i
)

goto :EOF

:DO_MAIN
	echo %1 = 172.22.!IP3!.!IP4!
	echo %TTPMACRO% %MACROPATH%dmesg収集.ttl 172.22.!IP3!.!IP4! /Gnavi%1
	     %TTPMACRO% %MACROPATH%dmesg収集.ttl 172.22.!IP3!.!IP4! /Gnavi%1
	exit /b
