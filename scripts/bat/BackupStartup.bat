@echo off

REM バックアップ使用例
REM call :BackupStartup %~dp0c_startup1.zip

REM 復元使用例
REM call :RestoreStartup %~dp0c_startup1.zip

goto :EOF

REM -------------------------------------------------------------------
REM カレントユーザーのスタートアップをバックアップ
REM @param %1 圧縮ファイル名
REM 7zaは、BATファイルと同じ場所に置いてある前提
REM -------------------------------------------------------------------
:BackupStartup
	if exist %1 del %1
	"%~dp07za.exe" a -tzip -r %1 "%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup\*.lnk"
	exit /b

REM -------------------------------------------------------------------
REM カレントユーザーのスタートアップを復元
REM @param %1 圧縮ファイル名
REM 7zaは、BATファイルと同じ場所に置いてある前提
REM -------------------------------------------------------------------
:RestoreStartup
	del "%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup\*.lnk"
	"%~dp07za.exe" x %1 -o"%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup"
	exit /b

