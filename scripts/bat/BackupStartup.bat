@echo off

REM �o�b�N�A�b�v�g�p��
REM call :BackupStartup %~dp0c_startup1.zip

REM �����g�p��
REM call :RestoreStartup %~dp0c_startup1.zip

goto :EOF

REM -------------------------------------------------------------------
REM �J�����g���[�U�[�̃X�^�[�g�A�b�v���o�b�N�A�b�v
REM @param %1 ���k�t�@�C����
REM 7za�́ABAT�t�@�C���Ɠ����ꏊ�ɒu���Ă���O��
REM -------------------------------------------------------------------
:BackupStartup
	if exist %1 del %1
	"%~dp07za.exe" a -tzip -r %1 "%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup\*.lnk"
	exit /b

REM -------------------------------------------------------------------
REM �J�����g���[�U�[�̃X�^�[�g�A�b�v�𕜌�
REM @param %1 ���k�t�@�C����
REM 7za�́ABAT�t�@�C���Ɠ����ꏊ�ɒu���Ă���O��
REM -------------------------------------------------------------------
:RestoreStartup
	del "%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup\*.lnk"
	"%~dp07za.exe" x %1 -o"%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup"
	exit /b

