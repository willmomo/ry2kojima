@echo off

REM -------------------------------------------------------------------
REM ElectronicObserver(七四式電子観測儀一四型)の集めた swf ファイルを
REM HugFlash を使って展開
REM その後、難解なファイル名を変更する BATファイル
REM -------------------------------------------------------------------

setlocal enabledelayedexpansion

set BASENAME=%~n1
if exist %~dpn1 rd /s /q %~dpn1
C:\Users\kojima\SkyDrive\tools\hugflash2_9_4\hugflash.exe "%1"
if exist %~dpn1\BMP\PNG\!BASENAME![12].png (
	start %~dpn1\BMP\PNG\!BASENAME![12].png
	set /p NEWNAME=艦むす名: 
	echo renall.wsf /p:%~dp1 /s !BASENAME! "!NEWNAME!"
	renall.wsf /p:%~dp1 /s !BASENAME! "!NEWNAME!"
) else (
	echo %~nx1 は、艦むすでは無いようです。
)
