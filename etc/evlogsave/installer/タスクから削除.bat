@echo off

schtasks /Delete /TN イベントログ保存
if errorlevel 1 (
	echo %errorlevel%
	echo タスク「イベントログ保存」の削除に失敗しました。
)

pause
