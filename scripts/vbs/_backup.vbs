'------------------------------------------------------------------------------
' このスクリプトが存在するフォルダに、FREEDOM と PRO2500 のミラーリングをする
' スクリプト。
'
' ほかでも使いやすくなるように、ある程度の汎用性を持たせてある。
'------------------------------------------------------------------------------
Option Explicit

'------------------------------------------------------------------------------
' メイン処理
'------------------------------------------------------------------------------
Sub doMain_FREEDOM()
	Dim objMirror, objFs, objNet, orgPath, orgDirs, objTs, strExcludeFile

	Set objNet = WScript.CreateObject("WScript.Network")
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	Set objMirror = New mirrorExe

	strExcludeFile = objFs.BuildPath(objFs.GetSpecialFolder(2), objNet.ComputerName) & ".txt"
	Set objTs = objFs.CreateTextFile(strExcludeFile, True)
	objTs.WriteLine("\.dropbox.cache")
	objTs.WriteLine("\RECYCLER")
	objTs.WriteLine("\System Volume Information")
	objTs.Close
	
	objMirror.Mode = "backup-mirror"
	objMirror.LogPath = objFs.BuildPath(objFs.GetSpecialFolder(2), objNet.ComputerName)
	objMirror.ExcludeOption = strExcludeFile
	
	orgDirs = Array("D:\", "E:\")

	For Each orgPath In orgDirs
		objMirror.OrgPath = orgPath
		objMirror.MirPath = objFs.BuildPath(objFs.BuildPath(objFs.GetParentFolderName(WScript.ScriptFullName), objNet.ComputerName), Replace(objMirror.OrgPath, ":", "-drv"))
		objMirror.Run
	Next
End Sub

'------------------------------------------------------------------------------
' メイン処理
'------------------------------------------------------------------------------
Sub doMain_PRO2500()
	Dim objMirror, objFs, objNet, orgPath, orgDirs, objTs, strExcludeFile

	Set objNet = WScript.CreateObject("WScript.Network")
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	Set objMirror = New mirrorExe

	strExcludeFile = objFs.BuildPath(objFs.GetSpecialFolder(2), objNet.ComputerName) & ".txt"
	Set objTs = objFs.CreateTextFile(strExcludeFile, True)
	objTs.WriteLine("\.dropbox.cache")
	objTs.WriteLine("\RECYCLER")
	objTs.WriteLine("\System Volume Information")
	objTs.Close

	objMirror.Mode = "backup-mirror"
	objMirror.LogPath = objFs.BuildPath(objFs.GetSpecialFolder(2), objNet.ComputerName)
	objMirror.ExcludeOption = strExcludeFile
	
	orgDirs = Array("C:\fswiki", "D:\Users")

	For Each orgPath In orgDirs
		objMirror.OrgPath = orgPath
		objMirror.MirPath = objFs.BuildPath(objFs.BuildPath(objFs.GetParentFolderName(WScript.ScriptFullName), objNet.ComputerName), Replace(objMirror.OrgPath, ":", "-drv"))
		objMirror.Run
	Next
	
	objMirror.OrgPath = "\\LANDISK-B7C184\disk"
	objMirror.MirPath = objFs.BuildPath(objFs.GetParentFolderName(WScript.ScriptFullName), "LANDISK-B7C184\disk")
	objMirror.Run
End Sub

'------------------------------------------------------------------------------
' メイン処理
'------------------------------------------------------------------------------
Sub doMain()
	Dim objNet

	Set objNet = WScript.CreateObject("WScript.Network")
	
	If objNet.ComputerName = "FREEDOM" Then
		doMain_FREEDOM
	ElseIf objNet.ComputerName = "PRO2500" Then
		doMain_PRO2500
	Else
		MsgBox objNet.ComputerName & " は、未対応のPCです。", vbOKOnly Or vbExclamation, WScript.ScriptName
	End If
End Sub

'------------------------------------------------------------------------------

Call doMain()


'==============================================================================
'	ここから kjmlib.vbs からのコピー
'==============================================================================
'------------------------------------------------------------------------------
' mirror.exe ラッパークラス
'
' update: 2011/07/26
'------------------------------------------------------------------------------
Class mirrorExe
	'--------------------------------------------------------------------------
	' コピー元フォルダの設定と取得
	Private m_orgPath
	
	Public Property Let OrgPath(newValue)
		m_orgPath = newValue
	End Property
	
	Public Property Get OrgPath()
		OrgPath = m_orgPath
	End Property

	'--------------------------------------------------------------------------
	' コピー先フォルダの設定と取得
	Private m_mirPath
	
	Public Property Let MirPath(newValue)
		m_mirPath = newValue
	End Property
	
	Public Property Get MirPath()
		MirPath = m_mirPath
	End Property

	'--------------------------------------------------------------------------
	' --LogPath オプションの設定と取得
	Private m_logPath
	
	Public Property Let LogPath(newValue)
		m_logPath = newValue
	End Property
	
	Public Property Get LogPath()
		LogPath = m_logPath
	End Property
	
	'--------------------------------------------------------------------------
	' --exclude オプションの設定と取得
	Private m_excludeOpt

	Public Property Let ExcludeOption(newValue)
		m_excludeOpt = newValue
	End Property

	Public Property Get ExcludeOption()
		ExcludeOption = m_excludeOpt
	End Property

	'--------------------------------------------------------------------------
	' --mode オプションの設定と取得
	Private m_mode

	Public Property Let Mode(newValue)
		m_mode = newValue
	End Property

	Public Property Get Mode()
		Mode = m_mode
	End Property

	'--------------------------------------------------------------------------
	' --test オプションの設定と取得
	Private m_test

	Public Property Let Test(newValue)
		m_test = newValue
	End Property

	Public Property Get Test()
		Test = m_test
	End Property

	'--------------------------------------------------------------------------
	' mirror.exe の実行。
	'
	Public Sub Run()
		Dim strCmd
		
		' 実行ファイル名の指定、パスが通っているものとする。
		strCmd = "mirror.exe"
		
		' コピー元フォルダの指定
		If InStr(OrgPath, " ") > 0 Then
			strCmd = strCmd & " """ & OrgPath & """"
		Else
			strCmd = strCmd & " " & OrgPath
		End If
		
		' コピー先フォルダの指定
		If InStr(MirPath, " ") > 0 Then
			strCmd = strCmd & " """ & MirPath & """"
		Else
			strCmd = strCmd & " " & MirPath
		End If

		' --exclude オプションの指定
		If ExcludeOption <> "" Then
			If InStr(ExcludeOption, " ") > 0 Then
				strCmd = strCmd & " --exclude=""" & ExcludeOption & """"
			Else
				strCmd = strCmd & " --exclude=" & ExcludeOption
			End If
		End If

		' --LogPath オプションの指定
		If LogPath <> "" Then
			If InStr(LogPath, " ") > 0 Then
				strCmd = strCmd & " --logpath=""" & LogPath & """"
			Else
				strCmd = strCmd & " --logpath=" & LogPath
			End If
		End If

		' --mode オプションの指定
		If Mode <> "" Then strCmd = strCmd & " --mode=" & Mode

		' --test オプションの指定
		If Test <> False Then strCmd = strCmd & " --test"
		
		''WScript.Echo strCmd

		' ウィンドウを最新のサイズと位置で表示します。アクティブなウィンドウになります。
		''Call WScript.CreateObject("WScript.Shell").Run(strCmd, 1, True)
		
		' ウィンドウを最新のサイズと位置で表示します。アクティブなウィンドウは切り替わりません。
		Call WScript.CreateObject("WScript.Shell").Run(strCmd, 4, True)
		
		' ウィンドウを最小化ウィンドウとして表示します。アクティブなウィンドウは切り替わりません。
		''Call WScript.CreateObject("WScript.Shell").Run(strCmd, 7, True)
	End Sub

	'--------------------------------------------------------------------------
	' クラスのイニシャライズ
	'
	Private Sub Class_Initialize()
		m_test = False
	End Sub
End Class

'==============================================================================
'	ここまで kjmlib.vbs からのコピー
'==============================================================================
