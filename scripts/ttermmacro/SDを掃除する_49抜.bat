@echo off

echo ----------------------------------------
echo.
echo     %0
echo.
echo ----------------------------------------

setlocal enabledelayedexpansion

set TTPMACRO=C:\tools\TeraTerm\ttpmacro.exe
set MACROPATH=%~dp0

set /p HDB_S=äJénë‰î‘çÜ() : 
set /p HDB_E=èIóπë‰î‘çÜ() : 

if "%HDB_S%" == "" set HDB_S=1
if "%HDB_E%" == "" set HDB_E=1

for /l %%i in (%HDB_S%,1,%HDB_E%) do (
	echo %%i|findstr "4 9">nul
	if !ERRORLEVEL! neq 0 (
		set /a IP3=%%i / 100
		set /a IP4=%%i %% 100
		if !IP4! equ 0 set IP4=100

		call :DO_MAIN %%i
	)
)

goto :EOF

:DO_MAIN
	echo %1 = 172.22.!IP3!.!IP4!
	echo %TTPMACRO% %MACROPATH%SDÇë|èúÇ∑ÇÈ.ttl 172.22.!IP3!.!IP4!
	     %TTPMACRO% %MACROPATH%SDÇë|èúÇ∑ÇÈ.ttl 172.22.!IP3!.!IP4!
	exit /b
