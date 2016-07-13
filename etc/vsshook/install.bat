copy \\ts00\home\kojima\vsshook\vsshook.dll %windir%\system32 /Y
regsvr32 /s %windir%\system32\vsshook.dll
copy \\ts00\home\kojima\vsshook\ssaddin.ini "C:\Program Files\Microsoft Visual Studio\Common\VSS\win32" /Y
pause
