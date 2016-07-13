@echo off

REM -------------------------------------------------------------------
REM sntpsv のセットアップ
REM -------------------------------------------------------------------

net stop SntpService
if not exist "%ProgramFiles%\sntpsv" md "%ProgramFiles%\sntpsv"
copy "%~dp0\sntpsv.exe" "%ProgramFiles%\sntpsv"
if errorlevel 1 goto ERR010
copy "%~dp0\sntpsv.ini" "%ProgramFiles%\sntpsv"
if errorlevel 1 goto ERR020
"%ProgramFiles%\sntpsv\sntpsv.exe" install
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
REM 以降エラーメッセージ
REM -------------------------------------------------------------------
:ERR010
	echo %~dp0\sntpsv.exe のコピーに失敗しました。
	goto :EOB

:ERR020
	echo %~dp0\sntpsv.ini のコピーに失敗しました。
	goto :EOB
	
:ERR030
	echo サービスの登録に失敗しました。
	goto :EOB

:ERR040
	echo w32time を「無効」にできませんでした。
	goto :EOB
	
:ERR050
	echo SntpServiceを開始できません。
	goto :EOB

:ERR060
	echo 時計合せタスク(SynchronizeTime)を「無効」にできませんでした。
	goto :EOB
	