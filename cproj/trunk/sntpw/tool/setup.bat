@echo off
rem ---------------------------------------------------------------------------
rem sntpw ���Z�b�g�A�b�v����B
rem ---------------------------------------------------------------------------

set INSTPATH=C:\sntpw

rem ---------------------------------------------------------------------------
rem sntpw.exe/ini �� c:\sntpw �ɃR�s�[����B
rem ---------------------------------------------------------------------------
if not exist "%INSTPATH%" md "%INSTPATH%"
copy "%~dp0sntpw.exe" "%INSTPATH%"
copy "%~dp0sntpw.ini" "%INSTPATH%"

rem ---------------------------------------------------------------------------
rem Windows Time(w32time) �̏�Ԃ��J�n�̂Ƃ��́A��~����
rem ---------------------------------------------------------------------------
sc query w32time|find "RUNNING"
if not errorlevel 1 net stop w32time

rem ---------------------------------------------------------------------------
rem Windows Time(w32time) �̃X�^�[�g�A�b�v�̎�ނ������̂Ƃ��́A�蓮�ɂ���
rem ---------------------------------------------------------------------------
sc qc w32time|find "AUTO_START"
if not errorlevel 1 sc config w32time start= demand

pause
