@echo off

REM -------------------------------------------------------------------
REM ElectronicObserver(���l���d�q�ϑ��V��l�^)�̏W�߂� swf �t�@�C����
REM HugFlash ���g���ēW�J
REM ���̌�A����ȃt�@�C������ύX���� BAT�t�@�C��
REM -------------------------------------------------------------------

setlocal enabledelayedexpansion

set BASENAME=%~n1
if exist %~dpn1 rd /s /q %~dpn1
C:\Users\kojima\SkyDrive\tools\hugflash2_9_4\hugflash.exe "%1"
if exist %~dpn1\BMP\PNG\!BASENAME![12].png (
	start %~dpn1\BMP\PNG\!BASENAME![12].png
	set /p NEWNAME=�͂ނ���: 
	echo renall.wsf /p:%~dp1 /s !BASENAME! "!NEWNAME!"
	renall.wsf /p:%~dp1 /s !BASENAME! "!NEWNAME!"
) else (
	echo %~nx1 �́A�͂ނ��ł͖����悤�ł��B
)
