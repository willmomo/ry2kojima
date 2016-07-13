@echo off

if "%1" == "" (
	echo 使用法：useradd [ユーザー名]
	goto :EOF
)

net user "%1" * /add
if errorlevel 1 (
	echo ユーザーの追加に失敗しました。
	goto :EOF
)

wmic useraccount where (Name = "%1") set PasswordExpires=False
