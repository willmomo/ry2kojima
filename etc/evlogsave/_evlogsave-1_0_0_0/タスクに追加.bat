@echo off

REM schtasks /Create /TN イベントログ保存 /RU {user} /RP {password} /SC DAILY /ST 19:00:00 /TR "\"C:\Program Files\evlogsave\evlogsave.exe\" -o \\nas\backup\evlog -s 30"
schtasks /Create /TN イベントログ保存 /SC DAILY /ST 19:00:00 /TR "\"C:\Program Files\evlogsave\evlogsave.exe\" -o \\nas\backup\evlog -s 30"
if errorlevel 1 (
	echo %errorlevel%
	echo タスクの追加に失敗しました。
) else (
	echo タスクに「イベントログ保存」を追加しました。
)

pause
