@echo off

REM -------------------------------------------------------------------
REM sntpsv �̃Z�b�g�A�b�v
REM 64bit�ł��Z�b�g�A�b�v����Ƃ��́A setup.bat x64 �Ƃ��Ď��s
REM 
REM �Ǘ��Ҍ����Ŏ��s����K�v������Ƃ��́A�ȉ��̃R�}���h���C���������Ă݂�
REM powershell.exe -Command Start-Process -FilePath "<setup.bat�̃t���p�X>" -ArgumentList "x64" -Verb Runas
REM -------------------------------------------------------------------

if "%1" == "x64" (
	set EXENAME=sntpsv64
) else (
	set EXENAME=sntpsv
)

net stop SntpService
if not exist "%ProgramFiles%\sntpsv" md "%ProgramFiles%\sntpsv"
copy "%~dp0\%EXENAME%.exe" "%ProgramFiles%\sntpsv"
if errorlevel 1 goto ERR010
copy "%~dp0\%EXENAME%.ini" "%ProgramFiles%\sntpsv"
if errorlevel 1 goto ERR020
"%ProgramFiles%\sntpsv\%EXENAME%.exe" install
if errorlevel 1 goto ERR030

REM w32time���~
echo $ net stop w32time
net stop w32time

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
	