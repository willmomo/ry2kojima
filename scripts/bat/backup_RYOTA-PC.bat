@echo off

rem ===================================================================
rem ���̃o�b�`�t�@�C��������ꏊ�Ƀo�b�N�A�b�v���쐬���܂��B
rem �o�b�N�A�b�v��Ƀt�@�C�����R�s�[���Ă���g�p���Ă��������B
rem ===================================================================

call :ENBAK
call :NASBAK
call :LOCALBAK

REM �o�b�N�A�b�v�I�����ɃV���b�g�_�E�����������Ƃ��́A�R�����g���O��
REM shutdown /s /t 0

goto :EOF

rem ===================================================================

rem -------------------------------------------------------------------
rem NAS�̃o�b�N�A�b�v
rem -------------------------------------------------------------------
:NASBAK
	echo $ NAS�̃o�b�N�A�b�v

	set NAS=LANDISK
	net use \\%NAS%\disk gat-x105 /user:ryota
	mirror --mode=backup-mirror --logpath="%TEMP%\log_%~n0" "\\%NAS%\disk" "%~dp0%NAS%\disk"
	if errorlevel 1 goto :EOF
	exit /b

rem -------------------------------------------------------------------
rem ���[�J���t�@�C���̃o�b�N�A�b�v
rem -------------------------------------------------------------------
:LOCALBAK
	echo $ ���[�J���t�@�C���̃o�b�N�A�b�v

	setlocal enabledelayedexpansion
	for %%i in (Documents Dropbox GoogleDrive Music Pictures SkyDrive Videos) do (
		set WORK=!USERPROFILE!\%%i
		mirror --exclude="%~dpn0.exc" --mode=backup-mirror --logpath="!TEMP!\log_%~n0" "!WORK!" "%~dp0!USERDOMAIN!\!WORK::=!"
		if errorlevel 1 goto :EOF
	)
	exit /b

rem -------------------------------------------------------------------
rem Evernote�̃o�b�N�A�b�v
rem -------------------------------------------------------------------
:ENBAK
	echo $ Evernote�̃o�b�N�A�b�v

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
