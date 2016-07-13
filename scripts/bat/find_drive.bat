@echo off

echo -- 有効なドライブを列挙
call :FINDDRV_A

echo -- ルートにMediaID.binの存在するドライブを検索
call :FINDDRV_B MediaID.bin
if "%FINDDRV_B%" == "" (
	echo 見つかりません
) else (
	echo %FINDDRV_B% に発見
)

goto :EOF

REM -------------------------------------------------------------------
REM ドライブを検索する
REM 有効なドライブを列挙する例
REM -------------------------------------------------------------------
:FINDDRV_A
	for %%i in (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) do (
		if exist %%i:\ (
			echo %%i あり
		)
	)
	exit /b

REM -------------------------------------------------------------------
REM ドライブを検索する
REM @param %1 ファイル名
REM 所定のファイルがあるドライブを見つけ出す例
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
