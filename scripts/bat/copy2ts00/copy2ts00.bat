@echo off
setlocal enabledelayedexpansion

set MAILSERV=sec.sanko-net.co.jp
set MAILCMD=%~dp0smail.exe
set MIRCMD=%~dp0mirror.exe
set MIREXL=%~dp0exclude.txt

if "%1" == "clean" (
	if exist "!TEMP!\%~n0_mail.txt" del "!TEMP!\%~n0_mail.txt"
	echo メールアドレスを情報を削除しました。
	goto :EOF
)

if not exist "!TEMP!\%~n0_mail.txt" (
	set /p MAILADDR=あなたのメールアドレスを入力してください: 
	echo !MAILADDR!>"!TEMP!\%~n0_mail.txt"
	echo メールアドレスを保存しました。
	goto :EOF
)

for /f %%i in (!TEMP!\%~n0_mail.txt) do (
	set MAILADDR=%%i
)

"!MIRCMD!" --mode=backup-mirror --exclude="!MIREXL!" --logpath=!TEMP!\logmir "!USERPROFILE!\Documents" "\\ts00\home\!USERNAME!\!COMPUTERNAME!\Documents"

if errorlevel 1 (
	echo %~nx0 - 異常終了 >!TEMP!\%~n0_body.txt
	echo. >>!TEMP!\%~n0_body.txt
	echo エラーが発生しまいた。 >>!TEMP!\%~n0_body.txt
	echo 詳細は `!TEMP!\logmir` を参照してください。 >>!TEMP!\%~n0_body.txt
	
	"!MAILCMD!" -f!MAILADDR! -h!MAILSERV! -s"%~nx0 - 異常終了" -F"!TEMP!\%~n0_body.txt" !MAILADDR!
) else (
	echo %~nx0 - 正常終了 >!TEMP!\%~n0_body.txt
	echo. >>!TEMP!\%~n0_body.txt
	echo 正常終了しました。 >>!TEMP!\%~n0_body.txt
	echo 処理の詳細は `!TEMP!\logmir` を参照してください。 >>!TEMP!\%~n0_body.txt
	
	"!MAILCMD!" -f!MAILADDR! -h!MAILSERV! -s"%~nx0 - 正常終了" -F"!TEMP!\%~n0_body.txt" !MAILADDR!
)
