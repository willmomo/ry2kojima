@echo off

REM -------------------------------------------------------------------
REM sntpsv �̃Z�b�g�A�b�v
REM -------------------------------------------------------------------

set EXENAME=sntpsv
REM set EXENAME=sntpsv64

net stop SntpService
if not exist "%ProgramFiles%\sntpsv" md "%ProgramFiles%\sntpsv"
copy "%~dp0\%EXENAME%.exe" "%ProgramFiles%\sntpsv"
if errorlevel 1 goto ERR010
copy "%~dp0\%EXENAME%.ini" "%ProgramFiles%\sntpsv"
if errorlevel 1 goto ERR020
"%ProgramFiles%\sntpsv\%EXENAME%.exe" install
if errorlevel 1 goto ERR030
sc config w32time start= disabled
if errorlevel 1 goto ERR040
net start SntpService
if errorlevel 1 goto ERR050
schtasks /change /tn "\Microsoft\Windows\Time Synchronization\SynchronizeTime" /disable
if errorlevel 1 goto ERR060

:EOB
pause
goto :EOF

REM -------------------------------------------------------------------
REM �ȍ~�G���[���b�Z�[�W
REM -------------------------------------------------------------------
:ERR010
	echo %~dp0\%EXENAME%.exe �̃R�s�[�Ɏ��s���܂����B
	goto :EOB

:ERR020
	echo %~dp0\%EXENAME%.ini �̃R�s�[�Ɏ��s���܂����B
	goto :EOB
	
:ERR030
	echo �T�[�r�X�̓o�^�Ɏ��s���܂����B
	goto :EOB

:ERR040
	echo w32time ���u�����v�ɂł��܂���ł����B
	goto :EOB
	
:ERR050
	echo SntpService���J�n�ł��܂���B
	goto :EOB

:ERR060
	echo ���v�����^�X�N(SynchronizeTime)���u�����v�ɂł��܂���ł����B
	goto :EOB
	