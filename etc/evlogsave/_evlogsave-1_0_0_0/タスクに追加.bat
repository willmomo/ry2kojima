@echo off

REM schtasks /Create /TN �C�x���g���O�ۑ� /RU {user} /RP {password} /SC DAILY /ST 19:00:00 /TR "\"C:\Program Files\evlogsave\evlogsave.exe\" -o \\nas\backup\evlog -s 30"
schtasks /Create /TN �C�x���g���O�ۑ� /SC DAILY /ST 19:00:00 /TR "\"C:\Program Files\evlogsave\evlogsave.exe\" -o \\nas\backup\evlog -s 30"
if errorlevel 1 (
	echo %errorlevel%
	echo �^�X�N�̒ǉ��Ɏ��s���܂����B
) else (
	echo �^�X�N�Ɂu�C�x���g���O�ۑ��v��ǉ����܂����B
)

pause
