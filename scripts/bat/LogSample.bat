@echo off
call :LogWrite "-------- start %~f0 --------"

for /L %%i in (1,1,99999) do (
	call :LogWrite "%%i %TEMP%\%~n0.log"
)

call :LogWrite "-------- end %~f0 --------"
goto :EOF

REM -------------------------------------------------------------------
REM ���O�o�͏���(��ʂɂ��\��)
REM -------------------------------------------------------------------
:LogEcho
	setlocal enabledelayedexpansion
	echo %*
	echo %DATE% %TIME% %*>>"%TEMP%\%~n0.log"
	exit /b

REM -------------------------------------------------------------------
REM ��ԃV���v���ȃ��O�o�͏���
REM -------------------------------------------------------------------
:LogWrite
	setlocal enabledelayedexpansion
	echo %DATE% %TIME% %*>>"%TEMP%\%~n0.log"
	exit /b

REM -------------------------------------------------------------------
REM ���O�t�@�C�����V�t�g���������Ƃ��ɌĂԏ���
REM -------------------------------------------------------------------
:LogShift
	setlocal enabledelayedexpansion
	for %%i in ("%TEMP%\%~n0.log") do (
		if %%~zi gtr 1048576 (
			for /L %%j in (8,-1,1) do (
				set /A K=%%j+1
				move /Y "%TEMP%\%~n0_%%j.log" "%TEMP%\%~n0_!K!.log"
			)
			move /Y "%TEMP%\%~n0.log" "%TEMP%\%~n0_1.log"
		)
	)
	exit /b
