@echo off

if "%1" == "" (
	echo �g�p�@�Fuseradd [���[�U�[��]
	goto :EOF
)

net user "%1" * /add
if errorlevel 1 (
	echo ���[�U�[�̒ǉ��Ɏ��s���܂����B
	goto :EOF
)

wmic useraccount where (Name = "%1") set PasswordExpires=False
