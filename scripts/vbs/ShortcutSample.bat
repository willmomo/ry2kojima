@echo off

REM ---------------------------------------------------------------------------
REM �V���[�g�J�b�g�n�X�N���v�g�̎g�p�T���v��
REM 
REM �w��t�H���_�ȉ��� *.ink ��񋓂��A���̏���\������v���O����
REM ---------------------------------------------------------------------------

for /R %1 %%i in (*.lnk) do (
	echo $ cscript //Nologo "%~dp0ShortcutView.vbs" "%%i"
	cscript //Nologo "%~dp0ShortcutView.vbs" "%%i"
)
