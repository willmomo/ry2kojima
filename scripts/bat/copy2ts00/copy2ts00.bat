@echo off
setlocal enabledelayedexpansion

set MAILSERV=sec.sanko-net.co.jp
set MAILCMD=%~dp0smail.exe
set MIRCMD=%~dp0mirror.exe
set MIREXL=%~dp0exclude.txt

if "%1" == "clean" (
	if exist "!TEMP!\%~n0_mail.txt" del "!TEMP!\%~n0_mail.txt"
	echo ���[���A�h���X�������폜���܂����B
	goto :EOF
)

if not exist "!TEMP!\%~n0_mail.txt" (
	set /p MAILADDR=���Ȃ��̃��[���A�h���X����͂��Ă�������: 
	echo !MAILADDR!>"!TEMP!\%~n0_mail.txt"
	echo ���[���A�h���X��ۑ����܂����B
	goto :EOF
)

for /f %%i in (!TEMP!\%~n0_mail.txt) do (
	set MAILADDR=%%i
)

"!MIRCMD!" --mode=backup-mirror --exclude="!MIREXL!" --logpath=!TEMP!\logmir "!USERPROFILE!\Documents" "\\ts00\home\!USERNAME!\!COMPUTERNAME!\Documents"

if errorlevel 1 (
	echo %~nx0 - �ُ�I�� >!TEMP!\%~n0_body.txt
	echo. >>!TEMP!\%~n0_body.txt
	echo �G���[���������܂����B >>!TEMP!\%~n0_body.txt
	echo �ڍׂ� `!TEMP!\logmir` ���Q�Ƃ��Ă��������B >>!TEMP!\%~n0_body.txt
	
	"!MAILCMD!" -f!MAILADDR! -h!MAILSERV! -s"%~nx0 - �ُ�I��" -F"!TEMP!\%~n0_body.txt" !MAILADDR!
) else (
	echo %~nx0 - ����I�� >!TEMP!\%~n0_body.txt
	echo. >>!TEMP!\%~n0_body.txt
	echo ����I�����܂����B >>!TEMP!\%~n0_body.txt
	echo �����̏ڍׂ� `!TEMP!\logmir` ���Q�Ƃ��Ă��������B >>!TEMP!\%~n0_body.txt
	
	"!MAILCMD!" -f!MAILADDR! -h!MAILSERV! -s"%~nx0 - ����I��" -F"!TEMP!\%~n0_body.txt" !MAILADDR!
)
