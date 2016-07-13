@echo off

rem ===================================================================
rem このバッチファイルがある場所にバックアップを作成します。
rem バックアップ先にファイルをコピーしてから使用してください。
rem ===================================================================

call :ENBAK
call :NASBAK
call :LOCALBAK

REM バックアップ終了時にシャットダウンさせたいときは、コメントを外す
REM shutdown /s /t 0

goto :EOF

rem ===================================================================

rem -------------------------------------------------------------------
rem NASのバックアップ
rem -------------------------------------------------------------------
:NASBAK
	echo $ NASのバックアップ

	set NAS=LANDISK
	net use \\%NAS%\disk gat-x105 /user:ryota
	mirror --mode=backup-mirror --logpath="%TEMP%\log_%~n0" "\\%NAS%\disk" "%~dp0%NAS%\disk"
	if errorlevel 1 goto :EOF
	exit /b

rem -------------------------------------------------------------------
rem ローカルファイルのバックアップ
rem -------------------------------------------------------------------
:LOCALBAK
	echo $ ローカルファイルのバックアップ

	setlocal enabledelayedexpansion
	for %%i in (Documents Dropbox GoogleDrive Music Pictures SkyDrive Videos) do (
		set WORK=!USERPROFILE!\%%i
		mirror --exclude="%~dpn0.exc" --mode=backup-mirror --logpath="!TEMP!\log_%~n0" "!WORK!" "%~dp0!USERDOMAIN!\!WORK::=!"
		if errorlevel 1 goto :EOF
	)
	exit /b

rem -------------------------------------------------------------------
rem Evernoteのバックアップ
rem -------------------------------------------------------------------
:ENBAK
	echo $ Evernoteのバックアップ

	if exist "%ProgramFiles%\Evernote\Evernote\ENScript.exe" (
	     set ENSCRIPT="%ProgramFiles%\Evernote\Evernote\ENScript.exe"
	) else (
	     set ENSCRIPT="%ProgramFiles(x86)%\Evernote\Evernote\ENScript.exe"
	)

	echo - %ENSCRIPT% syncDatabase
	%ENSCRIPT% syncDatabase /u willmomo /p lm314v21

	echo - %ENSCRIPT% exportNotes /q any: /f "%~dp0%USERDOMAIN%\EvernoteBackup.enex"
	%ENSCRIPT% exportNotes /q any: /f "%~dp0%USERDOMAIN%\EvernoteBackup.enex"
	exit /b
