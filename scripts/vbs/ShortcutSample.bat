@echo off

REM ---------------------------------------------------------------------------
REM ショートカット系スクリプトの使用サンプル
REM 
REM 指定フォルダ以下の *.ink を列挙し、その情報を表示するプログラム
REM ---------------------------------------------------------------------------

for /R %1 %%i in (*.lnk) do (
	echo $ cscript //Nologo "%~dp0ShortcutView.vbs" "%%i"
	cscript //Nologo "%~dp0ShortcutView.vbs" "%%i"
)
