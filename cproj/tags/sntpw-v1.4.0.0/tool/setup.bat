@echo off
rem ---------------------------------------------------------------------------
rem sntpw をセットアップする。
rem ---------------------------------------------------------------------------

rem ---------------------------------------------------------------------------
rem sntpw.exe/ini を c:\sntpw にコピーする。
rem ---------------------------------------------------------------------------
if not exist c:\sntpw md c:\sntpw
copy %~dp0sntpw.exe c:\sntpw
copy %~dp0sntpw.ini c:\sntpw

rem ---------------------------------------------------------------------------
rem Windows Time(w32time) の状態が開始のときは、停止する
rem ---------------------------------------------------------------------------
sc query w32time|find "RUNNING"
if not errorlevel 1 net stop w32time

rem ---------------------------------------------------------------------------
rem Windows Time(w32time) のスタートアップの種類が自動のときは、手動にする
rem ---------------------------------------------------------------------------
sc qc w32time|find "AUTO_START"
if not errorlevel 1 sc config w32time start= demand

pause
