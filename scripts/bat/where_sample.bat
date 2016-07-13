@echo off
call :WHERE mysqldump
echo MYSQL_EXE=%WHERE_RET%
goto :EOF

:WHERE
	set WHERE_RET=
	for /f "usebackq tokens=*" %%i in (`where %1`) do @set WHERE_RET=%%i && exit /b
	for /f "usebackq tokens=*" %%i in (`where /r "C:\Program Files" %1`) do @set WHERE_RET=%%i && exit /b
	for /f "usebackq tokens=*" %%i in (`where /r "C:\Program Files (x86)" %1`) do @set WHERE_RET=%%i && exit /b
	exit /b
