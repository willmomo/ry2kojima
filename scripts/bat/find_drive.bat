@echo off

echo -- �L���ȃh���C�u���
call :FINDDRV_A

echo -- ���[�g��MediaID.bin�̑��݂���h���C�u������
call :FINDDRV_B MediaID.bin
if "%FINDDRV_B%" == "" (
	echo ������܂���
) else (
	echo %FINDDRV_B% �ɔ���
)

goto :EOF

REM -------------------------------------------------------------------
REM �h���C�u����������
REM �L���ȃh���C�u��񋓂����
REM -------------------------------------------------------------------
:FINDDRV_A
	for %%i in (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) do (
		if exist %%i:\ (
			echo %%i ����
		)
	)
	exit /b

REM -------------------------------------------------------------------
REM �h���C�u����������
REM @param %1 �t�@�C����
REM ����̃t�@�C��������h���C�u�������o����
REM -------------------------------------------------------------------
:FINDDRV_B
	set FINDDRV_B=
	for %%i in (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) do (
		if exist %%i:\%1 (
			set FINDDRV_B=%%i
			exit /b
		)
	)
	exit /b
